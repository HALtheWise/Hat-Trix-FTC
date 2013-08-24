package initialTesting;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

import javax.bluetooth.*;
import javax.microedition.io.Connection;
import javax.microedition.io.StreamConnection;
import javax.microedition.io.Connector;

public class BluetoothCommunicator {
	static final int portNum = 1;

	String brickID;
	LocalDevice localDevice;
	Connection connection;
	BufferedOutputStream out;
	BufferedInputStream in;
	
	public static final byte END_OF_MESSAGE = 42;
	
	//	HAPPINESS == "0016530082A7"
	
	public static void main(String[] args){
		BluetoothCommunicator bc = new BluetoothCommunicator("0016530082A7");
		bc.connect(); 
		System.out.println("connected");
		try{System.in.read();}catch(Exception e){};
		System.out.println("sending");
		
		byte[] data = "hello".getBytes();
		sanitize(data);
		bc.sendData(data);
		
		System.out.println("sent");
		bc.close();
	}
	
	public BluetoothCommunicator(String brick){
		brickID = brick;
		try{
			localDevice = LocalDevice.getLocalDevice();
		}catch(Exception e){
			e.printStackTrace();
		}
	}
	
	public void connect(){
		try{
			connection = Connector.open("btspp://"+brickID+":"+portNum);
			out = new BufferedOutputStream(((StreamConnection) connection).openDataOutputStream());
			in = new BufferedInputStream(((StreamConnection) connection).openDataInputStream());
		}catch(Exception e){
			e.printStackTrace();
		}
	}
	
	public void sendData(byte[] data){
		try {
			out.write(data);
			out.write(END_OF_MESSAGE);
			out.flush();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public void sendNXTMessage(byte[] data){
		byte[] dataToSend = new byte[data.length + 4];
		dataToSend[0] = (byte) 0x80; //Command without response expected
		dataToSend[1] = (byte) 0x09; //Direct Message
		dataToSend[2] = (byte) 0x00; //Mailbox #1
		dataToSend[3] = (byte) data.length; //Length of message
		for (int i = 0; i<data.length; i++){
			dataToSend[i+4] = data[i];
		}
		try {
			out.write(dataToSend);
			out.flush();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	
	/**
	 * This method has never been seen to work, but should be used anyway.
	 */
	public void close(){
		try{
			connection.close();
		}catch(Exception e){
			e.printStackTrace();
		}
	}
	
	public static void sanitize(byte[] data){
		for(int i=0; i<data.length; i++){
			if(data[i] == END_OF_MESSAGE){
				data[i] -= 1;
			}
		}
	}
}
