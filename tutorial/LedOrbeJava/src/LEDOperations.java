
/**
* LEDOperations.java .
* IDL-to-Java �R���p�C�� (�|�[�^�u��), �o�[�W���� "3.1" �Ő���
* ������: LED.idl
* 2008�N7��31�� 15��45��29�b JST
*/

public interface LEDOperations 
{
  void turnOn (int ID);
  void turnOff (int ID);
  boolean getState (int ID);
} // interface LEDOperations
