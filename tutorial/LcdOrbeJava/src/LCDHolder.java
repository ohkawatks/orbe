
/**
* LCDHolder.java .
* IDL-to-Java �R���p�C�� (�|�[�^�u��), �o�[�W���� "3.1" �Ő���
* ������: LCD.idl
* 2008�N7��31�� 17��07��34�b JST
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
