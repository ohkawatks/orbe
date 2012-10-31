import java.applet.Applet;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.io.BufferedWriter;
import java.io.FileWriter;

import org.omg.CORBA.ORB;
import org.omg.PortableServer.POA;
import org.omg.PortableServer.POAHelper;
import org.omg.PortableServer.POAManager;


public class LEDApplet extends Applet {

	/**
	 * 
	 */
	private static final long serialVersionUID = 8895935158120036991L;

    int width = 16;
    int height = 16;
    Dimension windowSize = new Dimension((LEDImpl.NUMBER_OF_LEDS+1)*32, 32+16);

    LED led;
    ORB orb;
    public LEDApplet() {
		String IORfilename = "../ior";
		try {
		    // Initializing ORB/POA and server object
		    orb = ORB.init(new String[]{}, null);
		    org.omg.CORBA.Object rootpoa =
			orb.resolve_initial_references("RootPOA");
		    POA poa = POAHelper.narrow(rootpoa);
		    LEDImpl obj = new LEDImpl();
		    led = LEDHelper.narrow(poa.servant_to_reference(obj));
		    obj.addCallBackApplet(this);
		    obj.turnOn(2);
		    
		    // Writing stringified IOR to file specified by IORfilename
		    String ior = orb.object_to_string(obj._this());
		    BufferedWriter bw = new BufferedWriter(new FileWriter(IORfilename));
		    bw.write(ior, 0, ior.length());
		    bw.flush();
		    bw.close();

		    // Server is now ready
		    POAManager poamgr = poa.the_POAManager();
		    poamgr.activate();
		    System.out.println("Server is now ready.\n") ;
		    
		    new Thread(){
		    	public void run() {
		    		orb.run();
		    	}
		    }.start();
		}
		catch (Exception e) {
		    System.out.println("ERROR : " + e) ;
		    e.printStackTrace(System.out);
		}
    }

    @Override
	public void init() {
    	this.setSize(windowSize);
    	this.setBackground(Color.DARK_GRAY);
		super.init();
	}

	public void paint(Graphics g){
//        g.setColor(Color.DARK_GRAY);
//        g.fillRect(0, 0, windowSize.width - 1, windowSize.height - 1);
     
    	g.setColor(Color.GREEN);
        for(int i=0; i<LEDImpl.NUMBER_OF_LEDS; i++) {
        	int x = i*32 + 16;
        	int y = 16;
        	
        	if (led.getState(i)) {
        		g.fillOval(x, y, width, height);
        	} else {
        		g.drawOval(x, y, width, height);
        	}
        	
        }
    }


}
