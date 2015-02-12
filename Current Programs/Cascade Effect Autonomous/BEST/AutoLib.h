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

#include "drivers/hitechnic-irseeker-v2.h"
#include "GyroStuff.h"

//---------------- Constants ----------------//
#define DIST_SCALAR 3125

//------------- Global Variables ------------//


//---------- Function Declarations ----------//
void	mot	(int leftPow, int rightPow);
int  move	(float dist, float power, bool hold = true, bool glide = false, bool stopAtEnd = true);
void turn	(float deg, float power);
void Stop	(bool hold);
void liftFirstStage();
void liftTallArm ();
void lowerTallArm();
void dumpBalls(bool fastMode = false);
void hitKickstand();


//----------- Function Definitions ----------//

void dumpBalls(bool fastMode){
	const int DUMPING_POS = 130;
	const int HOLDING_POS = 55;

	servo[dropperServo] = DUMPING_POS;
	if(!fastMode){
		wait1Msec(4000);
		servo[dropperServo] = HOLDING_POS;
	}
	//wait1Msec(10000);
}

void grabGoal(){
	servo[grabberServo] = 255;
	wait1Msec(750);
	servo[grabberServo] = 127;
}

void liftFirstStage() {
	const int TIMEOUT = 7000;
	const int LIFT_HEIGHT = -10900;

	nMotorEncoder[elevator] = 0;
	ClearTimer(T1);
	while(nMotorEncoder[elevator] > LIFT_HEIGHT && time1[T1] < TIMEOUT){
		motor[elevator] = -100;
	}
	motor[elevator] = 0;

	ClearTimer(T1);
	while(nMotorEncoder[elevator] < LIFT_HEIGHT+1000 && time1[T1] < TIMEOUT){
		motor[elevator] = 100;
	}
	motor[elevator] = 0;

	if (time1[T1] >= TIMEOUT) writeDebugStreamLine("Lifting arm timed out after travelling %d", nMotorEncoder[elevator]);
}

void liftTallArm() {
	const int TIMEOUT = 4000;
	const int LIFT_HEIGHT = 3800;

	nMotorEncoder[car] = 0;
	ClearTimer(T1);
	while(abs(nMotorEncoder[car]) < LIFT_HEIGHT && time1[T1] < TIMEOUT){
		motor[car] = -90;
	}
	motor[car] = 0;
	if (time1[T1] >= TIMEOUT) writeDebugStreamLine("Lifting arm timed out after travelling %d", nMotorEncoder[car]);
}

void lowerTallArm() {
	const int TIMEOUT = 4000;

	ClearTimer(T1);
	while(nMotorEncoder[car] < 0 && time1[T1] < TIMEOUT){
		motor[car] = 30;
	}
	motor[car] = 0;
	if (time1[T1] >= TIMEOUT) writeDebugStreamLine("Lifting arm timed out after travelling %d", nMotorEncoder[car]);
}

void mot(int leftPow, int rightPow) //Takes left an right powers, applies them to wheels
{
	motor[FrontL] = leftPow;
	motor[BackL] = leftPow;

	motor[FrontR] = rightPow;
	motor[BackR] = rightPow;
}

void hitKickstand()
{
	turn(90, 75);
	move(-18, 75);
	turn(70, 75);
	move(80, 75);
}

//********************************************************************************************************
//*
//*	move( float dist, float power , bool hold, bool glide)
//*	is a move function that moves 'dist' at 'power' either abruptly, or 'glide'
//*
//********************************************************************************************************

int move( float dist, float power , bool hold, bool glide, bool stopAtEnd)
{
	float deg = (dist * DIST_SCALAR)/100; // scale from cm to motor rotations
	int mult;
	int lastTime, lastDegree;
	int lastRightDegree = 0;
	int lastLeftDegree = 0;
	float leftPower = power;
	float rightPower= power;
	float EDifference = 0;

	if( dist > 0 ) // account for backwards or forwards
		mult = 1;
	else
		mult = -1;

	float timelimit = (abs(dist)/(power))*50; // set a time limit based on distance

	nMotorEncoder[FrontR] = 0;
	nMotorEncoder[BackL] = 0;
	ClearTimer(T1);
	lastTime = time100[T1];
	lastRightDegree = nMotorEncoder[FrontR];
	lastLeftDegree = nMotorEncoder[BackL];

	if(glide == false)
	{
		while(abs(nMotorEncoder[FrontR]) < abs(deg))
		{
			if(time100[T1] > timelimit) // timeout if stalled
			{
				writeDebugStreamLine("timed out");
				Stop(hold);
				return 1;
			}


			if(lastRightDegree != nMotorEncoder[FrontR])
			{
				/*//straightening code
				EDifference = (nMotorEncoder[FrontR] - nMotorEncoder[BackL]);
				if(EDifference > 0) //Right is faster
				{
				leftPower *= (EDifference/nMotorEncoder[BackL]);
				}
				if(EDifference < 0) //Left is faster
				{
				rightPower *= (EDifference/nMotorEncoder[FrontR]);
				}
				//end straight code*/

				lastTime = time100[T1];
				lastRightDegree = nMotorEncoder[FrontR];
			}
			else if(time100[T1] - lastTime >= 5)
			{
				writeDebugStreamLine("encoder problem");
				Stop(hold);
				return 1;
			}
			//writeDebugStreamLine(" encoder value: %d" , nMotorEncoder[FrontR]);
			mot(leftPower*mult, rightPower*mult);

		}
		if(stopAtEnd)
		{
			Stop(false);
		}
		return 0;
	}
	else
	{
		move(dist/3 , power , hold, false, false);
		move(dist/3 , power/2, hold, false, false);
		move(dist/3 , power/3, hold, false, true);
		return 0;
	}


}


//************************************************************************
//*
//*	turn(float deg, float power) turns the robot 'deg' degrees at 'power'
//*	uses the gyro sensor
//*
//************************************************************************

void turn( float deg, float power )
{
	resetGyro();
	if( deg > 0 )
		power = -1 * power;
	ClearTimer(T1);
	while( abs(gyroVal) < abs(deg)) // turn until the gyro is correct
	{
		if(time1[T1] > 3000) // timeout if stalled
			Stop(1);
		mot(-power, power);
		updateGyro();
		//writeDebugStreamLine("gyro is reading %f" , gyroVal);
	}

	mot(0, 0);
}

void turnFromLWall( float deg, float power ) //only runs left side to turn away from a wall
{
	resetGyro();
	if( deg > 0 )
		power = -1 * power;
	ClearTimer(T1);
	while( abs(gyroVal) < abs(deg)) // turn until the gyro is correct
	{
		if(time1[T1] > 6000) // timeout if stalled
			Stop(1);
		mot(-power, 0);
		updateGyro();
		//writeDebugStreamLine("gyro is reading %f" , gyroVal);
	}

	mot(0, 0);
}


//************************************************************
//*
//* Stops the robot's actions and can prevent further movement
//*
//************************************************************


void Stop(bool hold)
{
	mot(0, 0);

	if(hold)
	{
		writeDebugStreamLine("Stopped");
		PlayTone(440, 25);
		while(1)
		{
			mot(0, 0);
		}
	}
}