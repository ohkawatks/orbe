/*
 * $Id: TCPtoUART.java 793 2008-09-17 02:09:28Z takaya $
 * Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology
 *
 */

package jp.go.aist.utils;

import gnu.io.CommPortIdentifier;
import gnu.io.NoSuchPortException;
import gnu.io.PortInUseException;
import gnu.io.SerialPort;
import gnu.io.SerialPortEvent;
import gnu.io.SerialPortEventListener;
import gnu.io.UnsupportedCommOperationException;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.Iterator;
import java.util.TooManyListenersException;

public class TCPtoUART implements Runnable, SerialPortEventListener, TransportBridge {
	int tcpServerPort = 5656;
	final int BUF_SIZE = 1024;

	protected InputStream comInputStream;
	protected OutputStream comOutputStream;

	FileOutputStream fileOutToUart = null;
	FileOutputStream fileInFromUart = null;
	protected boolean fileout = true;

	protected Selector selector;
	protected SocketChannel socketChannel;
	
    protected SerialPort port;

    public TCPtoUART(String comPortName, String tcpPortNumber){
    	this(comPortName, Integer.parseInt(tcpPortNumber));
    }

    public TCPtoUART(String comPortName, int tcpPortNumber){

        CommPortIdentifier portID = null;
        try{
            portID = CommPortIdentifier.getPortIdentifier(comPortName);
        }catch(NoSuchPortException ex){
            throw new RuntimeException(ex);
        }

        try{
            port = (SerialPort)portID.open("SerialPortWriter", 5000);
        }catch(PortInUseException ex){
            throw new RuntimeException(ex);
        }

        try {
            port.setSerialPortParams(115200, 
                                     SerialPort.DATABITS_8, 
                                     SerialPort.STOPBITS_1, 
                                     SerialPort.PARITY_NONE); 
            port.setFlowControlMode(SerialPort.FLOWCONTROL_NONE);
        } catch (UnsupportedCommOperationException ex){
            throw new RuntimeException(ex);
        }
        
        tcpServerPort = tcpPortNumber;

		try {
			port.addEventListener(this);
		} catch (TooManyListenersException ex) {
			ex.printStackTrace();
			System.exit(1);
		}

		port.notifyOnDataAvailable(true);

		try {
			comInputStream = port.getInputStream();
			comOutputStream = port.getOutputStream();

			selector = Selector.open();
			ServerSocketChannel ssc = ServerSocketChannel.open();
			ssc.configureBlocking(false);
			ssc.socket().bind(new InetSocketAddress(tcpServerPort));

			ssc.register(selector, SelectionKey.OP_ACCEPT);

		} catch (IOException ex) {
			ex.printStackTrace();
			System.exit(1);
		}
	}

	/**
	 * Buffer from COM to TCP
	 */
	private byte[] byteArrayC2T = new byte[BUF_SIZE];

	public void serialEvent(SerialPortEvent event) {
		switch (event.getEventType()) {
		case SerialPortEvent.BI:
		case SerialPortEvent.OE:
		case SerialPortEvent.FE:
		case SerialPortEvent.PE:
		case SerialPortEvent.CD:
		case SerialPortEvent.CTS:
		case SerialPortEvent.DSR:
		case SerialPortEvent.RI:
		case SerialPortEvent.OUTPUT_BUFFER_EMPTY:
			System.out.println("Event");
			break;
		case SerialPortEvent.DATA_AVAILABLE:
			try {
				while (comInputStream.available() > 0) {
					int length = comInputStream.read(byteArrayC2T);
					socketChannel.write(ByteBuffer.wrap(byteArrayC2T, 0, length));

					if (fileout) {
						fileInFromUart.write(byteArrayC2T, 0, length);
						fileInFromUart.flush();
					}
				}
			} catch (IOException e) {
			}
			break;
		}
	}

	public void start() {
		Thread thread = new Thread(this);
		thread.start();
	}

    volatile public boolean running = true;
	/**
	 * Buffer from TCP to COM
	 */
	public void run() {
		int length = 0;
		try {
			if (fileout) {
				fileOutToUart = new FileOutputStream("ToUart.bin");
				fileInFromUart = new FileOutputStream("FromUart.bin");
			}
			while (running) {
				try {
					while (running && (selector.select(100) > 0)) {
						Iterator<SelectionKey> itr = selector.selectedKeys()
								.iterator();
						while (itr.hasNext()) {
							SelectionKey key = itr.next();
							itr.remove();
							if (key.isAcceptable()) {
								ServerSocketChannel ssc = (ServerSocketChannel) key
										.channel();
								SocketChannel sc = ssc.accept();
								sc.configureBlocking(false);
								sc.register(selector, SelectionKey.OP_READ);
							} else if (key.isReadable()) {
								SocketChannel sc = (SocketChannel) key
										.channel();
								socketChannel = sc;
								ByteBuffer buf = ByteBuffer.allocate(BUF_SIZE);
								if ((length = sc.read(buf)) < 0) {
									sc.close();
									continue;
								}
								buf.flip();
								byte[] byteArray = buf.array();
								comOutputStream.write(byteArray, 0, length);
								comOutputStream.flush();

								if (fileout) {
									fileOutToUart.write(byteArray, 0, length);
									fileOutToUart.flush();
								}
							}
						}
					}
				} catch (IOException e) {
					e.printStackTrace();
					running = false;
				}
			}
		} catch (IOException ioe) {
			ioe.printStackTrace();
		} finally {
			try {
				if (fileout) {
					fileOutToUart.close();
					fileInFromUart.close();
				}
			} catch (IOException cle) {
				cle.printStackTrace();
			}
		}
		try {
			socketChannel.close();
			System.out.println("Socket closed");
			
			port.removeEventListener();
		} catch (Exception e) {
			e.printStackTrace();
		}
		
	}
	
	public void stop() {
		running = false;
		//this.notifyAll();
	}

    public void close(){
        port.close();
    }
    
	public static void main(String[] args) {
		TCPtoUART terminal = new TCPtoUART(args[0], args[1]);
		terminal.start();
	}
}
