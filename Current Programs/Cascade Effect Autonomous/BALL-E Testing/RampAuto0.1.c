#pragma config(Sensor, S2,     seeker,         sensorI2CCustom)
#pragma config(Sensor, S4,     gyro,           sensorI2CHiTechnicGyro)
#pragma config(Motor,  motorA,          auxMotor,      tmotorNXT, openLoop)
#pragma config(Motor,  motorB,          FrontL,        tmotorNXT, openLoop, reversed, encoder)
#pragma config(Motor,  motorC,          FrontR,        tmotorNXT, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "BALL-E overrides.c"


#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.
#include "TrackerAutoLib.h"
//#include "AutoLib.h"

#define DEBUG false

void initializeRobot()
{
	clearDebugStream();
	gyroCal();
	//servo[grabberServo]=???;
	//servo[dumperServo] =???;
	return;
}

void DemoFunctions() //Never called, just examples to copy from.
{
	FieldPos p;

	p.x = 175; p.y = 265;
	turnAndMoveTo(p, 50);

	if (DEBUG) wait1Msec(2000);
	else wait1Msec(200);

	turnToHeading(degreesToRadians(-90), 40, Backward);

	p.x += 0; p.y -= 33;
	moveTo(p, 30, Backward, NO_STEERING);
}

void rampStart(){
	FieldPos p;
	memset(&p, 0, sizeof(p));

	releaseGoal();

	p.x = 215; p.y = 30.2;
	moveTo(p, 50, Backward);		//Disembark ramp POWER SHOULD BE 20

	p.x = 270; p.y = 90;
	turnAndMoveTo(p, 50, Backward);

	if (DEBUG) wait1Msec(2000);
	else wait1Msec(200);

	p.x = 340; p.y = 27;
	turnAndMoveTo(p, 30, Backward);	//Go	to tall goal

	grabGoal();
	wait1Msec(1000);

	p.x = 270; p.y = 90;
	turnAndMoveTo(p, 35, Forward);	//Safely extract from goal zone

	p.x = 155; p.y = 110;
	turnAndMoveTo(p, 60, Forward);	//Gap between structure and ramp

	if (DEBUG) wait1Msec(2000);
	else wait1Msec(200);

	p.x = 55; p.y = 140;
	turnAndMoveTo(p, 60, Backward);	//Parking zone, after spinning

	releaseGoal();
	wait1Msec(1000);

	p.x = 155; p.y = 110;
	turnAndMoveTo(p, 60, Forward);	//Gap between structure and ramp
}

task main()
{
	initializeRobot();

	waitForStart(); // Wait for the beginning of autonomous phase.

	resetTracker();
	//Floor start tracking positions
	robot.x = 22.9 - 12.8; //9in, defined from robot away from wall, minus 5inches due to the slope of the ramp and trigonometry.
	robot.y = 30.2; //12in, on ramp half way from wall.
	robot.theta = degreesToRadians(180);
	StartTask(trackRobot);

	rampStart();
}
