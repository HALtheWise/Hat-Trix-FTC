package initialTesting;

import java.util.ArrayList;

public class BrickListManager {

	public String filename;
	public ArrayList<NxtInfo> knownDevices = new ArrayList<NxtInfo>();

	public BrickListManager(String filename) {
		this.filename = filename;
	}

	public boolean saveToFile(){
		return false;
	}

}
