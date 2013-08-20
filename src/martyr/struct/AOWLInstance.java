package martyr.struct;

import java.util.HashMap;
import java.util.Map;

public class AOWLInstance extends AOWLNode {
	/**
	 * 
	 */
	private static final long serialVersionUID = -2707243840206381919L;
	private String clase;
	private Map<String,String> propiedades;
	
	public AOWLInstance() {
		// TODO Auto-generated constructor stub
		super();
		this.clase = new String();
		this.propiedades = new HashMap<String, String>();
	}

	public AOWLInstance(String nombre, String clase){
		this.setID(nombre); 
		this.clase = new String(clase);
		this.propiedades = new HashMap<String, String>();
		 
	}
	
	
	//Setters y getters
	public String getClase() {
		return clase;
	}

	public void setClase(String clase) {
		this.clase = clase;
	}

	public Map<String, String> getPropiedades() {
		return propiedades;
	}

	public void setPropiedades(Map<String, String> propiedades) {
		this.propiedades = propiedades;
	}
	
	//agregar y regresar propiedades
	public void addPropiedad(String propiedad, String valor){
		this.propiedades.put(propiedad, valor);
	}
	
	public String getValue(String propiedad){
		return this.propiedades.get(propiedad);
	}
}
