package initialTesting;

public class NxtInfo {
	public String ip;
	public String brickName;
	public String friendlyName;
	
	//comment
	public NxtInfo(String btIP, String name){
		this.ip = btIP;
		this.brickName = this.friendlyName = name;
	}
}
