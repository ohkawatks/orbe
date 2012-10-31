
public class LCDImpl extends LCDPOA {

	@Override
	public void execute(byte command) {
		// TODO Auto-generated method stub

	}

	byte reg = 0;
	
	@Override
	public byte readData() {
		// TODO Auto-generated method stub
		return reg;
	}

	@Override
	public void writeData(byte data) {
		reg = data;
	}

}
