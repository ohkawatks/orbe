
/**
* LCDHolder.java .
* IDL-to-Java コンパイラ (ポータブル), バージョン "3.1" で生成
* 生成元: LCD.idl
* 2008年7月31日 17時07分34秒 JST
*/

public final class LCDHolder implements org.omg.CORBA.portable.Streamable
{
  public LCD value = null;

  public LCDHolder ()
  {
  }

  public LCDHolder (LCD initialValue)
  {
    value = initialValue;
  }

  public void _read (org.omg.CORBA.portable.InputStream i)
  {
    value = LCDHelper.read (i);
  }

  public void _write (org.omg.CORBA.portable.OutputStream o)
  {
    LCDHelper.write (o, value);
  }

  public org.omg.CORBA.TypeCode _type ()
  {
    return LCDHelper.type ();
  }

}
