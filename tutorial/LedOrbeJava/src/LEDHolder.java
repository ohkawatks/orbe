
/**
* LEDHolder.java .
* IDL-to-Java コンパイラ (ポータブル), バージョン "3.1" で生成
* 生成元: LED.idl
* 2008年7月31日 15時45分29秒 JST
*/

public final class LEDHolder implements org.omg.CORBA.portable.Streamable
{
  public LED value = null;

  public LEDHolder ()
  {
  }

  public LEDHolder (LED initialValue)
  {
    value = initialValue;
  }

  public void _read (org.omg.CORBA.portable.InputStream i)
  {
    value = LEDHelper.read (i);
  }

  public void _write (org.omg.CORBA.portable.OutputStream o)
  {
    LEDHelper.write (o, value);
  }

  public org.omg.CORBA.TypeCode _type ()
  {
    return LEDHelper.type ();
  }

}
