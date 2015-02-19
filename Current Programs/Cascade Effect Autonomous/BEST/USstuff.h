#pragma config(Sensor, S4,     sonar,          sensorSONAR)

int julietUS(){
	int reading = SensorValue[sonar];
	int centerPosition = 0;
	if(reading >= 130 && reading < 150){
		centerPosition = 1;
	}
	else if(reading < 130 && reading > 110){
		centerPosition = 3;
	}else{
		centerPosition = 2;
	}

	writeDebugStreamLine("DETECTED CENTER STRUCTURE POSITION %d", centerPosition);
	return centerPosition;
}
