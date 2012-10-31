
/**
* LEDHelper.java .
* IDL-to-Java コンパイラ (ポータブル), バージョン "3.1" で生成
* 生成元: LED.idl
* 2008年7月31日 15時45分29秒 JST
*/

abstract public class LEDHelper
{
  private static String  _id = "IDL:LED:1.0";

  public static void insert (org.omg.CORBA.Any a, LED that)
  {
    org.omg.CORBA.portable.OutputStream out = a.create_output_stream ();
    a.type (type ());
    write (out, that);
    a.read_value (out.create_input_stream (), type ());
  }

  public static LED extract (org.omg.CORBA.Any a)
  {
    return read (a.create_input_stream ());
  }

  private static org.omg.CORBA.TypeCode __typeCode = null;
  synchronized public static org.omg.CORBA.TypeCode type ()
  {
    if (__typeCode == null)
    {
      __typeCode = org.omg.CORBA.ORB.init ().create_interface_tc (LEDHelper.id (), "LED");
    }
    return __typeCode;
  }

  public static String id ()
  {
    return _id;
  }

  public static LED read (org.omg.CORBA.portable.InputStream istream)
  {
    return narrow (istream.read_Object (_LEDStub.class));
  }

  public static void write (org.omg.CORBA.portable.OutputStream ostream, LED value)
  {
    ostream.write_Object ((org.omg.CORBA.Object) value);
  }

  public static LED narrow (org.omg.CORBA.Object obj)
  {
    if (obj == null)
      return null;
    else if (obj instanceof LED)
      return (LED)obj;
    else if (!obj._is_a (id ()))
      throw new org.omg.CORBA.BAD_PARAM ();
    else
    {
      org.omg.CORBA.portable.Delegate delegate = ((org.omg.CORBA.portable.ObjectImpl)obj)._get_delegate ();
      _LEDStub stub = new _LEDStub ();
      stub._set_delegate(delegate);
      return stub;
    }
  }

  public static LED unchecked_narrow (org.omg.CORBA.Object obj)
  {
    if (obj == null)
      return null;
    else if (obj instanceof LED)
      return (LED)obj;
    else
    {
      org.omg.CORBA.portable.Delegate delegate = ((org.omg.CORBA.portable.ObjectImpl)obj)._get_delegate ();
      _LEDStub stub = new _LEDStub ();
      stub._set_delegate(delegate);
      return stub;
    }
  }

}
