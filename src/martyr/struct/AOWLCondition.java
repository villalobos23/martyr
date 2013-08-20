package martyr.struct;

import java.util.ArrayList;
import java.util.List;

public class AOWLCondition extends AOWLNode {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private String refEvent;
	private List<AOWLComplex> complex;
	private String reactiveValue;

	public AOWLCondition() {
		// TODO Auto-generated constructor stub
		super();
	}
	
	public AOWLCondition(String ev, List<AOWLComplex> cmplx, String val){
		this.refEvent = new String(ev);
		this.complex = new ArrayList<AOWLComplex>();
		this.complex.addAll(cmplx);
		this.reactiveValue = new String(val);
		this.setID("DummyID");
	}

	public String getRefEvent() {
		return refEvent;
	}

	public void setRefEvent(String refEvent) {
		this.refEvent = refEvent;
	}

	public List<AOWLComplex> getComplex() {
		return complex;
	}

	public void setComplex(List<AOWLComplex> complex) {
		this.complex = complex;
	}

	public String getReactiveValue() {
		return reactiveValue;
	}

	public void setReactiveValue(String reactiveValue) {
		this.reactiveValue = reactiveValue;
	}
	
	
}


