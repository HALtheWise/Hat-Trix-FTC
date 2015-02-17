#pragma config(Sensor, S4,     sonar,          sensorSONAR)

int julietUS(){
	int reading = SensorValue[sonar];
	if(reading >= 130 && reading < 150){
		return 1;
	}
	else if(reading < 130 && reading > 110){
		return 3;
	}else{
		return 2;
	}
}
