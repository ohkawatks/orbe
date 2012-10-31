
/**
* LCDPOA.java .
* IDL-to-Java コンパイラ (ポータブル), バージョン "3.1" で生成
* 生成元: LCD.idl
* 2008年7月31日 17時07分34秒 JST
*/

public abstract class LCDPOA extends org.omg.PortableServer.Servant
 implements LCDOperations, org.omg.CORBA.portable.InvokeHandler
{

  // Constructors

  private static java.util.Hashtable _methods = new java.util.Hashtable ();
  static
  {
    _methods.put ("execute", new java.lang.Integer (0));
    _methods.put ("writeData", new java.lang.Integer (1));
    _methods.put ("readData", new java.lang.Integer (2));
  }

  public org.omg.CORBA.portable.OutputStream _invoke (String $method,
                                org.omg.CORBA.portable.InputStream in,
                                org.omg.CORBA.portable.ResponseHandler $rh)
  {
    org.omg.CORBA.portable.OutputStream out = null;
    java.lang.Integer __method = (java.lang.Integer)_methods.get ($method);
    if (__method == null)
      throw new org.omg.CORBA.BAD_OPERATION (0, org.omg.CORBA.CompletionStatus.COMPLETED_MAYBE);

    switch (__method.intValue ())
    {
       case 0:  // LCD/execute
       {
         byte command = in.read_octet ();
         this.execute (command);
         out = $rh.createReply();
         break;
       }

       case 1:  // LCD/writeData
       {
         byte data = in.read_octet ();
         this.writeData (data);
         out = $rh.createReply();
         break;
       }

       case 2:  // LCD/readData
       {
         byte $result = (byte)0;
         $result = this.readData ();
         out = $rh.createReply();
         out.write_octet ($result);
         break;
       }

       default:
         throw new org.omg.CORBA.BAD_OPERATION (0, org.omg.CORBA.CompletionStatus.COMPLETED_MAYBE);
    }

    return out;
  } // _invoke

  // Type-specific CORBA::Object operations
  private static String[] __ids = {
    "IDL:LCD:1.0"};

  public String[] _all_interfaces (org.omg.PortableServer.POA poa, byte[] objectId)
  {
    return (String[])__ids.clone ();
  }

  public LCD _this() 
  {
    return LCDHelper.narrow(
    super._this_object());
  }

  public LCD _this(org.omg.CORBA.ORB orb) 
  {
    return LCDHelper.narrow(
    super._this_object(orb));
  }


} // class LCDPOA
