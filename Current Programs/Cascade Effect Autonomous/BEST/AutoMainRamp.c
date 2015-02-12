#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Hubs,  S3, HTServo,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     gyro,           sensorI2CHiTechnicGyro)
#pragma config(Sensor, S3,     ,               sensorI2CMuxController)
#pragma config(Sensor, S4,     seeker,         sensorI2CCustom)
#pragma config(Motor,  motorA,          stuffer,       tmotorNXT, openLoop)
#pragma config(Motor,  motorB,          deploySweep,   tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorC,          sweeper,       tmotorNXT, openLoop, reversed)
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

// !! Changes to motor configuration must be copied to all files !! //

#define DEBUG true
#define DO_EXTEND false
#define TEST_IR true

#include "JoystickDriver.c"
#include "drivers/hitechnic-irseeker-v2.h"

#include "AutoLib.h"

int centerPos = -1;

void onRamp()
{

	wait1Msec(1500);
	move(-180, 35);
	//wait1Msec(1000);
	if (DO_EXTEND) liftFirstStage();
	wait1Msec(1000);
	move(-60, 35, true, true); //Glide before goal grab
	wait1Msec(1000);
	grabGoal();
	wait1Msec(1000);
	dumpBalls(true);
	wait1Msec(1000);
	turnFromLWall(-170, 80);
	wait1Msec(1000);
	if (TEST_IR) StartTask(juliet2);
	move(-240, 40);
	if (TEST_IR) {
		StopTask(juliet2);
		writeDebugStreamLine("Tower in positon %d.", centerPos);
		return;
	}
}


task juliet2(){
	const int IR_THRESH = 40;

	HTIRS2setDSPMode(seeker, DSP_1200);
	int distance = 0;
	while(true){
		wait1Msec(3);
		int strengths[5];
		HTIRS2readAllACStrength(seeker, strengths[0], strengths[1], strengths[2], strengths[3], strengths[4]);
		int dir = HTIRS2readACDir(seeker);

		//if (dir != 0){
		if (strengths[3] > IR_THRESH){
			distance = nMotorEncoder[FrontR];
			writeDebugStreamLine("Encoder Count: %d", distance);
			break;
		}
	}
	if(abs(distance) > 6050){
		centerPos = 3;
	}else if(abs(distance > 0){
		centerPos = 1;
	}

}

task main()
{
	gyroCal();
	waitForStart();
	clearDebugStream();
	writeDebugStreamLine("starting!");
	nMotorEncoder[FrontL] = 0;
	nMotorEncoder[FrontR] = 0;
	nMotorEncoder[car] = 0;
	nMotorEncoder[elevator] = 0;
	nMotorEncoder[sweeper] = 0;
	nMotorEncoder[deploySweep] = 0;

	onRamp();
}
