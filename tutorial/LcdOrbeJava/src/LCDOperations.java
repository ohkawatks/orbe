
/**
* LCDOperations.java .
* IDL-to-Java �R���p�C�� (�|�[�^�u��), �o�[�W���� "3.1" �Ő���
* ������: LCD.idl
* 2008�N7��31�� 17��07��34�b JST
*/

public interface LCDOperations 
{
  void execute (byte command);
  void writeData (byte data);
  byte readData ();
} // interface LCDOperations
