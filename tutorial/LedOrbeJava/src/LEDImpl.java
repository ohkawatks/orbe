import java.applet.Applet;


public class LEDImpl extends LEDPOA {
	public static final int NUMBER_OF_LEDS = 8;
	boolean state[] = new boolean[NUMBER_OF_LEDS];
	
	@Override
	public void turnOff(int ID) {
		state[ID]=false;
		updateApplet();
	}

	@Override
	public void turnOn(int ID) {
		state[ID]=true;
		updateApplet();
	}

	@Override
	public boolean getState(int ID) {
		return state[ID];
	}
	
	private Applet callbackApplet = null;
	public void addCallBackApplet(Applet applet) {
		callbackApplet = applet;
	}
	public void updateApplet() {
		if (callbackApplet!=null) {
			callbackApplet.repaint();
		}
	}

}
