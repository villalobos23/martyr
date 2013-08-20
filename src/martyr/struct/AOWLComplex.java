package martyr.struct;

public class AOWLComplex extends AOWLNode {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private String refProp;
	private String symbol;
	private String booleanCond;
	
	public AOWLComplex() {
		// TODO Auto-generated constructor stub
		super();
	}
	
	public AOWLComplex(String prop, String symb, String cond){
		this.refProp=new String(prop);
		this.symbol= new String(symb);
		this.booleanCond = new String(cond);
		this.setID("DummyID");
	}

	public String getRefProp() {
		return refProp;
	}

	public void setRefProp(String refProp) {
		this.refProp = refProp;
	}

	public String getSymbol() {
		return symbol;
	}

	public void setSymbol(String symbol) {
		this.symbol = symbol;
	}

	public String getBooleanCond() {
		return booleanCond;
	}

	public void setBooleanCond(String booleanCond) {
		this.booleanCond = booleanCond;
	}
	
	
}
