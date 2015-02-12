#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S4,     gyro,           sensorI2CHiTechnicGyro)
#pragma config(Motor,  motorA,          grabber1,      tmotorNXT, openLoop)
#pragma config(Motor,  motorB,          grabber2,      tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     FrontR,        tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     rollers,       tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     BackL,         tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     BackR,         tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     FrontL,        tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     winch,         tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C4_1,     arm,           tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C4_2,     rollers,       tmotorTetrix, openLoop, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"
#define DIST_SCALAR 3125

int gyroOffset = 594;			// set default value

float gyroVal = 0.0;

void Stop(int hold)
{
	motor[FrontL] = 0;
	motor[BackL] = 0;
	motor[FrontR] = 0;
	motor[BackR] = 0;
	writeDebugStreamLine("Stopped");
	PlayTone(440, 25);
	if(hold)
	{
		while(1)
		{
			motor[FrontL] = 0;
			motor[BackL] = 0;
			motor[FrontR] = 0;
			motor[BackR] = 0;
		}
	}
}


int move( float dist, float power , int hold, int glide)  //initially called with (-210 , 80, 1, 0)
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

	if(glide != 1)
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
			writeDebugStreamLine(" encoder value: %d" , nMotorEncoder[FrontR]);
			motor[FrontL] = power * mult;
			motor[BackL] = power * mult;
			motor[FrontR] = power * mult;
			motor[BackR] = power * mult;

		}
		if(glide != -1)
		{
			motor[FrontL] = 0;
			motor[BackL] = 0;
			motor[FrontR] = 0;
			motor[BackR] = 0;
		}
		return 0;
	}
	else
	{
		move(dist/3 , power , 1 , -1);
		move(dist/3 , power/2 , 1 , -1);
		move(dist/3 , power/3 , 1, 0);
		return 0;
	}


}

int gyroCal()
{
	int sum = 0;

	// Take 50 readings and average them out
	for (int i = 0; i < 50; i++)
	{
		sum += SensorValue(S4);
		wait1Msec(5);
	}

	// Store new offset
	gyroOffset = (sum / 50.0);

	ClearTimer(T2);			// reset gyro timer

	// Return new offset value
	return gyroOffset;
}


//*******************************************************
//*
//*	updateGyro() - This routine integrates the gyro value
//*	in order to determine how far the robot has turned.
//*
//*******************************************************

float updateGyro()
{
	if( time1[T2] > 4 )
	{
		int gVal = SensorValue(S4) - gyroOffset;

		if( motor[FrontR] != 0 || motor[BackL] != 0 )
			gyroVal += (time1[T2] * gVal) / 1000.0;

		ClearTimer(T2);	// reset gyro timer
	}
	return gyroVal;
}

//*******************************************************
//*
//*	resetGyro() - This routine reset the gryo turn value.
//*
//*******************************************************

void resetGyro()
{
	gyroVal = 0;
	ClearTimer(T2);
}

void turn( int deg, int power )
{
	resetGyro();
	if( deg > 0 )
		power = -1 * power;
	ClearTimer(T1);
	while( abs(gyroVal) < abs(deg)) // turn until the gyro is correct
	{
		if(time1[T1] > 3000) // timeout if stalled
			Stop(1);
		motor[FrontL] = -power;
		motor[BackL] = -power;
		motor[FrontR] = power;
		motor[BackR] = power;
		updateGyro();
		//writeDebugStreamLine("gyro is reading %f" , gyroVal);
	}

	motor[FrontL] = 0;
	motor[BackL] = 0;
	motor[FrontR] = 0;
	motor[BackR] = 0;
}

task main()
{
	waitForStart();
	nMotorEncoder[FrontR] = 0;
	nMotorEncoder[grabber1] = 0;
	nMotorEncoder[grabber2] = 0;
	move(-260 , 80, 1, 1);

	ClearTimer(T1);
	while(nMotorEncoder[grabber1] > -90 && time1[T1] < 1500)
	{
		if(abs(nMotorEncoder[grabber1]-nMotorEncoder[grabber2]) > 10)
			writeDebugStreamLine("the grabbers aren't moving with each other");
		motor[grabber1] = -30;
		motor[grabber2] = -30;
	}
	motor[grabber1] = 0;
	motor[grabber2] = 0;

	//turn(90 , 30);

	//move(-600, 30);
	/*while(abs(nMotorEncoder[FrontR]) < 2000)
	{
	writeDebugStreamLine(" encoder value: %d" , nMotorEncoder[FrontR]);
	motor[FrontL] = -50;
	motor[BackL] = -50;
	motor[FrontR] = -50;
	motor[BackR] = -50;
	}
	motor[FrontL] = 0;
	motor[BackL] = 0;
	motor[FrontR] = 0;
	motor[BackR] = 0;
	*/

}
