#pragma config(Sensor, S4,     sonar,          sensorSONAR)

int julietUS(bool ramp = false){
	int reading = SensorValue[sonar];
	int centerPosition = 0;

	float rPos;
	if (ramp){
		rpos = robot.y - 10.0;
	}
	else{
		rpos = robot.x - 10.0;
	}

	if(reading >= 130 - rPos && reading < 150 - rPos){
		centerPosition = 1;
	}
	else if(reading < 130 - rPos && reading > 110 - rPos){
		centerPosition = 3;
		}else{
		centerPosition = 2;
	}

	if (ramp){
		centerPosition = 4 - centerPosition; //Converts 1 -> 3 and 3 -> 1 while leaving 2 unchanged
	}

	writeDebugStreamLine("DETECTED CENTER STRUCTURE POSITION %d", centerPosition);
	return centerPosition;
}
