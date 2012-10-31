/*
 * $Id: TCPvsUDPBridgeBase.java 793 2008-09-17 02:09:28Z takaya $
 * Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology
 *
 */

package jp.go.aist.utils;

import java.io.FileOutputStream;
import java.io.IOException;
import java.net.DatagramSocket;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.Socket;

public abstract class TCPvsUDPBridgeBase implements TransportBridge, Runnable {
	protected final int BUF_SIZE = 4096;
	protected byte[] sendBuf = new byte[BUF_SIZE];
	protected byte[] recvBuf = new byte[BUF_SIZE];
	protected final boolean debug = true;

//	protected Selector selector;
//	protected SocketChannel tcpSocketChannel;
//	protected SocketChannel udpSocketChannel;

	protected volatile boolean running = true;
    
	protected Socket tcpSocket;
	
	protected DatagramSocket udpSocket;
	protected DatagramPacket udpPacketSend = null;
	protected DatagramPacket udpPacketRecv = new DatagramPacket(sendBuf, BUF_SIZE);
	protected InetAddress udpDstAddress;
	protected InetAddress udpSrcAddress;
	protected int udpDstPort;
	protected int udpSrcPort;
    
	protected boolean fileout = false;
	protected FileOutputStream fileToUDP = null;
	protected FileOutputStream fileFromUDP = null;
	
	protected final String LogToUDP = "ToUDP.bin";
	protected final String LogFromUDP = "FromUDP.bin";
	
//	protected TCPvsUDPBridgeBase bridge;

	protected void serverForwardProcedure() throws IOException {
		throw new IOException("serverForwardProcedure is not defined in this abstrace class.");
	}
	protected void serverBackwardProcedure() throws IOException {
		throw new IOException("serverBackwardProcedure is not defined in this abstrace class.");
	}
	
	public void runForward() {
		while (running) {
			try {
				serverForwardProcedure();
			} catch (IOException e) {
				//e.printStackTrace();
				running = false;
			}
		}

	}

	public void runBackward() {
		while (running) {
			try {
				serverBackwardProcedure();
			} catch (IOException e) {
				//e.printStackTrace();
				running = false;
			}
		}

	}
	
	public void stop() {
		try {
			udpSocket.close();
			tcpSocket.close();
			System.out.println("Socket closed");
		} catch (Exception e) {
			e.printStackTrace();
		}

		running = false;
	}

	public void start() {
		Thread thread = new Thread(this);
		thread.start();
	}
	
	public class Backward implements Runnable {
		TCPvsUDPBridgeBase bridge;
		public Backward(TCPvsUDPBridgeBase b){
			bridge=b;
		}
		public void run() {
			bridge.runBackward();
		}
	}
	public class Forward implements Runnable {
		TCPvsUDPBridgeBase bridge;
		public Forward(TCPvsUDPBridgeBase b){
			bridge=b;
		}
		public void run() {
			bridge.runForward();
		}
	}
	
	public void run() {
		try {
			if (fileout) {
				fileToUDP = new FileOutputStream(LogToUDP);
				fileFromUDP = new FileOutputStream(LogFromUDP);
			}
			//bridge = this;
			new Thread(new Backward(this)).start();
			new Thread(new Forward(this)).start();
			
			while(running) {
				Thread.sleep(100);
			}
		} catch (IOException ioe) {
			ioe.printStackTrace();
		} catch (InterruptedException ite) {
			ite.printStackTrace();
		} finally {
			try {
				if (fileout) {
					fileToUDP.close();
					fileFromUDP.close();
				}
			} catch (IOException cle) {
				cle.printStackTrace();
			}
		}
	}
}
