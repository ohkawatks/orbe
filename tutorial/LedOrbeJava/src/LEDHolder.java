
/**
* LEDHolder.java .
* IDL-to-Java �R���p�C�� (�|�[�^�u��), �o�[�W���� "3.1" �Ő���
* ������: LED.idl
* 2008�N7��31�� 15��45��29�b JST
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
