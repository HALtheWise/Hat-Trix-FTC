package initialTesting;

public class NxtInfo {
	public int comPort;
	public String brickName;
	public String friendlyName;
	
	public NxtInfo(int comPort, String name){
		this.comPort = comPort;
		this.brickName = this.friendlyName = name;
	}
}
