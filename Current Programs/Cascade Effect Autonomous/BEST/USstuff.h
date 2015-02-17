#pragma config(Sensor, S4,     sonar,          sensorSONAR)

int julietUS(){
	int reading = SensorValue[sonar];
	if (reading == 255){
		return 2;
	}
	else if(reading > 130){
		return 1;
	}
	else{
		return 3;
	}
}
