
import java.io.BufferedReader;
import java.io.FileReader;

import jp.go.aist.utils.IORgenerator;
import jp.go.aist.utils.TCPtoUART;
import jp.go.aist.utils.TCPtoUDP;
import jp.go.aist.utils.TransportBridge;

import org.omg.CORBA.ORB;

public class LEDClient {

	static LED led;
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		TransportBridge bridge = null;
		try {
			String ior = "";
			if(true) {
			    ior = new IORgenerator("LED", "127.0.0.1", "5656").getIORString();
			    System.out.println(ior);
			    
			    if(true){
					bridge = new TCPtoUDP(5656, "192.168.1.55", 5656);
					bridge.start();
			    } else {
					bridge = new TCPtoUART("COM1", "5656");
					bridge.start();
					
					System.out.println("Waiting for opening COM Port for 3 seconds.");
					Thread.sleep(3000);
					System.out.println("Now start.");
			    }
			} else {
			    BufferedReader br = new BufferedReader(new FileReader("bin/ior"));
			    ior = br.readLine();
			    br.close();
			}
				

	    
		    ORB orb = ORB.init(args, null);
		    led = LEDHelper.narrow(orb.string_to_object(ior));
	
		    DisplayState();

		    System.out.println("led.turnOn(1)");
		    led.turnOn(1);

		    DisplayState();
		    
		    System.out.println("led.turnOn(2)");
		    led.turnOn(2);
		    
		    DisplayState();
		    
		    int wait = 50;
		    while(true) {
			    for(int i=0; i<8; i++) {
			    	led.turnOn(i);
				    Thread.sleep(wait);
			    }
			    for(int i=0; i<8; i++) {
			    	led.turnOff(i);
				    Thread.sleep(wait);
			    }
		    }
		} catch (Exception e) {
		    System.out.println("ERROR : " + e) ;
		    e.printStackTrace(System.out);
		} finally {
			bridge.stop();
		}
	}
	
	private static void DisplayState() {
	    for(int i=0; i<=2; i++) {
	    	System.out.println("led.getState("+i+")=" + led.getState(i));
	    }
	
	}

}
