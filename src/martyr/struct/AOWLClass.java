package martyr.struct;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

public class AOWLClass extends AOWLNode implements Serializable{
	 /**
	 * 
	 */
	private static final long serialVersionUID = -2373800992209750065L;
	List<String> Subclases;
	 public AOWLClass() {
		// TODO Auto-generated constructor stub
	}
	 
	public AOWLClass(String id, List<String> subs){
		this.setID(new String(id));
		this.Subclases = new ArrayList<String>();
		if(!subs.isEmpty()){
			this.Subclases.addAll(subs);
		}
	}
	 
	public void setSubclases(List<String> subclases) {
		Subclases = subclases;
	}
	
	 public List<String> getSubclases() {
		return Subclases;
	}

}
