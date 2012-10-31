/*
 * $Id: IORgenerator.java 793 2008-09-17 02:09:28Z takaya $
 * Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology
 *
 */

package jp.go.aist.utils;

import java.io.BufferedWriter;
import java.io.FileWriter;

public class IORgenerator {
	
	private String IOR;
	private String pad = "00000000";
	
	public String fillHeadZeroString(String src, int length) {
		int fillZero = length - src.length(); // to make long
		src = pad.substring(0, fillZero) + src;
		
		return src;
	}

	public String fillTrailerZeroString(String src, int length) {
		int fillZero = length - (src.length() % length); // to make long
		src += pad.substring(0, fillZero);
		
		return src;
	}

	public String hexInterfaceName(String interfaceName) {
		// Interface名を16進数に変換
		String ap_name = string_to_hex("IDL:" + interfaceName + ":1.0");
		int ap_len = (ap_name.length() / 2) + 1;
		String lengthString = fillHeadZeroString(Integer.toHexString(ap_len), 8);
		String wholeString = fillTrailerZeroString(lengthString + ap_name, 8);
		
		return wholeString;
	}
	
	public String hexIPAddress(String IPaddress) {
		// IP addressを16進数に変換
		String address = string_to_hex(IPaddress);
		int addr_len = (address.length() / 2) + 1;
			
		address = fillHeadZeroString(Integer.toHexString(addr_len), 2) + address;
		
		return address;
	}
	
	public String hexPortNumber(String portNumber, int tmp_addr_len) {
		// port番号付加
		String port = fillHeadZeroString(Integer.toHexString(new Integer(portNumber).intValue()), 8);

		switch (tmp_addr_len) {
		case 0:
		case 1:
			port = port.substring(tmp_addr_len * 2);
			break;
		case 2:
			port = port + pad.substring(2 * 2);
			break;
		case 3:
			port = port.substring(2);
			port = port + pad.substring(2 * 2);
			break;
		}

		return port;
	}
	
	public IORgenerator(String interfaceName, String IPaddress, String port) {
		
		String iorHeader = "IOR:00000000";
		String iorInterfaceName = hexInterfaceName(interfaceName);
		String iorStartCharSequence = "00000001" + "00000000";
		String iorIPAddress = hexIPAddress(IPaddress);
	
		int addr_len = (iorIPAddress.length() / 2);
		int tmp_addr_len = (addr_len - 1) % 4;
		
		String iorPortNumber = hexPortNumber(port, tmp_addr_len);
		String iorVersionString = "00010200" + "000000";
		String iorTrailer = "00000031" + 
		"afabcb0000000020af9478d600000001000000000000000100000008526f6f74504f410000000008" +
		"00000001000000001400000000000002000000010000002000000000000100010000000205010001" + 
		"0001002000010109000000010001010000000026000000020002";

		String iorCharSequence = iorVersionString + iorIPAddress +  iorPortNumber + iorTrailer;
		String iorCharSequenceLength = fillHeadZeroString(Integer.toHexString(iorCharSequence.length() / 2), 8);

		IOR = iorHeader + iorInterfaceName + 
			iorStartCharSequence + iorCharSequenceLength + iorCharSequence;
	}
	
	public String getIORString() {
		return IOR;
	}

	public static void main(String[] args) {
		if (args.length != 3) {
			System.out.println("Usage: IORgenerator I/Fname IPaddress port");
			System.exit(1);
		}

		IORgenerator iorGenerator = new IORgenerator(args[0], args[1], args[2]);
		
		try {
			BufferedWriter bw = new BufferedWriter(new FileWriter("ior"));
			bw.write(iorGenerator.getIORString());
			bw.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	// 文字列を16進数表記に変換
	public String string_to_hex(String arg) {
		String str = "";
		char ch;
		
		for (int i = 0; i < arg.length(); i++) {
			ch = arg.charAt(i);
			str += Integer.toHexString((int)ch);
		}
		
		return str;
	}
}
