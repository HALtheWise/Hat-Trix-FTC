#pragma config(Sensor, S4,     sonar,          sensorSONAR)

int julietUS(){
	int reading = SensorValue[sonar];
	int centerPosition = 0;

	float rPos = robot.x - 10.0;

	if(reading >= 130 - rPos && reading < 150 - rPos){
		centerPosition = 1;
	}
	else if(reading < 130 - rPos && reading > 110 - rPos){
		centerPosition = 3;
	}else{
		centerPosition = 2;
	}

	writeDebugStreamLine("DETECTED CENTER STRUCTURE POSITION %d", centerPosition);
	return centerPosition;
}
