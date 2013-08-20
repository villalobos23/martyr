package martyr.struct;


public class AOWLObjProp extends AOWLDTProp {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1883703737749979483L;
	private String range;
	public AOWLObjProp() {
		// TODO Auto-generated constructor stub
	}
	public AOWLObjProp(String id ,String domain, int type,String range) {
		// TODO Auto-generated constructor stub
		super(id,domain,type);
		this.range = range;
	}
	public String getRange() {
		return range;
	}
	public void setRange(String range) {
		this.range = range;
	}
		
}
