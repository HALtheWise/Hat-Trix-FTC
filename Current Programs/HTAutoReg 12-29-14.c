#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Sensor, S2,     seeker,         sensorI2CCustom)
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
#include "drivers/hitechnic-irseeker-v2.h"
#define DIST_SCALAR 3125



int degreesToBasket(float heading);
int gyroOffset = 594;			// set default value
int count = 0;
int basketNumber = 0;

typedef struct
{
	int dir[5];
	int str[5];
	int d;
} Signls;

int insert(Signls *sigList, int val, int direction);

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

int kickSeeker(Signls *sigList)
{
	int _dirDC = 0;
	int _dirAC = 0;
	int dcS1, dcS2, dcS3, dcS4, dcS5 = 0;
	int acS1, acS2, acS3, acS4, acS5 = 0;

	for(int i = 0; i < 5; i++)
	{
		sigList->str[i] = -1;
		sigList->dir[i] = -1;
	}

	// set the DSP to the new mode
	if ( ! HTIRS2setDSPMode(seeker, DSP_1200))
		return -1; // Sensor initialized

	// Read the current non modulated signal direction
	_dirDC = HTIRS2readDCDir(seeker);
	if (_dirDC < 0)
		return -1; // I2C read error occurred

	// read the current modulated signal direction
	_dirAC = HTIRS2readACDir(seeker);
	if (_dirAC < 0)
		return -1; // I2C read error occurred

	// Read the individual signal strengths of the internal sensors
	// Do this for both unmodulated (DC) and modulated signals (AC)
	if (!HTIRS2readAllDCStrength(seeker, dcS1, dcS2, dcS3, dcS4, dcS5))
		return -1; // I2C read error occurred
	if (!HTIRS2readAllACStrength(seeker, acS1, acS2, acS3, acS4, acS5 ))
		return -1; // I2C read error occurred

	insert(sigList, acS1, 0);
	insert(sigList, acS2, 1);
	insert(sigList, acS3, 2);
	insert(sigList, acS4, 3);
	insert(sigList, acS5, 4);


	count ++;
	writeDebugStreamLine("D %d %d", count, _dirAC);
	//writeDebugStreamLine("0 %d %d", dcS1, acS1);
	//writeDebugStreamLine("1 %d %d", dcS2, acS2);
	//writeDebugStreamLine("2 %d %d", dcS3, acS3);
	//writeDebugStreamLine("3 %d %d", dcS4, acS4);
	//writeDebugStreamLine("4 %d %d", dcS5, acS5);

	sigList->d = _dirAC;

	if(_dirAC == 2)// && acS2 < 50 && acS4 < 50) // make sure that the beacon is right in front of the sensor
	{
		static int debugPrinted = 0;
		PlayTone(440<<2, 10);
		if (!debugPrinted){
			debugPrinted = true;
			writeDebugStreamLine("Played sound at %d degrees", gyroVal);
			//basketNumber = degreesToBasket(gyroVal);
		}
		return 1;
	}

	else if(_dirAC == 3)
		return 2;
	else if(_dirAC == 4)
	{
		PlayTone(350<<2, 10);
		return 3;
	}

	return 1;

}

int move( float dist, float power , int hold, int glide, int ir, Signls *sigList)  //initially called with (-210 , 80, 1, 0)
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
			//writeDebugStreamLine(" encoder value: %d" , nMotorEncoder[FrontR]);
			if(!ir)
			{
				motor[FrontL] = power * mult;
				motor[BackL] = power * mult;
				motor[FrontR] = power * mult;
				motor[BackR] = power * mult;
			}
			else
			{
				kickSeeker(sigList);

				float ang = 45*(sigList->dir[0]-2);

				float lFrG = (sigList->str[0]*sin(ang))*3/5;
				float fWbK = (sigList->str[0]*cos(ang))*3/5;

				//float irChn = sigList->str[0]*sin(45*(5-sigList->dir[0]));
				//irChn/=1000;
				int m = 1;
				if(ang < 0)
					m = -1;


				int n = 1;
				if(ang > 0)
					n = -1;

				writeDebugStreamLine("ANG  =  %d", ang);
				writeDebugStreamLine("lFrG   =   %d", lFrG);
				writeDebugStreamLine("rightPOW  =  %d", (power*mult)+(n*lFrG));
				writeDebugStreamLine("********** left POW  =  %d", (power*mult)+(m*lFrG));

				writeDebugStreamLine("strength   STR[%d, %d, %d, %d, %d]", sigList->str[0], sigList->str[1],
				sigList->str[2], sigList->str[3], sigList->str[4]);
				writeDebugStreamLine("direction   DIR[%d, %d, %d, %d, %d]", sigList->dir[0], sigList->dir[1],
				sigList->dir[2], sigList->dir[3], sigList->dir[4]);

				motor[FrontL] = (power * mult)+(m*lFrG);
				motor[BackL] = (power * mult)+(m*lFrG);
				motor[FrontR] = (power * mult)+(n*lFrG);
				motor[BackR] = (power * mult)+(n*lFrG);
			}

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
		move(dist/3 , power , 1 , -1, ir, sigList);
		move(dist/3 , power/2 , 1 , -1, ir, sigList);
		move(dist/3 , power/3 , 1, 0, ir, sigList);
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
	if( time1[T2] > 2 )
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

/*void Oldturn( int deg, int power )
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
}*/

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




int degreesToBasket(float heading){
	if (heading < 14) return 3;
	else if (heading < 34) return 2;
	else if (heading < 56) return 1;
	return 0;
}

int insert(Signls *sigList, int val, int direction)
{
	int returned = -1;
	for(int i = 4; i>=0; i--)
	{
		if(sigList->str[i] < val)
		{
			if(i == 4)
			{
				sigList->str[i] = val;
				sigList->dir[i] = direction;
				returned = i;
			}
			else
			{
				sigList->str[i+1] = sigList->str[i];
				sigList->str[i] = val;

				sigList->dir[i+1] = sigList->dir[i];
				sigList->dir[i] = direction;
				returned = i;
			}
		}
		else
			return returned;
	}
	return returned;
}


void test(Signls *sigList)
{
	int count = 0;
	while(true)
	{
		count++;
		wait10Msec(1);
		kickSeeker(sigList);

		float ang = 30*sigList->dir[0]-5;
		writeDebugStreamLine("ang=%d",ang);
		float lFrG = (sigList->str[0]*sin(ang))/1000;
		float fWbK = (sigList->str[0]*cos(ang))/1000;

		float irChn = sigList->str[0]*sin(45*(5-sigList->dir[0]));
		irChn/=1000;

		writeDebugStreamLine("%d",count);
		//writeDebugStreamLine("The 'LFRG' = %d",lFrG);
		//writeDebugStreamLine("The 'FWBK' = %d",fWbK);
		writeDebugStreamLine("strength   STR[%d, %d, %d, %d, %d]", sigList->str[0], sigList->str[1],
		sigList->str[2], sigList->str[3], sigList->str[4]);
		writeDebugStreamLine("direction   DIR[%d, %d, %d, %d, %d]", sigList->dir[0], sigList->dir[1],
		sigList->dir[2], sigList->dir[3], sigList->dir[4]);
	}
}


void onFloor(Signls *sigList)
{
	move(40, 40, 1, 0, 0, sigList);
	kickSeeker(sigList);
	writeDebugStreamLine("1st   STR[%d, %d, %d, %d, %d]", sigList->str[0], sigList->str[1],
	sigList->str[2], sigList->str[3], sigList->str[4]);
	writeDebugStreamLine("1st   DIR[%d, %d, %d, %d, %d]", sigList->dir[0], sigList->dir[1],
	sigList->dir[2], sigList->dir[3], sigList->dir[4]);

	int po = -1;
	float ang = 0;
	if(sigList->dir[0] == 3 && sigList->dir[1] == 2)
		po = 2;
	else
	{
		move(40, 30, 1, 0,0, sigList);
		wait10Msec(100);
		kickSeeker(sigList);
		writeDebugStreamLine("2nd   STR[%d, %d, %d, %d, %d]", sigList->str[0], sigList->str[1],
		sigList->str[2], sigList->str[3], sigList->str[4]);
		writeDebugStreamLine("2nd   DIR[%d, %d, %d, %d, %d]", sigList->dir[0], sigList->dir[1],
		sigList->dir[2], sigList->dir[3], sigList->dir[4]);


		if(sigList->dir[0] == 2 && sigList->str[0] > 100)
			po = 1;
		else if(sigList->dir[0] == 2 && sigList->str[0] <= 100)
			po = 0;
		else
			writeDebugStreamLine("center piece position could not be determined");
	}


	if(po < 0)
		Stop(0);

	writeDebugStreamLine("position=%d",po);


	if(po == 0)
	{
		move(37, 30, 1, 0, 0, sigList);
		ang = 75/(1+1/10);
		turn(-ang, 25);
		move(50,50, 1, 0, 0, sigList);
	}
	else if(po == 1)
	{
		ang = 45/(1+1/10);
		turn(-ang, 25);
		move(30, 30, 1, 0, 0, sigList);
		ang = 12/(1+1/10);
		turn(-ang, 25);
		move(50,50, 1, 0, 0, sigList);
	}
	else if(po == 2)
	{
	}
	/*int direc = sigList->dir[0];
	if(direc > 2)
	{
	writeDebugStreamLine("the sensor says strongest is dir 3 or 4 in onFloor");
	if(sigList->dir[1] == 0)
	direc = 0;
	else if(sigList->dir[1] == 1)
	direc = 2;
	else
	{
	ClearTimer(T3);
	move(20, 25, 1, 0, 0, sigList);
	kickSeeker(sigList);
	direc = sigList->dir[0];
	while(direc > 2)
	{
	writeDebugStreamLine("direction   DIR[%d, %d, %d, %d, %d]", sigList->dir[0], sigList->dir[1],
	sigList->dir[2], sigList->dir[3], sigList->dir[4]);
	turn(-27, 25);
	if(time10(T3) > 500)
	{
	PlayTone(200<<2, 20);
	writeDebugStreamLine("took TOO LONG to find the right ir beac");
	Stop(0);
	}
	kickSeeker(sigList);
	direc = sigList->dir[0];
	}
	}
	}

	float ang = ((direc-2)*30)/(1+1/10);
	writeDebugStreamLine("ANGonFloor  =  %d,		&&	 DIR  =  %d",ang,direc);
	turn(-ang, 25);
	move(80, 30, 1, 0, 0, sigList);
	ang = (((direc-2)*30)+82)/(1+1/10);
	turn(-ang, 25);
	move(50,50, 1, 0, 0, sigList);
	*/
}


task main()
{

	gyroCal();
	waitForStart();
	clearDebugStream();
	writeDebugStreamLine("starting!");
	nMotorEncoder[FrontR] = 0;
	nMotorEncoder[grabber1] = 0;
	nMotorEncoder[grabber2] = 0;
	static int loopNum = 0;


	Signls sigList;

	for(int i = 0; i < 5; i++)
	{
		sigList.str[i] = -1;
		sigList.dir[i] = -1;
	}
	sigList.d = -1;

	move(40, 30, 1, 0, 0, sigList);
	kickSeeker(&sigList);
	test(&sigList);
	//onFloor(&sigList);


	//move(150, 40, 1, 0, 0, &sigList);

	/*if(beac == -1)
	{
	writeDebugStreamLine("This no good");
	Stop(0);
	}
	else if(beac == 1)
	{
	writeDebugStreamLine("orientation 1 is running");
	turn(-41, 25);
	move(50, 30, 1, 0);
	turn(82, 25);
	move (40, 40, 1, 0);
	}
	else if(beac == 2)
	{
	writeDebugStreamLine("orientation 2 is running");
	turn(-19, 25);
	move(60, 30, 1, 0);
	turn(30, 25);
	move(40, 40, 1, 0);
	}
	else if(beac == 3)
	{
	writeDebugStreamLine("orientation 3 is running");
	move(60, 30, 1, 0);
	turn(-36, 25);
	move(40, 40, 1, 0);
	}
	else
	{
	writeDebugStreamLine("the sensor thought it was in region %d", beac);
	}*/


}