package martyr.struct;

import java.util.ArrayList;
import java.util.List;

public class AOWLActProp extends AOWLNode {
	/**
	 * 
	 */
	private static final long serialVersionUID = 4406753123863794119L;
	private String dominio;
	private List<AOWLCondition> condiciones;
	private int type;
	
	public AOWLActProp() {
		// TODO Auto-generated constructor stub
	}
	
	public AOWLActProp(String id, String dominio, List<AOWLCondition> conds,int type)
	{
		this.setID(new String(id));
		this.dominio = new String(dominio);
		this.condiciones = new ArrayList<AOWLCondition>();
		this.type = type;
		condiciones.addAll(conds);
	}

	public String getDominio() {
		return dominio;
	}

	public void setDominio(String dominio) {
		this.dominio = dominio;
	}

	public List<AOWLCondition> getCondiciones() {
		return condiciones;
	}

	public void setCondiciones(List<AOWLCondition> condiciones) {
		this.condiciones = condiciones;
	}

	public int getType() {
		return type;
	}

	public void setType(int type) {
		this.type = type;
	}
	
	
}
