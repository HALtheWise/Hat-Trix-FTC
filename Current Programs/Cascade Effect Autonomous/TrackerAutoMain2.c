#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Hubs,  S3, HTServo,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     seeker,         sensorI2CCustom)
#pragma config(Sensor, S3,     ,               sensorI2CMuxController)
#pragma config(Sensor, S4,     gyro,           sensorI2CHiTechnicGyro)
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
#pragma config(Servo,  srvo_S3_C1_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S3_C1_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S3_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S3_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S3_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.
#include "TrackerAutoLib.h"
#include "AutoLib.h"
#include "IRstuff.c"

#define DEBUG true

void initializeRobot()
{
	clearDebugStream();
	gyroCal();
	//servo[grabberServo]=???;
	//servo[dumperServo] =???;
	return;
}

int centerPos = -1;

void floorStart(){
	FieldPos p;
	memset(&p, 0, sizeof(p));

	p = robot;
	p.x += 80;	p.y += 0; //Forward 80cm
	moveTo(p, 40);

	//p.x = 1000000;	p.y = -420000; //Approximately 25 degree angle right
	//turnTo(p, 30);
	turnToHeading(degreesToRadians(-25), 30);

	centerPos = juliet();

	writeDebugStreamLine("position=%d", centerPos);

	if (DEBUG) wait1Msec(4000);
	else wait1Msec(200);

	if(centerPos == 1)
	{
		p.x = 175; p.y = 265;
		turnAndMoveTo(p, 50);

		if (DEBUG) wait1Msec(2000);
		else wait1Msec(200);

		turnToHeading(degreesToRadians(-90), 40, Backward);

		if (DEBUG) wait1Msec(2000);
		else wait1Msec(200);

		//turn(-45, 25);
		liftTallArm();

		p.x += 0; p.y -= 33;
		moveTo(p, 30, Backward, NO_STEERING);
		//move(-33, 30);
		dumpTallBalls();

		if (DEBUG) wait1Msec(2000);
		else wait1Msec(200);


		p.x += 0; p.y += 15;
		moveTo(p, 30, Forward, NO_STEERING);
		//move(15, 30);
		lowerTallArm();


		if (DEBUG) wait1Msec(2000);
		else wait1Msec(200);


		p.x = 155; p.y = 250;
		turnAndMoveTo(p, 50);

		if (DEBUG) wait1Msec(2000);
		else wait1Msec(200);

		p.x = 155; p.y = 170; //Hit kickstand
		turnAndMoveTo(p, 50);

		if (DEBUG) wait1Msec(3000);
		else wait1Msec(1000);

		p.x = 60; p.y = 182; //Return to base
		turnAndMoveTo(p, 50);
		turnToHeading(degreesToRadians(0), 40);

		//turn(90, 25);
		//move(-20, 30);
		//turn(85, 25);
		//move(80, 50);
	}
	else if(centerPos == 2)
	{
		//do stuff
	}
	else if(centerPos == 3)
	{
		//do stuff
	}
	else writeDebugStreamLine("Detection of center structure failed in unexpected way.");
}

task main()
{
	initializeRobot();

	waitForStart(); // Wait for the beginning of autonomous phase.

	resetTracker();
	//Floor start tracking positions
	robot.x = 22.9; //9in, defined from robot
	robot.y = 213.0; //84in, measured
	robot.theta = 0.0;
	StartTask(trackRobot);

	floorStart();
}
