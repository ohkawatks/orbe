
/**
* LEDPOA.java .
* IDL-to-Java コンパイラ (ポータブル), バージョン "3.1" で生成
* 生成元: LED.idl
* 2008年7月31日 15時45分29秒 JST
*/

public abstract class LEDPOA extends org.omg.PortableServer.Servant
 implements LEDOperations, org.omg.CORBA.portable.InvokeHandler
{

  // Constructors

  private static java.util.Hashtable _methods = new java.util.Hashtable ();
  static
  {
    _methods.put ("turnOn", new java.lang.Integer (0));
    _methods.put ("turnOff", new java.lang.Integer (1));
    _methods.put ("getState", new java.lang.Integer (2));
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
       case 0:  // LED/turnOn
       {
         int ID = in.read_long ();
         this.turnOn (ID);
         out = $rh.createReply();
         break;
       }

       case 1:  // LED/turnOff
       {
         int ID = in.read_long ();
         this.turnOff (ID);
         out = $rh.createReply();
         break;
       }

       case 2:  // LED/getState
       {
         int ID = in.read_long ();
         boolean $result = false;
         $result = this.getState (ID);
         out = $rh.createReply();
         out.write_boolean ($result);
         break;
       }

       default:
         throw new org.omg.CORBA.BAD_OPERATION (0, org.omg.CORBA.CompletionStatus.COMPLETED_MAYBE);
    }

    return out;
  } // _invoke

  // Type-specific CORBA::Object operations
  private static String[] __ids = {
    "IDL:LED:1.0"};

  public String[] _all_interfaces (org.omg.PortableServer.POA poa, byte[] objectId)
  {
    return (String[])__ids.clone ();
  }

  public LED _this() 
  {
    return LEDHelper.narrow(
    super._this_object());
  }

  public LED _this(org.omg.CORBA.ORB orb) 
  {
    return LEDHelper.narrow(
    super._this_object(orb));
  }


} // class LEDPOA
