
/**
* LCDHelper.java .
* IDL-to-Java コンパイラ (ポータブル), バージョン "3.1" で生成
* 生成元: LCD.idl
* 2008年7月31日 17時07分34秒 JST
*/

abstract public class LCDHelper
{
  private static String  _id = "IDL:LCD:1.0";

  public static void insert (org.omg.CORBA.Any a, LCD that)
  {
    org.omg.CORBA.portable.OutputStream out = a.create_output_stream ();
    a.type (type ());
    write (out, that);
    a.read_value (out.create_input_stream (), type ());
  }

  public static LCD extract (org.omg.CORBA.Any a)
  {
    return read (a.create_input_stream ());
  }

  private static org.omg.CORBA.TypeCode __typeCode = null;
  synchronized public static org.omg.CORBA.TypeCode type ()
  {
    if (__typeCode == null)
    {
      __typeCode = org.omg.CORBA.ORB.init ().create_interface_tc (LCDHelper.id (), "LCD");
    }
    return __typeCode;
  }

  public static String id ()
  {
    return _id;
  }

  public static LCD read (org.omg.CORBA.portable.InputStream istream)
  {
    return narrow (istream.read_Object (_LCDStub.class));
  }

  public static void write (org.omg.CORBA.portable.OutputStream ostream, LCD value)
  {
    ostream.write_Object ((org.omg.CORBA.Object) value);
  }

  public static LCD narrow (org.omg.CORBA.Object obj)
  {
    if (obj == null)
      return null;
    else if (obj instanceof LCD)
      return (LCD)obj;
    else if (!obj._is_a (id ()))
      throw new org.omg.CORBA.BAD_PARAM ();
    else
    {
      org.omg.CORBA.portable.Delegate delegate = ((org.omg.CORBA.portable.ObjectImpl)obj)._get_delegate ();
      _LCDStub stub = new _LCDStub ();
      stub._set_delegate(delegate);
      return stub;
    }
  }

  public static LCD unchecked_narrow (org.omg.CORBA.Object obj)
  {
    if (obj == null)
      return null;
    else if (obj instanceof LCD)
      return (LCD)obj;
    else
    {
      org.omg.CORBA.portable.Delegate delegate = ((org.omg.CORBA.portable.ObjectImpl)obj)._get_delegate ();
      _LCDStub stub = new _LCDStub ();
      stub._set_delegate(delegate);
      return stub;
    }
  }

}
