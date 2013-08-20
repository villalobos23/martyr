package martyr.struct;

public class AOWLDTProp extends AOWLNode {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = -8507043526404409527L;
	private String domain;
	/**
	 * Esta propiedad determina el tipo de propiedad
	 */
	private int type;
	public static final int INT_PROP = 0;
	public static final int DOUBLE_PROP = 2;
	public static final int STRING_PROP = 1;
	
	public AOWLDTProp() {
		// TODO Auto-generated constructor stub
	}
	
	public AOWLDTProp(String id, String domain,int type) {
		// TODO Auto-generated constructor stub
		this.setID(id);
		this.domain = new String(domain);
		this.type = type;
	}

	public String getDomain() {
		return domain;
	}

	public int getType() {
		return type;
	}
	
	
	
}
