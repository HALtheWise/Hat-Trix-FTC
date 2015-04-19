#pragma config(Sensor, S4,     HTSMUX,            sensorI2CCustom9V )

#include "drivers/hitechnic-sensormux.h"
#include "drivers/lego-ultrasound.h"

const tMUXSensor frontSonar = msensor_S4_3;
const tMUXSensor sideSonar = msensor_S4_4;

int getFrontSensorReading(bool adjusted = false){
	hogCPU();
	int value;
	if (adjusted){
		value = USreadDist(frontSonar) + 12; //TODO: this value has not been tuned.
	}
	else{
		value = USreadDist(frontSonar);
	}
	releaseCPU();
	return value;
}

int getSideSensorReading(bool adjusted = false){
	hogCPU();
	int value;
	if (adjusted){
		value = USreadDist(sideSonar) + 22;
	}
	value = USreadDist(sideSonar);
	releaseCPU();
	return value;
}

int julietUS(bool ramp = false){
	int reading;
	if(ramp){
		reading = getSideSensorReading(true);
	}
	else{
		reading = getFrontSensorReading();
	}
	int centerPosition = 0;

	float rPos;
	if (ramp){
		rPos = robot.y - 10.0;
	}
	else{
		rPos = robot.x - 10.0;
	}

	reading = reading + rPos;

	writeDebugStreamLine("US sensor reads: %d", reading);

	if(reading >= 130 && reading < 150){
		centerPosition = 1;
	}
	else if(reading < 130 && reading > 110){
		centerPosition = 3;
	}
	else{
		centerPosition = 2;
	}

	if (ramp){
		centerPosition = 4 - centerPosition; //Converts 1 -> 3 and 3 -> 1 while leaving 2 unchanged
	}

	writeDebugStreamLine("DETECTED CENTER STRUCTURE POSITION %d", centerPosition);
	return centerPosition;
}

// 0 is +x, goes clockwise.
// TODO: this should account for ramp sides and (possibly) the tower.
float expectedUSreading(FieldPos p, int facing){
	switch (facing)
	{
	case 0:
		return FIELD_SIZE - p.x;
		break;
	case 1:
		return FIELD_SIZE - p.y;
		break;
	case 2:
		return p.x;
		break;
	case 3:
		return p.y;
		break;

	default:
		writeDebugStreamLine("ERROR: unexpected facing direction");
	}
	return 0;
}

void adjustByDelta(float delta, int facing, FieldPos *p){
	switch (facing)
	{
	case 0:
		p->x += delta;
		break;
	case 1:
		p->y += delta;
		break;
	case 2:
		p->x -= delta;
		break;
	case 3:
		p->y -= delta;
		break;

	default:
		writeDebugStreamLine("ERROR: unexpected facing direction");
	}
}

// Converts the angle of the sensor into integers 0...3 as described above,
// or -1 if it isn't within THRESHOLD. Assumes the input has been normalized.
int direction(float ang){
	const float THRESHOLD = degreesToRadians(10);
	if (abs(ang) < THRESHOLD) return 0;
	if (abs(ang - PI/2) < THRESHOLD) return 1;
	if (abs(ang - PI) < THRESHOLD || abs(ang + PI) < THRESHOLD) return 2;
	if (abs(ang + PI/2) < THRESHOLD) return 3;
	return -1;
}

//********************************************************************************************************
//*
//*	This thread doesn't quite work yet, but we merged it into the judging branch anyway because
//* we thought it was interesting. It endeavors to constantly monitor the robot position, and identify
//* any time that the sonar just happens to be facing a wall. Then, if it thinks it sees the wall,
//* it will use that to update the robot's positon estimate.
//*
//********************************************************************************************************
task USmagic(){
	while(true){
		wait1Msec(50);
		int facing = direction(robot.theta);
		if (facing < 0) continue;

		float delta = getFrontSensorReading(true) - expectedUSreading(robot, (facing+2)%4);
		writeDebugStreamLine("US correction: %dcm; facing: %d", delta, facing);
		wait1Msec(150);
	}
}
