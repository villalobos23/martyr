package martyr.struct;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.jena.fuseki.http.UpdateRemote;
import org.openjena.atlas.lib.StrUtils;




import com.hp.hpl.jena.graph.Node;
import com.hp.hpl.jena.query.Dataset;
import com.hp.hpl.jena.query.DatasetFactory;
import com.hp.hpl.jena.query.Query;
import com.hp.hpl.jena.query.QueryExecution;
import com.hp.hpl.jena.query.QueryExecutionFactory;
import com.hp.hpl.jena.query.QueryFactory;
import com.hp.hpl.jena.query.ReadWrite;
import com.hp.hpl.jena.query.ResultSet;
import com.hp.hpl.jena.query.ResultSetFormatter;
import com.hp.hpl.jena.rdf.model.Model;
import com.hp.hpl.jena.rdf.model.ModelFactory;
import com.hp.hpl.jena.rdf.model.Property;
import com.hp.hpl.jena.rdf.model.Resource;
import com.hp.hpl.jena.rdf.model.Statement;
import com.hp.hpl.jena.sparql.JenaTransactionException;
import com.hp.hpl.jena.sparql.modify.request.QuadDataAcc;
import com.hp.hpl.jena.sparql.modify.request.UpdateCreate;
import com.hp.hpl.jena.sparql.modify.request.UpdateDataInsert;
import com.hp.hpl.jena.tdb.TDBFactory;
import com.hp.hpl.jena.tdb.base.file.Location;
import com.hp.hpl.jena.update.GraphStore;
import com.hp.hpl.jena.update.GraphStoreFactory;
import com.hp.hpl.jena.update.Update;
import com.hp.hpl.jena.update.UpdateAction;
import com.hp.hpl.jena.update.UpdateExecutionFactory;
import com.hp.hpl.jena.update.UpdateFactory;
import com.hp.hpl.jena.update.UpdateProcessor;
import com.hp.hpl.jena.update.UpdateRequest;

import de.fuberlin.wiwiss.ng4j.NamedGraph;
import de.fuberlin.wiwiss.ng4j.NamedGraphSet;
import de.fuberlin.wiwiss.ng4j.impl.NamedGraphSetImpl;
import de.fuberlin.wiwiss.ng4j.Quad;



public class Ontology implements Serializable{
	/**
	 * 
	 */
	private static final long serialVersionUID = -3865444863186454077L;
	//accesibles
	public static final int CLASS = 0;
	public static final int DATAPROP = 1;
	public static final int OBJPROP = 2;
	public static final int EVENT = 4;
	public static final int ACTPROP = 3;
	public static final int NOTMAINTAG = 5;
	private ArrayList<AOWLClass> clases;
	private ArrayList<AOWLEvent> eventos;
	private ArrayList<AOWLDTProp> dtprops;
	private ArrayList<AOWLObjProp> objprops;
	private ArrayList<AOWLActProp> actprops;
	private ArrayList<AOWLInstance> instancias;
	private String update_q = "";
	private ArrayList<AOWLComplex> cmplxStack;
	private ArrayList<AOWLCondition> condStack;
	//totalmente privado
	
	public Ontology(){
		System.loadLibrary("mioa");
	}
	
	public Ontology(boolean restart) {
		// TODO Auto-generated constructor stub
		System.loadLibrary("mioa");
		this.clean();
	}
	
	public void reload(){
		for(AOWLClass clase: clases){// clases
			this.mioatize_class(clase.getID());
			for(String subclase: clase.getSubclases()){
				this.mioatize_subclass(clase.getID(), subclase);
			}
		}
		
		for(AOWLDTProp dtprop: dtprops){
			this.mioatize_property(dtprop.getDomain(), dtprop.getID(), dtprop.getType());
		}
		
		for(AOWLObjProp objprop: objprops){
			this.mioatize_property(objprop.getDomain(), objprop.getID(), objprop.getType());
			this.mioatize_subclass(objprop.getRange(), objprop.getDomain());
		}
		
		for(AOWLActProp actprop: actprops){
			this.mioatize_property(actprop.getDominio(), actprop.getID(), actprop.getType());
			for(AOWLCondition cond : actprop.getCondiciones()){
				for(AOWLComplex cpmlx: cond.getComplex()){
					this.mioatize_activeproperty(actprop.getDominio(), 
												cond.getRefEvent(),
												actprop.getID(),
												cond.getReactiveValue(),
												cpmlx.getRefProp()+" "+cpmlx.getSymbol()+" "+cpmlx.getBooleanCond());
				}
			}
		}
		
		for(AOWLEvent evento: eventos){
			for(String subevento:evento.getSubeventos()){
				this.mioatize_subevent(subevento, evento.getID());
			}
		}
		
		for(AOWLInstance instancia: instancias){
			this.mioatize_create(instancia.getID(), instancia.getClase());
			Iterator it = instancia.getPropiedades().entrySet().iterator();
			while (it.hasNext()) {
				Map.Entry pair = (Map.Entry) it.next();
				this.mioatize_values(instancia.getID(), instancia.getClase(), pair.getKey().toString(), pair.getValue().toString());
				//it.remove();
			}
		}
	}
	
	public void clean(){
		cmplxStack = new ArrayList<AOWLComplex>();
		condStack = new ArrayList<AOWLCondition>();
		clases = new ArrayList<AOWLClass>();
		eventos = new ArrayList<AOWLEvent>();
		actprops = new ArrayList<AOWLActProp>();
		dtprops = new ArrayList<AOWLDTProp>();
		objprops = new ArrayList<AOWLObjProp>();
		instancias = new ArrayList<AOWLInstance>();
		this.cleanMIOA();
	}
	
	//se apilan los complex  para insertarlos por condicion
	private void stackCmplx(AOWLComplex cmplx){
				this.cmplxStack.add(cmplx);
	}
		
		//cuando se obtiene el valor reactivo se saben que estas son todos los complex de la cond. 
		//se agregan los cmplx que esten actualmente apilados, al final se limpia la pila de complex
	private void stackCond(AOWLCondition cond){
			this.condStack.add(cond);
			this.cmplxStack.clear();
	}
		
	//diferentes tipos de añadir
	/**
	 * Añadir generico hecho para ser usado por metodos mas especificos
	 * para cada tipo de nodo
	 * al final del add se limpian las pilas de complex y cond
	 * 
	 * @param tag: etiqueta
	 * @param id: id/about del tag
	 * @param subCE: sub clases o subeventos
	 * @param dominio: dominio de la propiedad
	 * @param rango: rango de la propiedad
	 * @param newCondition: Condición a almacenar
	 * @param newComplex: Complex a almacenar
	 * @return true si fue insertado de manera correcta, false en otro caso
	 * */
	public boolean add(int tag,
						String id,
						List<String> subCE, 
						String dominio,
						String rango, 
						AOWLCondition newCondition,
						AOWLComplex newComplex,
						int type){
		boolean check=false;
		
		//System.out.println(tag);
		
		switch(tag){
			//casos para los main tags
		//TODO añadir a estructura enlazada solo si se inserta bien en MIOA
			case CLASS:
				this.clases.add(new AOWLClass(id, subCE));
				//System.out.println("clase "+id+" agregada?:"+this.mioatize_class(new String(id)));
				this.mioatize_class(new String(id));
				for(String subcc:subCE){//aqui estan las superclases de la clase actual
					//System.out.println("subclase"+id+" agregada a"+subcc+"?"+this.mioatize_subclass(subcc, id));
					this.mioatize_subclass(id,subcc);
				}
				break;
				
			case OBJPROP:
					this.objprops.add(new AOWLObjProp(id,dominio,type,rango));
					//System.out.println(this.mioatize_property(dominio, id));
					//agregar la propiedad a la lista de propiedades
					this.mioatize_property(dominio, id,type);
					//Para habilitar la jerarquia de propiedades entre las clases es necesario generar este tipo de herencias
					this.mioatize_subclass(rango, dominio);
				break;
			case DATAPROP:
					this.dtprops.add(new AOWLDTProp(id,dominio,type));
					//System.out.println(this.mioatize_property(dominio, id));
					this.mioatize_property(dominio, id,type);
				break;
			case EVENT:
				this.eventos.add(new AOWLEvent(id,subCE));
				//System.out.println("Evento: "+id);
				for(String subee:subCE){
					//hijo padre
					this.mioatize_subevent(id,subee);
				}
				break;
			case ACTPROP:
				this.actprops.add(new AOWLActProp(id,dominio,condStack,type));
				this.mioatize_property(dominio, id,type);
				//System.out.println("ACT PROP");
				for(AOWLCondition ccond:condStack){
					//System.out.println("primer for");
					for(AOWLComplex ccmplx:ccond.getComplex()){
						//System.out.println("segundo for");
						System.out.println(" "+dominio+" "+ccond.getRefEvent()+" "+id+" "+ccond.getReactiveValue()+" "
								+ccmplx.getRefProp()+" "+ccmplx.getSymbol()+" "+ccmplx.getBooleanCond());
						this.mioatize_activeproperty(dominio, ccond.getRefEvent(), id, ccond.getReactiveValue(),
								ccmplx.getRefProp()+" "+ccmplx.getSymbol()+" "+ccmplx.getBooleanCond());
					}
				}
				
				condStack.clear();
				break;
			case NOTMAINTAG:// es un complex o un condition
				if(newComplex.getID().equals("DummyID")&& newCondition.getID().equals("")){//es un complex
//					System.out.println("complex");
					this.stackCmplx(newComplex);
				}else if(newComplex.getID().equals("")&& newCondition.getID().equals("DummyID")){//es un condition
					this.stackCond(newCondition);
//					System.out.println("condition");
				}else{
					//condition and complex both null
					return false;
				}
				break;
			default://es cualquier otro valor
				return false;
		}
		return true;
	}
	
	/**
	 * Añadir una clase, evento, non-act prop
	 * */
	public boolean add(int tag, String id, String domain,String range,List<String> sub,int type)
	{
		boolean success= add(tag,id,sub,domain,range,null,null,type);
		return success;
	}
	
	/**
	 * Añadir un ActProp
	 * */
	
	public boolean add(int tag,String id, String actDomain,int type){
		boolean success = add(tag, id, null, actDomain, null, null, null,type);
		return success;
	}
	
	/**
	 * Encolar un complex
	 * */
	public boolean add(String prop, String symb, String cond)
	{
		//System.out.println("COMPLEX");
		boolean success = add(NOTMAINTAG,null,null,null,null,new AOWLCondition(),new AOWLComplex(prop,symb,cond),-1);
		return success;
	}
	
	/**
	 * Encolar un condition
	 * */
	public boolean add(String evento, String valor)
	{
		//System.out.println("Condition");
		boolean success = add(NOTMAINTAG,null,null,null,null,new AOWLCondition(evento, cmplxStack, valor),new AOWLComplex(),-1);
		return success;
	}
	/**
	 * metodo para crear una instancia
	 * @param instName
	 * @param classname
	 * @return
	 */
	public boolean create(String instName, String classname){
		this.instancias.add(new AOWLInstance(instName, classname));
		boolean success = this.mioatize_create(instName, classname);
		return success;
	}
	/**
	 * Metodo para establecer los valores de las propiedades de la instancia
	 * @param instance
	 * @param classname
	 * @param prop
	 * @param value
	 * @return
	 */
	public boolean setValues(String instance,String classname, String prop, String value){
		this.getInstancia(instance).addPropiedad(prop, value);
		boolean success = this.mioatize_values(instance,classname,prop,value);
		return success;
	}
	
	/**
	 * 
	 * @param ID
	 * @return
	 */
	private AOWLInstance getInstancia(String ID){
		for (int i = 0 ; i< this.instancias.size(); i++){
			if (this.instancias.get(i).getID().equals(ID)){
				return this.instancias.get(i);
			}
		}
		return new AOWLInstance();
	}
	/**
	 * 
	 * @param tbox
	 * @return
	 */
	public boolean updateChanges(String tbox){
		AOWLInstance inst;
		int order = 0;
		tbox = "http://localhost/TBOX2.owl#";
		String delete = "DELETE{",
			   insert = "INSERT{",
			   where = "WHERE{",
			   header = "PREFIX tbox:<"+tbox+">";
		System.out.println("cantidad de instancias "+this.instancias.size());
		for (int i = 0 ; i < this.instancias.size(); i++){//Por cada instancia obtenemos sus propiedades y el valor que poseen
			inst = this.instancias.get(i);
			/*props = inst.getPropiedades().keySet();
			p = props.iterator();*/
			order = 0;
			System.out.println("Cantidad de propiedad de la instancia "+inst.getPropiedades().size());
			for(Map.Entry<String,String> props : inst.getPropiedades().entrySet()){
				String prop = props.getKey(), 
					   objeto = this.mioatize_getInsValue(inst.getClase(), inst.getID(), prop);
						System.out.println("prop "+prop);
						inst.getPropiedades().put(prop, objeto);
						System.out.println(inst.getID()+" "+prop+" "+objeto);
						//Generate query string
						//create the delete part, sujeto, propiedad, objeto 
						//delete = delete+" tbox:"+inst.getID()+" tbox:"+prop+" ?o .\n";
						delete = StrUtils.strjoinNL(delete," tbox:"+inst.getID()+" tbox:"+prop+" ?"+inst.getID()+order+" .");
						//create the insert part, sujeto propiedad, 
						//insert = insert+" tbox:"+inst.getID()+" tbox:"+prop+" "+objeto+".\n";
						insert = StrUtils.strjoinNL(insert," tbox:"+inst.getID()+" tbox:"+prop+" \""+objeto+"\" . ");
						System.out.println("New: "+objeto);
						//where = where+" tbox:"+inst.getID()+" tbox:"+prop+" ?o .\n";
						where = StrUtils.strjoinNL(where," tbox:"+inst.getID()+" tbox:"+prop+" ?"+inst.getID()+order+" .");
						order++;
			}
				
		}
		//Armamos la super cadena de consulta
		delete = delete+"}";
		insert = insert+"}";
		where = where+"}";
		this.update_q =StrUtils.strjoinNL(header,delete,insert,where);// header+delete+insert+where;
		return true;
	}
	
	/**
	 * 
	 */
	public void showNumbers(){
		System.out.println("clases:"+this.clases.size()+
				" eventos:"+this.eventos.size()+
				" dtprops:"+this.dtprops.size()+
				" objprops:"+this.objprops.size()+
				" actprops:"+this.actprops.size()+
				" instancias:"+this.instancias.size());
		System.out.println("MIOA:");
		this.test();
	}
	
	/**
	 *  Determina Los objetos AOWLDTProp de las propiedades de la clase especificada 
	 * @param domain nombre de la clase
	 * @return una lista de AOWLDTProps con las propiedades 
	 */
	public List<AOWLDTProp> propsByClass(String domain){
		List<AOWLDTProp> result = new ArrayList<AOWLDTProp>();
		Iterator<AOWLDTProp> prop_it = this.dtprops.iterator();
		AOWLDTProp checked_prop;
		while(prop_it.hasNext()){
			checked_prop = prop_it.next();
			if(checked_prop.getDomain().equalsIgnoreCase(domain)){
				result.add(checked_prop);
			}
		}
		return result;
	}
	/**
	 * Determina solo los nombres de las propiedades de la clase especificada
	 * @param domain nombre de la clase
	 * @return una lista de Strings con las propiedades
	 */
	public List<String> propsByClassAsString(String domain){
		List<String> result = new ArrayList<String>();
		Iterator<AOWLDTProp> prop_it = this.dtprops.iterator();
		AOWLDTProp checked_prop;
		while(prop_it.hasNext()){
			checked_prop = prop_it.next();
			if(checked_prop.getDomain().equalsIgnoreCase(domain)){
				result.add(checked_prop.getID());
			}
		}
		return result;
	}
	
	//Los JNI methods para la creación de la ontologia
		/**
		 * 
		 * @param classname 
		 * nombre identificador de la clase a insertar
		 * @return
		 * Retorna true si MIOA pudo insertar la clase, false sino se puede
		 */
		private native boolean mioatize_class(String classname);
		
		/**
		 * Agrega una sub clase a MIOA
		 * @param classname
		 * @param superclass
		 * @return boolean del resultado de la operacion
		 */
		private native boolean mioatize_subclass(String classname, String superclass);
		
		/***
		 * Agregar un subevento a MIOA
		 * @param eventname
		 * @param superevent
		 * @return boolean del exito de la operacion
		 */
		private native boolean mioatize_subevent(String eventname,String superevent);
		
		/**
		 * 
		 * @param classname
		 * @param propname
		 * @param type
		 * @return
		 */
		private native boolean mioatize_property(String classname, String propname,int type);
		
		/**
		 * Agregar una propiedad activa a MIOA
		 * @param classname
		 * @param eventname
		 * @param propname
		 * @param reactvalue
		 * @param condition
		 * @return boolean del exito de la operación
		 */
		private native boolean mioatize_activeproperty(String classname, String eventname, String propname, String reactvalue, String condition);
		
		/**
		 * Obtener el valor de una propiedad determinada de una instancia
		 * @param clase
		 * @param instancia
		 * @param prop
		 * @return el valor en String de la propiedad en cuestión
		 */
		private native String mioatize_getInsValue(String clase, String instancia, String prop);
		
		/**
		 * Hacer pruebas con MIOA y su inferencia
		 */
		public native void test();
		
	//JNI methods para instancias
		
		/**
		 * Crear una Instancia en MIOA
		 * @param instName
		 * @param classname
		 * @return boolean de la creación de la instancia
		 */
		private native boolean mioatize_create(String instName, String classname);
		
		/**
		 * Modifica el valor de alguna propiedad de una instancia
		 * @param instance
		 * @param classname
		 * @param prop
		 * @param value
		 * @return boolean del exito de la operacion
		 */
		private native boolean mioatize_values(String instance,String classname, String prop, String value);
	//la idea seria añadir un objetivo a la vez, vease un paralel o un serial y poder dividirlos por esos grupos
		
		/**
		 * Preparar a MIOA para la ejecución de un Evento
		 * @param evento
		 * @return si se pudo efectuar la planificación del objetivo
		 */
		public native boolean mioatize_objective(String evento);
		
		/**
		 * Llevar a cambio la parte activa de la Consulta mediante los objetivos planificados
		 * @param classname
		 * @param prop
		 * @param stat
		 * @return si se pudo ejecutar la operación de activación
		 */
		public native boolean mioatize_execution(String classname, String prop,int stat);
		
		/**
		 * Para limpiar a MIOA y reinicializarla
		 */
		private native void cleanMIOA();
		/**
		 * getters vs stters
		 * 
		 * */	
		
		/**Getter para las clases
		 * @return la lista de clases
		 */
		public List<AOWLClass> getClases() {
			return clases;
		}

		/**
		 * Getter para los eventos
		 * @return la lista de Eventos
		 */
		public List<AOWLEvent> getEventos() {
			return eventos;
		}
		
		/**
		 * @return lista de los datatype property de la ontología
		 */
		public List<AOWLDTProp> getDtprops() {
			return dtprops;
		}

		/**
		 * @return lista de las propiedades de objeto de la Ontología
		 */
		public List<AOWLObjProp> getObjprops() {
			return objprops;
		}

		/**
		 * @return las propiedades activas de la Ontología
		 */
		public List<AOWLActProp> getActprops() {
			return actprops;
		}

		/**
		 * 
		 * @return las instancias de la ontologia
		 */
		public List<AOWLInstance> getInstancias() {
			return instancias;
		}
		/**
		 * @return consulta de acutalización
		 */
		public String getUpdate_q() {
			return update_q;
		}

		/**
		 * Setter para actualizar la consulta que cambiara el estado de la ontologia a causa de los eventos disparados
		 * @param update_q
		 */
		public void setUpdate_q(String update_q) {
			this.update_q = update_q;
		}
		
		
}
