#pragma config(Sensor, S2,     seeker,         sensorI2CCustom)
#pragma config(Sensor, S4,     gyro,           sensorI2CHiTechnicGyro)
#pragma config(Motor,  motorA,          auxMotor,      tmotorNXT, openLoop)
#pragma config(Motor,  motorB,          FrontL,        tmotorNXT, openLoop, reversed, encoder)
#pragma config(Motor,  motorC,          FrontR,        tmotorNXT, openLoop, reversed)
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
void liftTallArm ();
void lowerTallArm();
void dumpTallBalls();


//----------- Function Definitions ----------//

void dumpTallBalls(){
#ifndef BALLE

	//servo[dumperServo] = ???;
	//wait1Msec(2000);
	//servo[dumperServo] = ???;
	wait1Msec(10000);

#endif
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

	if( dist > 0 ) // account for backwards or forwards
		mult = 1;
	else
		mult = -1;

	float timelimit = (abs(dist)/(power))*50; // set a time limit based on distance

	nMotorEncoder[FrontR] = 0;
	ClearTimer(T1);
	lastTime = time100[T1];
	lastDegree = nMotorEncoder[FrontR];

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

			if(lastDegree != nMotorEncoder[FrontR])
			{
				lastTime = time100[T1];
				lastDegree = nMotorEncoder[FrontR];
			}
			else if(time100[T1] - lastTime >= 5)
			{
				writeDebugStreamLine("encoder problem");
				Stop(hold);
				return 1;
			}
			//writeDebugStreamLine(" encoder value: %d" , nMotorEncoder[FrontR]);
			mot(power*mult, power*mult);

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


//************************************************************
//*
//* Stops the robot's actions and can prevent further movement
//*
//************************************************************


void Stop(bool hold)
{
	mot(0, 0);
	writeDebugStreamLine("Stopped");
	PlayTone(440, 25);
	if(hold)
	{
		while(1)
		{
			mot(0, 0);
		}
	}
}
