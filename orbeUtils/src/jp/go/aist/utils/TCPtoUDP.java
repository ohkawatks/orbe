/*
 * $Id: TCPtoUDP.java 909 2008-11-13 10:29:55Z takaya $
 * Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology
 *
 */

package jp.go.aist.utils;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.ServerSocket;

public class TCPtoUDP extends TCPvsUDPBridgeBase {
	int listenPort;
	String targetHost;
	int targetPort;
	private static boolean send_status = false;
	
	public TCPtoUDP(int listenPort, String targetHost, int targetPort) {
		super();
		
		this.listenPort = listenPort;
		this.targetHost = targetHost;
		this.targetPort = targetPort;
	}
	
	public void run() {
		try{
			ServerSocket ss = new ServerSocket(listenPort);
			tcpSocket = ss.accept();
			
			udpDstAddress = InetAddress.getByName(targetHost);
			udpDstPort = targetPort;
			udpSocket = new DatagramSocket();
			udpSrcAddress = udpSocket.getLocalAddress();
			udpSrcPort = udpSocket.getLocalPort();
			
			super.run();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	protected void serverForwardProcedure() throws IOException {
		int length = 0;
		int loop_count;
		int resend_interval = 100;
		InputStream  is = tcpSocket.getInputStream();
		
		while (running) {
			length = is.read(sendBuf);
			if (length<0) {
				throw new IOException("Socket read failed code=" + length);
			}
			if (length==0) continue;
//			System.out.println("F:"+new String(sendBuf, 0, length));
			
			do {
				loop_count = 0;
				
				DatagramPacket sendPacket = new DatagramPacket(sendBuf, length, udpDstAddress, udpDstPort);
				udpSocket.send(sendPacket);
				send_status = true;
				
				do {
					try {
						Thread.sleep(1);
						loop_count++;
					} catch (Exception e) {
						e.printStackTrace();
					}
				} while (send_status && (loop_count < resend_interval));
				
				if (loop_count >= resend_interval)
					System.out.println("Resend");

			} while (send_status);
			
			if (fileout) {
				fileToUDP.write(sendBuf, 0, length);
				fileToUDP.flush();
			}
		}
	};

	protected void serverBackwardProcedure() throws IOException {
		int length;
		OutputStream os = tcpSocket.getOutputStream();
		
		while (running) {
			DatagramPacket recvPacket = new DatagramPacket(recvBuf, BUF_SIZE);
			udpSocket.receive(recvPacket);
			length = recvPacket.getLength();
			if (length<0) {
				throw new IOException("Socket read failed code=" + length);
			}
			if (length==0) continue;
//			System.out.println("B:"+new String(recvBuf, recvPacket.getOffset(), length));
			
			os.write(recvBuf, 0, length);
			os.flush();
			send_status = false;

			if (fileout) {
				fileFromUDP.write(recvBuf, 0, length);
				fileFromUDP.flush();
			}
		}
	};


	/**
	 * @param args
	 */
	public static void main(String[] args) {
		TCPtoUDP bridge = new TCPtoUDP(Integer.parseInt(args[0]), args[1], Integer.parseInt(args[2]));
		bridge.start();
	}


}
