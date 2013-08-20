package martyr.parser;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Iterator;
import java.util.List;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.TransformerException;

import martyr.query.QueryCentral;
import martyr.struct.AOWLClass;
import martyr.struct.AOWLDTProp;
import martyr.struct.Ontology;

import org.w3c.dom.Document;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;


import com.hp.hpl.jena.ontology.OntModelSpec;
import com.hp.hpl.jena.query.ARQ;
import com.hp.hpl.jena.query.Dataset;
import com.hp.hpl.jena.query.Query;
import com.hp.hpl.jena.query.QueryExecution;
import com.hp.hpl.jena.query.QueryExecutionFactory;
import com.hp.hpl.jena.query.QueryFactory;
import com.hp.hpl.jena.query.ReadWrite;
import com.hp.hpl.jena.query.ResultSet;
import com.hp.hpl.jena.query.ResultSetFormatter;
import com.hp.hpl.jena.rdf.model.Model;
import com.hp.hpl.jena.rdf.model.ModelFactory;
import com.hp.hpl.jena.rdf.model.RDFNode;
import com.hp.hpl.jena.rdf.model.ResIterator;
import com.hp.hpl.jena.sparql.util.Context;
import com.hp.hpl.jena.tdb.TDBFactory;
import com.hp.hpl.jena.tdb.base.file.Location;
import com.hp.hpl.jena.update.UpdateFactory;
import com.hp.hpl.jena.update.UpdateRequest;


//TODO implementar los maintags restantes: propiedad inversa, propiedad transitiva
public class Parser {
	private Ontology action;
	private QueryCentral query;
	private File xml;
	private DocumentBuilderFactory dbFactory;
	private DocumentBuilder dBuilder;
	private Document doc;
	private String AOWL = "aowl:",RDF = "rdf:",OWL="owl:",RDFS="rdfs:";
	private String attr_regex = "[.\"]",resource_regex = "[.\"#]";
	//	Main Ontology tags
	private String MainTags[]={OWL+"Class",OWL+"DatatypeProperty",OWL+"ObjectProperty",AOWL+"ActiveProperty",AOWL+"Event",OWL+"TransitiveProperty"};
	//SubTags, cada arreglo corresponde a las etiquetas de cada etiqueta principal
	private String Properties[][]={{RDFS+"subClassOf"},
							{RDFS+"domain"},
							{RDFS+"domain",RDFS+"range",OWL+"inverseOf"},
							{RDFS+"domain",AOWL+"condition"},
							{AOWL+"subEventOf"},
							 {RDFS+"label",RDFS+"domain",RDFS+"range",OWL+"inverseOf"}
							 };
	//Tags correspondientes a una condicion
	private String condTags[] = {AOWL+"refEvent",AOWL+"complex",AOWL+"reactiveValue"};
	//Tags correspondiente a un aowl:complex
	private String cmplxTags[] ={AOWL+"refProperty",AOWL+"symbol",AOWL+"BooleanCond"};
	private String validsymbols[] = {"=","!=","<=",">=",">","<"};
	
	//Tags correspondientes al abox
	//private String InstanceTags[] ={OWL+"NamedIndividual"};
	private String InstProps[]={RDF+"type",RDF+"value"};
	
	Parser() throws ParserConfigurationException{
		dbFactory = DocumentBuilderFactory.newInstance();
		dbFactory.setNamespaceAware(true);
		dBuilder = dbFactory.newDocumentBuilder();
		this.query = new QueryCentral();
		this.query.load();
		this.action = this.query.getAction();
	}
	//Ontology Parser Methods
	/**
	 * Carga del documento XML
	 * @param filepath
	 * @throws IOException
	 * @throws SAXException
	 */
	private void loadDoc(String filepath)throws IOException, SAXException{
		xml = new File(filepath);
		doc = dBuilder.parse(xml);
		doc.getDocumentElement().normalize();
	}

	//estas dos funciones extraen y limpian el nombre del rdfs:about / rdf:id
	/**
	 * Extrae el ID desde el String pasado como parametro
	 * @param attr
	 * @return El ID ya procesado
	 */
	 String refinedAttrID(String attr){
			if(attr.contains("#")){
				//System.out.println(attr.substring(attr.lastIndexOf('#')+1,attr.length()-1).trim());
				//obtener lo que este despues del ultimo #
				return attr.substring(attr.lastIndexOf('#')+1,attr.length()-1).trim();
			}else{
				return attr.split(attr_regex)[1];
			}
		}
	/**
	 * 	Obtener el string del nodo pasado como parametro al pasarlo a otra funcion encargada de procesarlo a nivel de cadena
	 * @param node analizado para determinar si posee un ID
	 * @return String del ID del nodo
	 */
		String getSemanticID(Node node){
			NamedNodeMap attrlist = node.getAttributes();
			for(int index=0;index<attrlist.getLength();index++){
				String attr = attrlist.item(index).toString();
				if(attr.contains("rdf:about")||
					attr.contains("rdfs:about")||
					attr.contains("rdf:ID")||
					attr.contains("rdf:resource")||
					attr.contains("aowl")){
						return this.refinedAttrID(attr);
				}
			}
			return "";//que hacer si no tiene ni id ni nada ? -> en teoria esto nunca sucede
			//esta funcion tomara el id o about que salga primero.
		}
		
		/**
		 * Funcion para obtener el tipo de dato de una propiedad
		 * @param node del cual se obtiene la información
		 * @return int con el tipo de dato relacionado 0 entero, 1 cadena, 2 real, -1 sino tiene
		 */
		int getDataType(Node node){
			NamedNodeMap attrlist = node.getAttributes();
			for(int index=0;index<attrlist.getLength();index++){
				String attr = attrlist.item(index).toString();
				if(attr.contains("rdf:type")){
					//por ahora solo dos tipos de propiedades entero(0) y cadena(1)
					if(this.refinedAttrID(attr).equals("xsd:integer"))return AOWLDTProp.INT_PROP;
					if(this.refinedAttrID(attr).equals("xsd:double"))return AOWLDTProp.DOUBLE_PROP;
					if(this.refinedAttrID(attr).equals("xsd:string"))return AOWLDTProp.STRING_PROP;
				}
			}
			return -1;
		}
/**
 * Funcion para determinar que la condicion posee un simbolo adecuado
 * @param symbol a analizar
 * @return boolean de acuerdo a si es valido, revisar simbolos en el arreglo validsymbols
 */
	private boolean validSymbol(String symbol){
		List<String> validos = Arrays.asList(validsymbols);
		return validos.contains(symbol);
	}
	
/**
 * Descompone la etiqueta complex del active property en sus componentes atomicos para su procesamiento 
 * @param cmplx
 */
	 private void extractCmplx(Node cmplx){
		 NodeList cmplx_childs = cmplx.getChildNodes().item(1).getChildNodes();
		 List cmplx_items = Arrays.asList(cmplxTags);
		 String attr, nname,prop="noprop",symb="nosymb",cond="nocond";
		 for(int index =0 ; index< cmplx_childs.getLength();index++){
			 Node parts = cmplx_childs.item(index);
			 nname = parts.getNodeName();
			 if(cmplx_items.contains(nname)){
				 attr = this.getSemanticID(parts);
				 
				 prop = (cmplx_items.indexOf(nname)==0 )? attr:prop;
				 symb = (cmplx_items.indexOf(nname)==1 && this.validSymbol(attr))? attr:symb;
				 cond = (cmplx_items.indexOf(nname)==2)? attr:cond;
//				 System.out.println(parts.getNodeName()+" "+attr);
			 }
			 
		 }
		 //System.out.println(prop+" "+symb+" "+cond);
		 action.add(prop, symb, cond);
		 
	 }
/**
 * Descompone y analiza las etiquetas presentes en un nodo condition de un Active Property
 * @param condition
 */
	 private void extractCondTags(Node condition){
		 NodeList f_inside,s_inside, nl = condition.getChildNodes();
		 List cond = Arrays.asList(condTags), cmplx_props=Arrays.asList(cmplxTags);
		 String id,nname="",valor="novalue",evento="noevent";
		// System.out.println("extractCondTags");
		 if(nl.item(1).getChildNodes().getLength()>0){//control para elemento vacio fantasma que aparece
			 f_inside = nl.item(1).getChildNodes();//Elementos Dentro del primero Description
			 	for (int index =0; index<f_inside.getLength();index++){
			 		if(cond.contains(f_inside.item(index).getNodeName())){//Si es un tag conocido
			 			Node cond_elem = f_inside.item(index);
			 			nname = cond_elem.getNodeName();
			 			if(!nname.equals(cond.get(1))){//si tiene Atributos
			 				id = this.getSemanticID(cond_elem);
							evento = (cond.indexOf(nname)==0)? id:evento;
							valor = (cond.indexOf(nname)==2 )? id:valor;
			 				//System.out.println(cond_elem.getNodeName()+" "+id);
			 			}else{//si es complex
			 				//System.out.println("is complex");
			 				this.extractCmplx(cond_elem);//descomponer y traducir el complex
			 			}
			 		}else{
			 			//System.out.println(f_inside.item(index).getNodeName());
			 		}
			 	}
			 	//System.out.println(evento+" "+valor);
			 	//System.out.println(nl.getLength());
			 	action.add(evento, valor);
		 }else{
			// System.out.println("empty nl");
		 }
		 
	 }
	 
/**
 * dada una de las MainTags pasarlas al formato necesario y agregarlas a la Ontologia y a MIOA
 * TODO refactor	
 * @param tag etiqueta analizada
 */
	private void refine(String tag){
		
		NodeList nl,childs,sub;
		Node item,subItem;
		String id,nname,domain="",range="",subiName;
		List<String> subs = new ArrayList<String>();
		int ntag,nprop,listLength,childLength,dataType;
		//TODO:darle uso al success
		boolean success;
		List Props;
		nl = doc.getElementsByTagName(tag);
		listLength = nl.getLength();
		ntag = Arrays.asList(MainTags).indexOf(tag);//Saber que etiqueta principal es
		Props = Arrays.asList(Properties[ntag]);//Attrs posibles para la etiqueta principal
		for(int index=0; index<listLength;index++){// se navegan los nodos correspondientes a la etiqueta
			item = nl.item(index);
			id = this.getSemanticID(item);//ID obtenido y limpio
			dataType = this.getDataType(item);
			//System.out.println(dataType);
			//System.out.println(tag+" "+id);
			childs = item.getChildNodes();
			childLength = childs.getLength();
			for(int cindex=0;cindex<childLength;cindex++){//por cada una de las etiquetas contenidas dentro de la principal
				subItem=childs.item(cindex);
				subiName=subItem.getNodeName();
				//System.out.println(subiName);
				if(Props.contains(subiName) && subItem.getAttributes().getLength()>0){//ver si la propiedad esta en la lista
					nprop = Props.indexOf(subiName);
					nname = this.getSemanticID(subItem);
					//System.out.println(nname);
//					System.out.println(subiName+" "+nname);
					//En este punto extraemos las propiedades de los tags que no son active properties
					//TODO cambiar los numeros por constantes
					if(ntag==0 || ntag ==4){//si es una clase o un evento
						//encolar las subclases/subeventos
						subs.add(nname);
					}else if(ntag==1||ntag==3){//si es un Dtprop
						//obtener el dominio
						domain = nname;
					}else if(ntag==2){//si es un objprop
						//obtener el dominio y el rango
						domain = (nprop==0)? nname:domain;
						range = (nprop==1)?nname:range;
						//TODO que hacer si es el inverse
					}else{//Transtive Property
						
					}
				}else if(subItem.getNodeName().equals(Properties[3][1])){//En este caso es que se dan las propiedades activas
					//una condicion tiene dentro un rdf:Description,
					//que dentro tiene un evento y un complex que tiene dentro otro rdf:description
					//dentro de este ultimo estan los 3 tags que importan
					//System.out.println("cond tags");
					extractCondTags(subItem);
				}	
			}
			//TODO si es un transitive property pasar la info como un objprop ?
			//TODO implementar transitive prop despues.
			//si se añadira una prop activa
			success = (ntag==3)?action.add(ntag,id,domain,dataType):action.add(ntag,id,domain,range,subs,dataType);
			subs.clear();
		}//otro nodo
	}
		
/**
 * Funcion de carga del TBOX de la ontologia
 * @param filepath de la ontologia cargada
 * @param test si se esta probando la carga del TBOX
 * @throws ParserConfigurationException
 * @throws TransformerException
 * @throws IOException
 */
	public void loadtbox(String filepath,boolean test) throws ParserConfigurationException, TransformerException, IOException{
		if(test){//Datos de prueba
			this.testTBOX();
		}else{//Datos reales
			try {
				this.loadDoc(filepath);
				this.query.loadDoc(filepath);
			}
			catch(IOException e){e.printStackTrace();}
			catch(SAXException e){e.printStackTrace();}
			Iterator<String> tag = Arrays.asList(MainTags).iterator();
			
			while(tag.hasNext()){//add elements by the tag names passed as parameter
				String value = tag.next();
				this.refine(value);
			}
		}		
		
	}
	
	
	/**
	 * Funcion que se encarga de cargas las instancias de la ontologia
	 * @return boolean si todo el proceso de carga fue exitoso
	 */
	public boolean instanciar(){
		boolean success = false;
		int prop_length;
		String clase="", instancia, propiedad, valor;
		List<AOWLClass> clases = action.getClases();
		Iterator<AOWLClass> iter_class = clases.iterator();
		AOWLClass inst_type;
		List<String> propiedades;
		Iterator<String> prop_iter;
		String props, value;
		while(iter_class.hasNext()){
			inst_type = iter_class.next();
			NodeList instancias = doc.getElementsByTagName("tbox:"+inst_type.getID()),
					 prop_list;
			int ntag;
			for(int item = 0; item < instancias.getLength(); item++){
				prop_list = instancias.item(item).getChildNodes();
				instancia = this.getSemanticID(instancias.item(item));
				action.create(instancia,inst_type.getID());
				//System.out.println(instancia);
				for(int prop = 0; prop < prop_list.getLength(); prop++){
					propiedades = action.propsByClassAsString(inst_type.getID());
					prop_length = prop_list.item(prop).getNodeName().split(":").length;
					if(prop_length > 1 ){
							props = prop_list.item(prop).getNodeName().split(":")[1];
							value = prop_list.item(prop).getTextContent();
							System.out.println("la tiene "+props);
							System.out.println("y vale: "+value);
						    action.setValues(instancia, inst_type.getID(), props, value);
					}
				}
			}
		}
		return true;
	}

	/**
	 * Funcion para cargar el ABOX de la Ontologia cargada
	 * @param test si se esta probando o se esta cargando una ontologia real
	 */
	public void loadabox(boolean test){
		if(test){//instancias de prueba	
			this.testABOX();
			//verificar todo
			action.test();
			action.showNumbers();
		}else{//Instancias reales
			this.instanciar();
		}
		

	}
/**
 * Funcion maestra de carga
 * @param filepath direccion de la ontologia a cargar
 * @param test si se esta probando o no
 * @throws ParserConfigurationException 
 * @throws TransformerException
 * @throws IOException
 */
	public void load(String filepath,boolean test) throws ParserConfigurationException, TransformerException, IOException{
		//TODO borrar al hacer las pruebas generales
		this.getQuery().clearOnt();
		this.getQuery().setOwlSource(filepath);
		this.loadtbox(filepath, test);
		this.loadabox(test);
	}

/**
 * Metodo para probar de manera aislada el parsing del TBOX
 * 
 */
	public void testTBOX(){
		//clases
		System.out.println("clase fenMeteorologico");
		action.add(0, "fenMeteorologico","","",new ArrayList<String>(),-1);
		System.out.println("clase fenClimatico");
		action.add(0,"fenClimatico","","",new ArrayList<String>(),-1);
		ArrayList<String> supc = new ArrayList<String>();
		supc.add("fenMeteorologico");
		supc.add("fenClimatico");
		System.out.println("clase huracan subclases fenMeteorologico y fenClimatico");
		action.add(0, "huracan", "", "", supc, -1);
		//propiedades
		
		System.out.println("propiedad NumMuertos clase fenClimatico");
		action.add(1, "NumMuertos", "fenClimatico", "", new ArrayList<String>(), 0);
		System.out.println("propiedad HuboPerdidaHumana clase fenClimatico");
		action.add(1,"HuboPerdidaHumana","fenClimatico","",new ArrayList<String>(),1);
		System.out.println("propiedad intensidad clase huracan");
		action.add(1,"intensidad","huracan","",new ArrayList<String>(), 0);
		//TODO implementar esta de op de subpropiedad
		
		//agregar evento
		System.out.println("agregando evento ");
		//agregar subprop
		System.out.println("complex");
		action.add("NumMuertos",">","0");
		System.out.println("condicion");
		action.add("HuboMuerte", "Si");
		//System.out.println("Prop Activa HuboMuerte clase fenClimatico");
		action.add(3,"HuboPerdidaHumana","fenClimatico","",new ArrayList<String>(),-1);
		
	}
/**
 * Funcion de prubea para probar la carga de las instancias	
 */
	public void testABOX(){
		System.out.println("instancia ike clase huracan");
		action.create("ike", "huracan");
		System.out.println("instancia desastreVargas clase fenClimatico");
		action.create("desastreVargas", "fenClimatico");
		System.out.println("setting Values");
		action.setValues("ike", "huracan", "NumMuertos", "125");
		action.setValues("ike", "huracan", "intensidad", "4");
		System.out.println("Execute");
		action.mioatize_objective("HuboMuerte");
		action.mioatize_execution("huracan", "HuboPerdidaHumana", 1);
	}
	
	/**
	 * 
	 * @param query Strin que contiene la consulta en actSPARQL
	 * @return
	 */
	public boolean execute(String query){
		return this.query.execute(query);
	}
	
	/**
	 * metodo consulta de la ontologia
	 * @return
	 */
	public Ontology getAction() {
		return action;
	}
	
	/**
	 * Metodo para modificar la ontologia
	 * @param action
	 */
	public void setAction(Ontology action) {
		this.action = action;
	}
	
	/**
	 * Consulta al objeto encargado de la manipulacion de las consultas
	 * @return el objeto de la clase QueryCentral
	 */
	public QueryCentral getQuery() {
		return query;
	}
	
	/**
	 * Metodo encargado de modificar el objeto manipulador de consultas
	 * @param query objeto por el cual se cambiara al QueryCentral actual
	 */
	public void setQuery(QueryCentral query) {
		this.query = query;
	}
	
	
	//Fin Ontology Parser Methods
	
	
	
}
