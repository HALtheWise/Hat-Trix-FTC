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

#include "JoystickDriver.c"

#include "AutoLib.h"


int count = 0;

typedef struct
{
	int dir[5];
	int str[5];
	int d;
} Signls;

typedef struct
{
	int centerPos;
} World;

World world;

int insert(Signls *sigList, int val, int direction);

//***************************************************************
//*
//*	kickSeeker(Signls *sigList) updates sigList's array values
//*	with the most recent IR sensor values.
//*
//*	returns -1 if an error with the sensor occurs during reading
//*
//***************************************************************

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

	sigList->d = _dirAC;

	if(_dirAC == 2)
	{
		static int debugPrinted = 0;
		PlayTone(440<<2, 10);
		if (!debugPrinted){
			debugPrinted = true;
		}
		return 1;
	}

	else if(_dirAC == 3)
		return 2;
	else if(_dirAC == 4)
	{
		PlayTone(350<<2, 10);
		return 3;


		return 1;

	}
	return 1;
}

//**************************************************************************************************
//*
//*	insert(Signls *sigList, int val, int direction) inserts 'val' and 'direction' into the parallel
//*	arrays sigList->str[] and sigList->dir[], which are ordered highest to lowest strength
//*
//**************************************************************************************************

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



//********************************************************************************
//*
//*	onFloor(Signls *sigList) is the autonomous function for kicking the kickstand
//*
//*	uses the kickSeeker function
//*
//********************************************************************************

int juliet(Signls *sigList);

void onFloor(Signls *sigList)
{
	servo[dropperServo] = 55;
	liftFirstStage();
	move(80, 30, 1, 0);
	turn(15, 30);

	float ang=0;

	world.centerPos = juliet(sigList);

	writeDebugStreamLine("position=%d", world.centerPos);

	if (DEBUG) wait1Msec(500);

	if(world.centerPos == 1)
	{
		turn(-50, 35);

		move(75, 35);

		turn(-55, 35);
		wait1Msec(250);
		liftTallArm();

		move(-24, 30);
		wait1Msec(1000);
		dumpBalls();
		move(15, 30);
		lowerTallArm();


	}
	else if(world.centerPos == 2)
	{
		turn(-70, 35);
		move(19, 35);
		turn(-65, 35);
		liftTallArm();

		move(-20, 20);
		dumpBalls();
		move(15, 30);
		lowerTallArm();
		if (DEBUG) wait1Msec(500);
		//hitKickstand();
	}
	else if(world.centerPos == 3)
	{
		turn(70, 35);
		move(23, 35);
		turn(85, 40);
		liftTallArm();
		move(-13, 20);
		wait1Msec(100);
		dumpBalls();
		move(15, 30);
		lowerTallArm();


	}
	else writeDebugStreamLine("Detection of center structure failed in unexpected way.");
}
/*void onRamp(Signls *sigList)
{
float son = SensorValue(sonar);
move(70, 30, 1, 0, 0, sigList, 0);
if(true){}

}*/



/*void testSonar()
{
int count = 0;
while(true)
{
wait10Msec(10);
count++;
writeDebugStreamLine("SonarSensor		=		%d",SensorValue(sonar));
writeDebugStreamLine("count=%d",count);
}
}*/


//******************************************************************************************
//*
//*	testIr(Signls *sigList) activelly prints out the values of the ir sensor, and the updated
//*	forms of the parallel arrays sigList->dir[] and sigList->str[]
//*
//******************************************************************************************

void testIr(Signls *sigList)
{
	const int IRthresh = 25;
	int count = 0;
	while(true)
	{
		count++;
		wait1Msec(100);
		kickSeeker(sigList);

		float ang = 30*sigList->dir[0]-5;
		writeDebugStreamLine("ang=%d",ang);
		float lFrG = (sigList->str[0]*sin(ang))/1000;
		float fWbK = (sigList->str[0]*cos(ang))/1000;

		float irChn = sigList->str[0]*sin(45*(5-sigList->dir[0]));
		irChn/=1000;

		writeDebugStreamLine("%d",count);
		writeDebugStreamLine("strength   STR[%d, %d, %d, %d, %d]", sigList->str[0], sigList->str[1],
		sigList->str[2], sigList->str[3], sigList->str[4]);
		writeDebugStreamLine("direction   DIR[%d, %d, %d, %d, %d]", sigList->dir[0], sigList->dir[1],
		sigList->dir[2], sigList->dir[3], sigList->dir[4]);


		if(sigList->str[0]>IRthresh && sigList->dir[0] == 2 && sigList->str[1]<IRthresh){
			writeDebugStreamLine("DETECTED POSITION 1");
			} else if(sigList->str[0]>IRthresh && sigList->dir[0] == 1 && sigList->str[1]<IRthresh){
			writeDebugStreamLine("DETECTED POSITION 3");
			} else{

			writeDebugStreamLine("DETECTED POSITION 2");
		}

	}
}

int juliet(Signls *sigList){

	const int IRthresh = 25;
	kickSeeker(sigList);

	if(sigList->str[0]>IRthresh && sigList->dir[0] == 2 && sigList->str[1]<IRthresh){
		writeDebugStreamLine("DETECTED POSITION 1");
		return 1;
	}
	else if(sigList->str[0]>IRthresh && sigList->dir[0] == 1 && sigList->str[1]<IRthresh){
		writeDebugStreamLine("DETECTED POSITION 3");
		return 3;
	}
	else{
		writeDebugStreamLine("DETECTED POSITION 2");
		return 2;
	}
}

/*void score()
{
ClearTimer(T1);
float deg = (dist * DIST_SCALAR)/100; // scale from cm to motor rotations

}*/


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

	world.centerPos = 0;


	Signls sigList;

	for(int i = 0; i < 5; i++)
	{
		sigList.str[i] = -1;
		sigList.dir[i] = -1;
	}
	sigList.d = -1;

	onFloor(&sigList);
}
