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

//#include "BALL-E overrides.c" //COMMENT OUT THIS LINE BEFORE RUNNING ON COMPETITION ROBOT


#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.
#include "TrackerAutoLib.h"
#include "AutoLib.h"
#include "Field Positions.h"
//#include "IRstuff.c"

#define DEBUG false
#define DOLIFT true

int centerPos = -1;

typedef struct {
	float x; //(0,0) is at the very center of the field. Positive x extends away from our color center goal.
	float y; //Positive y extends away from our color kickstand
} CenterRelativePos;

void translate(CenterRelativePos input, FieldPos *result){
	const float FIELD_SIZE = 365.76;
	float angle = 0.0; //Heading of center structure with respect to field coords.
	if (centerPos == 1){
		angle = degreesToRadians(-90);
	}else if (centerPos == 2){
		angle = degreesToRadians(-45);
	}else if (centerPos == 1){
		angle = 0;
	}else{
		writeDebugStreamLine("Warning: center position not detected yet!");
	}
	result->x = FIELD_SIZE/2;
	result->y = FIELD_SIZE/2;
	result->theta = 0;

	result->x += input.x * cos(angle) - input.y * sin(angle);
	result->y += input.y * cos(angle) + input.x * sin(angle);
}

void turnAndMoveTo (CenterRelativePos target, int power, DrivingDirection forward = Forward){
		FieldPos target2;
		translate(target, target2);
		turnAndMoveTo(target2, power, forward);
}

void initializeRobot()
{
	clearDebugStream();
	gyroCal();
	//servo[grabberServo]=???;
	//servo[dropperServo] =???;
	return;
}

void floorStart(){
	FieldPos p;
	memset(&p, 0, sizeof(p));

	servo[dropperServo] = 240;
	if(DOLIFT) liftFirstStage();

	p.x = 100;	p.y = 213.0; //Forward 80cm
	moveTo(p, 40);

	turnToHeading(degreesToRadians(-27), 30);

//	centerPos = juliet(); //Take IR beacon reading


	centerPos = 1;
	//fail! You forgot to comment the BALL-E import!
//

	writeDebugStreamLine("DETECTED CENTER STRUCTURE POSITION %d", centerPos);

	if (DEBUG) wait1Msec(4000);
	else wait1Msec(200);

	if(centerPos == 1)
	{
		p.x = 171; p.y = 265;
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
		dumpBalls();

		if (DEBUG) wait1Msec(2000);
		else wait1Msec(200);


		p.x += 0; p.y += 15;
		moveTo(p, 30, Forward, NO_STEERING);
		//move(15, 30);
		lowerTallArm();


		if (DEBUG) wait1Msec(2000);
		else wait1Msec(200);


		p.x = 150; p.y = 250;
		turnAndMoveTo(p, 50);

		if (DEBUG) wait1Msec(2000);
		else wait1Msec(200);

		/*p.x = 150;*/ p.y = 170; //Hit kickstand
		turnAndMoveTo(p, 50);

		if (DEBUG){
			wait1Msec(2000);
			p.x = 22; p.y = 213; //Return to base
			turnAndMoveTo(p, 50);
			turnToHeading(degreesToRadians(0), 40);
		}
	}
	else if(centerPos == 2)///////////////////////////////////////////////////////////////////////////////////////////
	{
		//It just so happens that the position for delivery of the POSITION=2 ball is exactly the same as the IR sensing position.
		//p.x = 100; p.y = 213;
		//turnAndMoveTo(p, 50);

		if (DEBUG) wait1Msec(1000);
		else wait1Msec(200);

		turnToHeading(degreesToRadians(-35), 40, Backward);

		if (DEBUG) wait1Msec(1000);
		else wait1Msec(200);

		liftTallArm();

		p.x += 50; p.y += -20;
		moveTo(p, 30, Backward, NO_STEERING);
		dumpBalls();

		if (DEBUG) wait1Msec(2000);
		else wait1Msec(200);


		p.x -= 20; p.y += 5;
		moveTo(p, 30, Forward, NO_STEERING);
		lowerTallArm();


		if (DEBUG) wait1Msec(1000);
		else wait1Msec(200);


		p.x = 90; p.y = 175;
		turnAndMoveTo(p, 50);

		if (DEBUG) wait1Msec(2000);
		else wait1Msec(200);

		p.x = 180; p.y = 145; //Hit kickstand
		turnAndMoveTo(p, 50);

		if (DEBUG){
			wait1Msec(2000);
			p.x = 22; p.y = 213; //Return to base
			turnAndMoveTo(p, 50);
			turnToHeading(degreesToRadians(0), 40);
		}
	}
	else if(centerPos == 3)///////////////////////////////////////////////////////////////////////////////////////////
	{
		p.x = 100; p.y = 155;
		turnAndMoveTo(p, 50);

		if (DEBUG) wait1Msec(1000);
		else wait1Msec(200);

		turnToHeading(degreesToRadians(0), 40, Backward);

		if (DEBUG) wait1Msec(1000);

		liftTallArm();

		p.x += 30; p.y += 0;
		moveTo(p, 30, Backward, NO_STEERING);
		dumpBalls();

		if (DEBUG) wait1Msec(2000);
		else wait1Msec(200);


		p.x -= 15; p.y += 0;
		moveTo(p, 30, Forward, NO_STEERING);
		lowerTallArm();


		if (DEBUG) wait1Msec(1000);
		else wait1Msec(200);


		/*p.x = 110;*/ p.y = 130;
		turnAndMoveTo(p, 50);

		if (DEBUG) wait1Msec(2000);
		else wait1Msec(200);

		p.x = 195; /*p.y = 155;*/ //Hit kickstand
		turnAndMoveTo(p, 50);

		if (DEBUG){
			wait1Msec(2000);
			p.x = 22; p.y = 213; //Return to base
			turnAndMoveTo(p, 50);
			turnToHeading(degreesToRadians(0), 40);
		}
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
