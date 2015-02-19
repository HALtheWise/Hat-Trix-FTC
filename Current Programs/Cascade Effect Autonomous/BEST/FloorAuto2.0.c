#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Hubs,  S3, HTServo,  none,     none,     none)
#pragma config(Sensor, S2,     gyro,           sensorI2CHiTechnicGyro)
#pragma config(Sensor, S4,     sonar,          sensorSONAR)
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

typedef struct {
	float x; //(0,0) is at the very center of the field. Positive x extends away from our color center goal.
	float y; //Positive y extends away from our color kickstand
} CenterRelativePos;


#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.
#include "TrackerAutoLib.h"
#include "AutoLib.h"
#include "Field Positions.h"
#include "USstuff.h"
//#include "IRstuff.c"

#define DEBUG false
#define DOLIFT1 true
#define DOLIFT2 true

int centerPos = -1;

float getCenterAngle(){
	if (centerPos == 1){
		return degreesToRadians(-90);
		}else if (centerPos == 2){
		return degreesToRadians(-45);
		}else if (centerPos == 1){
		return 0;
		}else{
		writeDebugStreamLine("Warning: center position not detected yet!");
		return 0;
	}
}

void translate(CenterRelativePos input, FieldPos *result){
	writeDebugStreamLine("Input was %d, %d", input.x, input.y);
	const float FIELD_SIZE = 365.76;
	float angle = getCenterAngle(); //Heading of center structure with respect to field coords.
	result->x = FIELD_SIZE/2;
	result->y = FIELD_SIZE/2;
	result->theta = angle;

	result->x = result->x + input.x * cos(angle) - input.y * sin(angle);
	result->y = result->y + input.y * cos(angle) + input.x * sin(angle);

	writeDebugStreamLine("Output was %d, %d", result->x, result->y);
}

void turnAndMoveTo (CenterRelativePos target, int power, DrivingDirection forward = Forward){
	FieldPos target2;
	translate(target, target2);
	turnAndMoveTo(target2, power, forward);
}

void turnTo (CenterRelativePos target, int power, DrivingDirection forward = Forward){
	FieldPos target2;
	translate(target, target2);
	turnTo(target2, power, forward);
}

void moveTo (CenterRelativePos target, int power, DrivingDirection forward = Forward){
	FieldPos target2;
	translate(target, target2);
	moveTo(target2, power, forward);
}

void initializeRobot()
{
	initPositions();
	clearDebugStream();
	gyroCal();
	servo[dropperServo] = HOLDING_POS;
	//servo[grabberServo]=???;
	return;
}

void floorStart(){
	const int speed_fast = 70;
	const int speed_normal = 60;
	const int speed_slower = 45;
	const int speed_precise = 35;

	const float GRAB_GOAL = true;

	//	centerPos = 1; //Override for testing purposes
	centerPos = julietUS();

	if(DOLIFT1) liftFirstStage(true);
	wait1Msec(600);

	writeDebugStreamLine("DETECTED CENTER STRUCTURE POSITION %d", centerPos);

	turnAndMoveTo(GPS_awayFromWallUS, speed_normal, Backward);

	if (DEBUG) wait1Msec(4000);
	else wait1Msec(200);

	if(centerPos == 1)
	{
		turnAndMoveTo(GPS_prepareForCenterDump, speed_normal, Backward);
		if (DEBUG) wait1Msec(2000);
		else wait1Msec(100);

		while(!firstStageIsLifted){wait1Msec(5);}
		if(DOLIFT2) liftTallArm();

		turnTo(GPS_centerDumpPosition1, speed_slower, Backward);
		moveTo(GPS_centerDumpPosition1, speed_precise, Backward);

		wait1Msec(2000);
		if(DOLIFT2) dumpBalls();

		if (DEBUG) wait1Msec(2000);
		else wait1Msec(100);

		turnAndMoveTo(GPS_prepareForCenterDump, speed_normal, Forward);
		if(DOLIFT2) lowerTallArm();

		if (DEBUG) wait1Msec(2000);
		else wait1Msec(100);

		if (GRAB_GOAL){
			turnAndMoveTo(GPS_navPoint1, speed_fast);
			turnAndMoveTo(GPS_mediumGoalPosition, speed_fast, Backward);
			grabGoal();
		}
		else{

			turnAndMoveTo(GPS_prepareForKickstand, speed_normal);

			if (DEBUG) wait1Msec(2000);
			else wait1Msec(100);

			turnAndMoveTo(GPS_hitKickstand, speed_normal);
		}
	}
	else if(centerPos == 2)///////////////////////////////////////////////////////////////////////////////////////////
	{
		turnAndMoveTo(GPS_prepareForCenterDump, speed_normal, Backward);
		if (DEBUG) wait1Msec(2000);
		else wait1Msec(200);

		while(!firstStageIsLifted){wait1Msec(5);}

		if(DOLIFT2) liftTallArm();

		turnTo(GPS_centerDumpPosition2, speed_slower, Backward);
		moveTo(GPS_centerDumpPosition2, speed_precise, Backward);

		wait1Msec(2000);
		if(DOLIFT2) dumpBalls();

		if (DEBUG) wait1Msec(2000);
		else wait1Msec(200);

		turnAndMoveTo(GPS_prepareForCenterDump, speed_normal, Forward);
		if(DOLIFT2) lowerTallArm();

		if (DEBUG) wait1Msec(2000);
		else wait1Msec(200);

		if (GRAB_GOAL){
			turnAndMoveTo(GPS_navPoint1, speed_fast);
			wait1Msec(100);
			turnAndMoveTo(GPS_mediumGoalPosition, speed_fast, Backward);
			grabGoal();
		}
		else{

			turnAndMoveTo(GPS_prepareForKickstand, speed_normal);

			if (DEBUG) wait1Msec(2000);
			else wait1Msec(200);

			turnAndMoveTo(GPS_hitKickstand, speed_normal);
		}
	}
	else if(centerPos == 3)///////////////////////////////////////////////////////////////////////////////////////////
	{
		//turnAndMoveTo(GPS_prepareForCenterDump, speed_normal, Backward);
		if (DEBUG) wait1Msec(2000);
		//else wait1Msec(200);

		while(!firstStageIsLifted){wait1Msec(5);}

		if(DOLIFT2) liftTallArm();

		turnTo(GPS_centerDumpPosition3, speed_normal, Backward);
		moveTo(GPS_centerDumpPosition3, speed_precise, Backward);

		wait1Msec(2000);
		if(DOLIFT2) dumpBalls();

		if (DEBUG) wait1Msec(2000);
		//else wait1Msec(200);

		turnAndMoveTo(GPS_prepareForCenterDump, speed_normal, Forward);
		if(DOLIFT2) lowerTallArm();

		if (DEBUG) wait1Msec(2000);
		//else wait1Msec(200);

		if(GRAB_GOAL){
			turnAndMoveTo(GPS_prepareForKickstand, speed_normal, Backward);
			wait1Msec(100);
			turnAndMoveTo(GPS_mediumGoalPosition, speed_normal, Backward);
			grabGoal();
		}
		else{
			turnAndMoveTo(GPS_prepareForKickstand, speed_normal);

			if (DEBUG) wait1Msec(2000);
			else wait1Msec(200);

			turnAndMoveTo(GPS_hitKickstand, speed_normal);
		}
	}
	else writeDebugStreamLine("Detection of center structure failed in unexpected way.");
}

task main()
{
	initializeRobot();

	waitForStart(); // Wait for the beginning of autonomous phase.

	int startTime = nPgmTime;
	resetTracker();
	//Floor start tracking positions
	robot.x = GPS_floorStartingPositionUS.x;
	robot.y = GPS_floorStartingPositionUS.y;
	robot.theta = GPS_floorStartingPositionUS.theta;
	StartTask(trackRobot);

	floorStart();

	writeDebugStreamLine("Autonomous completed in %.2f seconds", (nPgmTime-startTime)/1000.0);
}
