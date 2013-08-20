package martyr.struct;

import java.util.ArrayList;
import java.util.List;

public class AOWLEvent extends AOWLNode {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1985878453687543077L;
	private List<String> subeventos;//es esto realmente una lista de subeventos?
	
	public AOWLEvent() {
		// TODO Auto-generated constructor stub
	}
	
	public AOWLEvent(String id, List<String> subs){
		this.setID(new String(id));
		this.subeventos= new ArrayList<String>();
		if(!subs.isEmpty())
			this.subeventos.addAll(subs);
	}
	
	public void setSubeventos(List<String> subeventos) {
		this.subeventos = subeventos;
	}
	
	public List<String> getSubeventos() {
		return subeventos;
	}
	
}
