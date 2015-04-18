#pragma config(Sensor, S4,     HTSMUX,            sensorI2CCustom9V )

#include "drivers/hitechnic-sensormux.h"
#include "drivers/lego-ultrasound.h"

const tMUXSensor frontSonar = msensor_S4_3;
const tMUXSensor sideSonar = msensor_S4_4;

int getFrontSensorReading(){
	return USreadDist(frontSonar);
}

int getSideSensorReading(){
	return USreadDist(sideSonar);
}

int julietUS(bool ramp = false){
	int reading = getFrontSensorReading();
	int centerPosition = 0;

	float rPos;
	if (ramp){
	//	rPos = robot.y - 10.0;
	}
	else{
	//	rPos = robot.x - 10.0;
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
