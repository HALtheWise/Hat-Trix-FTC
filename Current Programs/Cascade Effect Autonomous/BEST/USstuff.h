#pragma config(Sensor, S4,     sonar,          sensorSONAR)

void updateOnCenterStructure(){
	const int USoffset = 35;
	int reading = SensorValue[sonar];

	RelativePos centerRelativePos;
	FieldPos centerFieldPos;
	getCenterFieldPos(centerFieldPos);
	subtract(robot, centerFieldPos, centerRelativePos);

	float newX = -(reading + USoffset);
	writeDebugStreamLine("Updated on center structure, difference was %dcm", newX - centerRelativePos.x);
	centerRelativePos.x = newX;

	add(centerFieldPos, centerRelativePos, robot);
}

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
