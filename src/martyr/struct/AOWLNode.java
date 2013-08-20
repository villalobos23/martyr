package martyr.struct;

import java.io.Serializable;

public abstract class AOWLNode implements Serializable{
	/**
	 * 
	 */
	private static final long serialVersionUID = -1340199814540214595L;
	private String ID;
	
	public AOWLNode() {
		// TODO Auto-generated constructor stub
		ID = "";
	}
	
	public String getID() {
		return ID;
	}
	public void setID(String iD) {
		ID = iD;
	}
}
