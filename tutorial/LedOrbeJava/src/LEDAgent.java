import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;

import jp.go.aist.utils.IORgenerator;
import jp.go.aist.utils.TCPtoUART;
import jp.go.aist.utils.TCPtoUDP;
import jp.go.aist.utils.TransportBridge;


public class LEDAgent {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		System.out.println("LED Agnet + Bridge is starting...");
		TransportBridge bridge = null;
		try {
			String ior = "";
			if(true) {
			    ior = new IORgenerator("LED", "127.0.0.1", "5757").getIORString();
			    BufferedWriter bw = new BufferedWriter(new FileWriter("ior"));
			    bw.write(ior, 0, ior.length());
			    bw.flush();
			    bw.close();
			    System.out.println(ior);//TODO: output to file "ior"
			    
			    if(true){
					bridge = new TCPtoUDP(5757, "192.168.1.59", 5757);
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
			
			while(true) {
				Thread.sleep(100);
			}
		} catch (Exception e) {
		    System.out.println("ERROR : " + e) ;
		    e.printStackTrace(System.out);
		} finally {
			bridge.stop();
		}
	}

}
