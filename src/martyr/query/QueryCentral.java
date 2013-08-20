package martyr.query;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.Arrays;
import java.util.Iterator;
import java.util.List;

import org.apache.jena.fuseki.http.UpdateRemote;
import org.openjena.atlas.lib.StrUtils;

import martyr.struct.Ontology;

import com.hp.hpl.jena.graph.Node;
import com.hp.hpl.jena.query.Dataset;
import com.hp.hpl.jena.query.QueryExecution;
import com.hp.hpl.jena.query.QueryExecutionFactory;
import com.hp.hpl.jena.query.ReadWrite;
import com.hp.hpl.jena.query.ResultSet;
import com.hp.hpl.jena.query.ResultSetFormatter;
import com.hp.hpl.jena.rdf.model.Model;
import com.hp.hpl.jena.rdf.model.ModelFactory;
import com.hp.hpl.jena.rdf.model.Property;
import com.hp.hpl.jena.rdf.model.Resource;
import com.hp.hpl.jena.rdf.model.Statement;
import com.hp.hpl.jena.sparql.modify.request.QuadDataAcc;
import com.hp.hpl.jena.sparql.modify.request.UpdateDataInsert;
import com.hp.hpl.jena.tdb.TDBFactory;
import com.hp.hpl.jena.tdb.base.file.Location;
import com.hp.hpl.jena.update.GraphStore;
import com.hp.hpl.jena.update.GraphStoreFactory;
import com.hp.hpl.jena.update.Update;
import com.hp.hpl.jena.update.UpdateExecutionFactory;
import com.hp.hpl.jena.update.UpdateFactory;
import com.hp.hpl.jena.update.UpdateProcessor;
import com.hp.hpl.jena.update.UpdateRequest;

import de.fuberlin.wiwiss.ng4j.NamedGraph;
import de.fuberlin.wiwiss.ng4j.NamedGraphSet;
import de.fuberlin.wiwiss.ng4j.Quad;
import de.fuberlin.wiwiss.ng4j.impl.NamedGraphSetImpl;

public class QueryCentral {

	private Ontology action; 
	private String tdbSource = "TDB/atdb";
	private String owlSource;
	
	public QueryCentral(){
		this.tdbSource = "TDB/atdb";
	}
	
	public QueryCentral(Ontology action){
		this.action = action;
	}
	
	public QueryCentral(String source, Ontology ont){
		this.tdbSource = source;
		this.action = ont;
	}
	
	public Ontology getAction() {
		return action;
	}



	public void setAction(Ontology action) {
		this.action = action;
	}



	public String getTdbSource() {
		return tdbSource;
	}



	public void setTdbSource(String tdbSource) {
		this.tdbSource = tdbSource;
	}

	public void setOwlSource(String prefix){
		this.owlSource = prefix;
	}
	//Query Parsers
	
	/**
	 * TODO deberia regresar los resultados de al consulta
	 * determinar si posee WHEN, checkear si esta bien formada la consulta y pasarla a ejecucion
	 * @param query
	 * @return boolean si la ejecucion se dio bien o no 
	 */
	public boolean execute(String query){

		boolean executable= false,stat = false;
		String when_regex ="WHEN";
		String divisions[] = query.trim().split(when_regex);
		String main_query = divisions[0];
		String update_s;
		if(divisions.length>1){//si hay WHEN
			divisions[1]=divisions[1].replaceAll("\\s","");//reemplazar espacios
			//parse brackets and check them
			//System.out.println(Arrays.toString(divisions[1].split("[\\{\\}]")));
			//separamos por llaves
			executable = this.process_events(Arrays.asList(divisions[1].split("[\\{\\}]")));
		}
		
		if(executable){
			action.updateChanges(this.owlSource);
			update_s = action.getUpdate_q();
			System.out.println(update_s);
			this.updateMethod(update_s);
			//make the update query
		}
		
		stat = this.queryStatic(main_query);
		
		return executable && stat;
	}
	 
	
	/***
	  * Esta funcion se encarga de crear los objetivos de activación
	  * @param strings
	  * @return
	  */
	 private boolean process_events(List<String> strings){
		 
		 int status=0;
		 
		 for(String s:strings){//por cada cadena 
			 //System.out.println(s);
			 if(s.toUpperCase().equals("SERIAL")){//Eventos en Serial
				 //System.out.println("En serial vale");
				 status=1;	 
			 }else if(s.toUpperCase().equals("PARALLEL")){//Eventos en Paralelo
				//System.out.println("en paralelo vale");
				 status = 2;
			 }else{
				 if(s.equals("")){//si es un espacio en blanco que no afecta
					// System.out.println("espacio en blanco");
					 continue;
				 }else{
					 if(analizeNstack(Arrays.asList(s.split("\\],")),status)){
						// System.out.println("evento: "+s);
					 }else{
						 return false;
					 }
				 }
				
				 
			 }

		 }
		 return true;
	}
	 
	/**
	 * 
	 * @param ecp Lista que posee los eventos, la clase y la propiedad sobre los cuales se ejecutaran los eventos
	 * @param modo Determinar se se ejecuta en serial o paralelo
	 * @return true si todo salio bien false en caso contrario
	 */
	private boolean analizeNstack(List<String> ecp, int modo){
		if(ecp.size()!=2){//sino posee eventos por un lado y por el otro clase y prop esta mal formada
			return false;
		}else{
			String e = ecp.get(0);//la lista de eventos
			String cp = ecp.get(1);//la clase y la propiedad
			String clase="", propiedad="";
			if(cp.split(",").length!=2){//si no hay exactamente una clase y una propiedad
				return false;
			}else{
				clase = cp.split(",")[0];propiedad = cp.split(",")[1];
				e = e.replaceAll("\\[", "");
				/*System.out.println("clase: "+clase);
				System.out.println("prop: "+propiedad);
				System.out.println("eventos: "+e);*/
				for(String s:e.split(",")){
					System.out.println("tripleta: "+s+" "+clase+" "+propiedad);
					if(!action.mioatize_objective(s)){//organización de los objetivos, vease eventos
						return false;
					}
				}
			}
			//Ejecutar los eventos y pasar al siguiente Paralel o Serial
			action.mioatize_execution(clase,propiedad,modo);		
		}
		//Limpiar y seguir.
		
		return true;
	}
	
	
	//Sparql 1.1 query method
	/**
	 * Esta parte se encarga de hacer al consulta estatica
	 * @param queryString
	 * @return
	 */
	private boolean queryStatic(String queryString){
		
		
		//manera 3
		Location location = new Location(this.tdbSource);
		Dataset dataset = TDBFactory.createDataset(location);
		 dataset.begin(ReadWrite.READ) ;

		 try {
		     QueryExecution qExec = QueryExecutionFactory.create(queryString, dataset) ;
		     ResultSet rs = qExec.execSelect() ;
		     try {
		         ResultSetFormatter.out(rs) ;
		     } finally { qExec.close() ; }

		 } finally { dataset.end() ; }
		return true;
	}
	
	//Sparql 1.1 Update methods
	/**
	 * Esta metodo ejecuta la consulta de actualizacion del repositorio
	 * @param sparqlUpdateString
	 */
	public void updateMethod(String sparqlUpdateString){
		 Location location = new Location(this.tdbSource);
		 Dataset dataset = TDBFactory.createDataset(location);
		 dataset.begin(ReadWrite.WRITE) ;
		 
		 try{
		
			// ... perform a SPARQL Update
			  GraphStore graphStore = GraphStoreFactory.create(dataset) ;
			  /*sparqlUpdateString = StrUtils.strjoinNL(
			          "PREFIX e: <http://example/>",
			          "INSERT DATA { e:sujeto e:predicado \"objeto\" .",
			          "e:otra e:perfecta \"tripleta\" }"
			          ) ;*/

			 UpdateRequest request = UpdateFactory.create(sparqlUpdateString) ;
			 //System.out.println(request.toString());
			 UpdateProcessor proc = UpdateExecutionFactory.create(request, graphStore) ;
			 //System.out.println(proc.getGraphStore());
		     proc.execute() ;
		     
		     // Finally, commit the transaction.
		     dataset.commit() ;
		     // Or call .abort()
		 }finally{
			 dataset.end();
		 }

	}
/**
 * 	Metodo para vaciar el TDB
 */
	public void clearOnt(){
		Location location = new Location(this.tdbSource);
		 Dataset dataset = TDBFactory.createDataset(location);
		 dataset.begin(ReadWrite.WRITE) ;
		 
		 try{
		
			// ... perform a SPARQL Update
			  GraphStore graphStore = GraphStoreFactory.create(dataset) ;
			  String sparqlUpdateString = "CLEAR DEFAULT";

			 UpdateRequest request = UpdateFactory.create(sparqlUpdateString) ;
			 //System.out.println(request.toString());
			 UpdateProcessor proc = UpdateExecutionFactory.create(request, graphStore) ;
			 //System.out.println(proc.getGraphStore());
		     proc.execute() ;
		     this.getAction().clean();
		     // Finally, commit the transaction.
		     dataset.commit() ;
		     // Or call .abort()
		 }finally{
			 dataset.end();
		 }
	
	}
/**
 * Metodo para cargar el documento owl al TDB
 * @param fl
 */
	public void loadDoc(String fl){
		Location location = new Location(this.tdbSource);
		 Dataset dataset = TDBFactory.createDataset(location);
		 dataset.begin(ReadWrite.WRITE) ;
		 
		 try{
		
			// ... perform a SPARQL Update
			  GraphStore graphStore = GraphStoreFactory.create(dataset) ;
			  String sparqlUpdateString = "LOAD <"+fl+">";

			 UpdateRequest request = UpdateFactory.create(sparqlUpdateString) ;
			 //System.out.println(request.toString());
			 UpdateProcessor proc = UpdateExecutionFactory.create(request, graphStore) ;
			 //System.out.println(proc.getGraphStore());
		     proc.execute() ;
		     
		     // Finally, commit the transaction.
		     dataset.commit() ;
		     // Or call .abort()
		 }finally{
			 dataset.end();
		 }
	}
	
	public void save() {
		try {
			FileOutputStream fileOut = new FileOutputStream("serialization/ontology.ser");
			ObjectOutputStream out = new ObjectOutputStream(fileOut);
			out.writeObject(this.action);
			out.close();
			fileOut.close();
			System.out.println("Serialized data is saved in serialization/ontology.ser");
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public void load(){
		try
	      {
	         FileInputStream fileIn = new FileInputStream("serialization/ontology.ser");
	         ObjectInputStream in = new ObjectInputStream(fileIn);
	         this.action = new Ontology();
	         this.action = (Ontology) in.readObject();
	         in.close();
	         fileIn.close();
	         this.action.reload();
	         //new File("serialization/ontology.ser").delete();
	      }catch(IOException i)
	      {
	         i.printStackTrace();
	      }catch(ClassNotFoundException c)
	      {
	         System.out.println("Ontology class not found");
	         c.printStackTrace();
	      }
	}
	
}
