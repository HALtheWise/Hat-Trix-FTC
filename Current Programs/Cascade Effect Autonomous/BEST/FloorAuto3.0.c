#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Hubs,  S3, HTServo,  none,     none,     none)
#pragma config(Sensor, S2,     gyro,           sensorI2CHiTechnicGyro)
#pragma config(Sensor, S4,     sonar,          sensorSONAR)
#pragma config(Motor,  motorA,          lateralSweep,  tmotorNXT, openLoop)
#pragma config(Motor,  motorB,          stuffer,       tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorC,          verticalSweep, tmotorNXT, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_1,     FrontL,        tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     BackL,         tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     FrontR,        tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     BackR,         tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     rightRoller,   tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     leftRoller,    tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C4_1,     elevator,      tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C4_2,     car,           tmotorTetrix, openLoop, encoder)
#pragma config(Servo,  srvo_S3_C1_1,    grabberServo,         tServoContinuousRotation)
#pragma config(Servo,  srvo_S3_C1_2,    dropperServo,         tServoStandard)
#pragma config(Servo,  srvo_S3_C1_3,    sweeperServo,         tServoContinuousRotation)
#pragma config(Servo,  srvo_S3_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S3_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S3_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//#include "BALL-E overrides.c" //COMMENT OUT THIS LINE BEFORE RUNNING ON COMPETITION ROBOT

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.
#include "TrackerAutoLib.h"
#include "AutoLib.h"
#include "USstuff.h"
#include "Center Relative Positions.h"
#include "Field Positions.h"
#include "Auto GUI.h"
//#include "IRstuff.c"

AutoMode mode = MODE_MEDIUM_ALWAYS;

#define DOLIFT1 false
#define DOLIFT2 true

void initializeRobot()
{
	initPositions();
	clearDebugStream();
	gyroCal();
	servo[dropperServo] = HOLDING_POS;
	//servo[grabberServo]=???; //We should probably initialize this
	return;
}

void floorStart(){
	const int speed_fast = 70;
	const int speed_normal = 60;
	const int speed_slower = 45;
	const int speed_precise = 35;
	const int inter_move_delay = 100;

	if (mode == MODE_NO_MOVE){
		liftFirstStage(false);
		return;
	}

	centerPos = julietUS();
	//	centerPos = 1; //Override for testing purposes

	if(DOLIFT1) liftFirstStage(true); 	// Lift first stage extension in parallel
		wait1Msec(600);				// Gives time to make it impressive that we're lifting in parallel

		turnAndMoveTo(GPS_awayFromWallUS, speed_normal, Backward);

		wait1Msec(inter_move_delay);

		if(mode == MODE_DEFEND_CENTER_MEDIUM){
			turnAndMoveTo(GPS_defendPoint, speed_normal, Forward);
			wait1Msec(500);
		}

		FieldPos target;
		translate(GPS_prepareForCenterDump, target);
	if(distanceBetween(target, robot) > 50.0){ //If the robot is more than half a meter from "prepare to dump" position
		turnAndMoveTo(GPS_prepareForCenterDump, speed_normal, Backward);
	}

	RelativePos pos120;

	switch (centerPos) //Turn to ball dumping position
	{
		case 1:
		pos120 = GPS_centerDumpPosition1;
		break;

		case 2:
		pos120 = GPS_centerDumpPosition1;
		break;

		case 3:
		pos120 = GPS_centerDumpPosition3;
		break;

		default:
		writeDebugStreamLine("Detection of center structure failed in unexpected way.");
		return;
	}

	navOffset = ROBOT_dumpRelativePos;
	updateTRobot();

	turnTo(pos120, speed_slower, Backward);

	if(DOLIFT1){while(!firstStageIsLifted){wait1Msec(5);}} //Wait until first stage extension has completed
	if(DOLIFT2) liftTallArm();

	moveTo(pos120, speed_precise, Backward);

	wait1Msec(1500); //Stabilize robot
	if(DOLIFT2) dumpBalls();

	navOffset = ROBOT_nullRelativePos;
	updateTRobot();

	turnAndMoveTo(GPS_prepareForCenterDump, speed_normal, Forward);
	if(DOLIFT2) lowerTallArm();

	wait1Msec(inter_move_delay);

	if (mode == MODE_MEDIUM_ALWAYS || mode == MODE_DEFEND_CENTER_MEDIUM){
		if (centerPos == 1 || centerPos == 2){ //Backside navigation
			turnAndMoveTo(GPS_navPoint1, speed_fast);
			}else {//Frontside navigation
				turnAndMoveTo(GPS_prepareForKickstand, speed_normal);
		}
		turnAndMoveTo(GPS_mediumGoalPosition, speed_fast, Backward);
		grabGoal();
	}
	else if(mode == MODE_KICKSTAND_ALWAYS){

		turnAndMoveTo(GPS_prepareForKickstand, speed_normal);
		wait1Msec(inter_move_delay);

		turnAndMoveTo(GPS_hitKickstand, speed_normal);
	}
}

task main()
{
	initializeRobot();
	mode = getAutoMode();
	waitForStart(); // Wait for the beginning of autonomous phase.

	int startTime = nPgmTime;

	resetTracker();
	robot.x = GPS_floorStartingPositionUS.x;
	robot.y = GPS_floorStartingPositionUS.y;
	robot.theta = GPS_floorStartingPositionUS.theta;
	StartTask(trackRobot); //Begin GPS tracking

	floorStart();

	int dt = nPgmTime-startTime;
	writeDebugStreamLine("Autonomous completed in %.2f seconds", (dt)/1000.0);
	wait1Msec(10);
}
