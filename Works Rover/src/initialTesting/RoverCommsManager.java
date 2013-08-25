package initialTesting;

import java.util.ArrayList;

public class RoverCommsManager {

	private static final int PACKET_LENGTH = 16;
	protected NxtInfo currentDevice;
	protected BluetoothCommunicator communicator;
	
	public ArrayList<NxtInfo> knownDevices = new ArrayList<NxtInfo>();
	
	public RoverCommsManager() {
		knownDevices.add(new NxtInfo("0016530082A7", "HAPPINESS"));
		// TODO Auto-generated constructor stub
	}
	
	public void connectToNXT(NxtInfo device){
		if(communicator != null){
			communicator.close();
		}
		currentDevice = device;
		communicator = new BluetoothCommunicator(device.ip);
		communicator.connect();
	}
	
	public byte[] makeDataPacket(HIDManager hidManager){
		byte[] data = new byte[PACKET_LENGTH];
		
		return data;
	}
	
	
	public static void sanitize(byte[] data){
		for(int i=0; i<data.length; i++){
//			if(data[i] == END_OF_MESSAGE){
//				data[i] -= 1;
//			}
		}
	}
}
