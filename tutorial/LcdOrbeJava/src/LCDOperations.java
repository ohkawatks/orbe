
/**
* LCDOperations.java .
* IDL-to-Java コンパイラ (ポータブル), バージョン "3.1" で生成
* 生成元: LCD.idl
* 2008年7月31日 17時07分34秒 JST
*/

public interface LCDOperations 
{
  void execute (byte command);
  void writeData (byte data);
  byte readData ();
} // interface LCDOperations
