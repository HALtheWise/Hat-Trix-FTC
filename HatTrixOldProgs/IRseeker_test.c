#pragma config(Sensor, S4,     IRSeeker,       sensorHiTechnicIRSeeker1200)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

task main()
{

	clearDebugStream();
	while(1)
	{
		int value = SensorValue(S4);
		wait10Msec(50);
		writeDebugStreamLine(" seeker value: %d" , value);
}




}