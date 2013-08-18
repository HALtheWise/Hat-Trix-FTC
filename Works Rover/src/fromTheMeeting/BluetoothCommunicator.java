package fromTheMeeting;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

import javax.bluetooth.*;
import javax.microedition.io.Connection;
import javax.microedition.io.StreamConnection;
import javax.microedition.io.Connector;

public class BluetoothCommunicator {
	String brickID;
	int portNum = 1;
	LocalDevice localDevice;
	Connection connection;
	DataOutputStream out;
	DataInputStream in;
	
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
			out = ((StreamConnection) connection).openDataOutputStream();
			in = ((StreamConnection) connection).openDataInputStream();
		}catch(Exception e){
			e.printStackTrace();
		}
	}
	
	public void sendData(byte[] data){
		try {
			out.write(data);
			out.writeByte(END_OF_MESSAGE);
			out.flush();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
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
