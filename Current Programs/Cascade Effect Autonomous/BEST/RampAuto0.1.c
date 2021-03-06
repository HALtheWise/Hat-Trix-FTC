#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Hubs,  S3, HTServo,  none,     none,     none)
#pragma config(Sensor, S2,     gyro,           sensorI2CHiTechnicGyro)
#pragma config(Sensor, S4,     seeker,         sensorI2CCustom)
#pragma config(Motor,  motorA,          sweeper1,      tmotorNXT, openLoop, reversed)
#pragma config(Motor,  motorB,          sweeper2,      tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     FrontL,        tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     BackL,         tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     FrontR,        tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     BackR,         tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     rightRoller,   tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     leftRoller,    tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C4_1,     elevator,      tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C4_2,     car,           tmotorTetrix, openLoop, encoder)
#pragma config(Servo,  srvo_S3_C1_1,    grabberServo,         tServoStandard)
#pragma config(Servo,  srvo_S3_C1_2,    dropperServo,         tServoStandard)
#pragma config(Servo,  srvo_S3_C1_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S3_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S3_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S3_C1_6,    servo6,               tServoNone)
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
