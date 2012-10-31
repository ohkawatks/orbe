
import jp.go.aist.utils.IORgenerator;
import jp.go.aist.utils.TCPtoUART;
import jp.go.aist.utils.TCPtoUDP;
import jp.go.aist.utils.TransportBridge;

import org.omg.CORBA.ORB;

public class LCDClient {
	static LCD lcd;

	public static void init() {
	    lcd.execute((byte)0x28);
	    lcd.execute((byte)0x06);
	    lcd.execute((byte)0x0c);
	    lcd.execute((byte)0x01);
	}
	
	public static void displayString(String msg) {
		int length = msg.length();
		for(int i=0; i<length; i++) {
			lcd.writeData((byte)msg.charAt(i));
		}
		
	}
	
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		TransportBridge bridge = null;
		try {
		    String ior = new IORgenerator("LCD", "127.0.0.1", "5656").getIORString();
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
		    
		    ORB orb = ORB.init(args, null);
		    lcd = LCDHelper.narrow(orb.string_to_object(ior));
	
			System.out.println("Now the message is on the LCD.");
			init();
		    System.out.println("LCD status = " + lcd.readData());
		    String msg1 = "Welcome to ORBE";
		    String msg2 = "AIST/ITRI Japan";
			while(true) {
				for(int i=0; i<15; i++) {
				    lcd.execute((byte)0x80);
					for (int n=0; n<i; n++) displayString("-");
					displayString(msg1);
					
				    lcd.execute((byte)0xc0);
					for (int n=0; n<(16-i); n++) displayString("-");
					displayString(msg2);
					
					Thread.sleep(100);
				}
			}
		}
		catch (Exception e) {
		    System.out.println("ERROR : " + e) ;
		    e.printStackTrace(System.out);
		}
	}
	
	


}
