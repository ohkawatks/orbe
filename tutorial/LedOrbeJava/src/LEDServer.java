
import java.io.BufferedWriter;
import java.io.FileWriter;

import org.omg.CORBA.ORB;
import org.omg.PortableServer.POA;
import org.omg.PortableServer.POAHelper;
import org.omg.PortableServer.POAManager;

public class LEDServer {
	public static void main(String args[])
	    {
		String IORfilename = "ior";
		try {
		    // Initializing ORB/POA and server object
		    ORB orb = ORB.init(args, null);
		    org.omg.CORBA.Object rootpoa =
			orb.resolve_initial_references("RootPOA");
		    POA poa = POAHelper.narrow(rootpoa);
		    LEDImpl obj = new LEDImpl();
		    LED nano = LEDHelper.narrow(poa.servant_to_reference(obj));
		    
		    // Writing stringified IOR to file specified by IORfilename
		    String ior = orb.object_to_string(obj._this());
		    BufferedWriter bw = new BufferedWriter(new FileWriter(IORfilename));
		    bw.write(ior, 0, ior.length());
		    bw.close();

		    // Server is now ready
		    POAManager poamgr = poa.the_POAManager();
		    poamgr.activate();
		    System.out.println("Server is now ready.\n") ;
		    orb.run();
		}
		catch (Exception e) {
		    System.out.println("ERROR : " + e) ;
		    e.printStackTrace(System.out);
		}
	    }   

}
