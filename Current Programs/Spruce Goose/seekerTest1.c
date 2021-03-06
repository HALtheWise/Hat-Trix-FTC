#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     touch,          sensorTouch)
#pragma config(Sensor, S3,     gyro,           sensorI2CHiTechnicGyro)
#pragma config(Sensor, S4,     seeker,         sensorI2CCustom)
#pragma config(Motor,  motorA,          kicker1,       tmotorNXT, openLoop)
#pragma config(Motor,  motorB,          kicker2,       tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     tailWinch,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     hookWinch,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     arms,          tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     wrist,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     FL,            tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C3_2,     BL,            tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C4_1,     FR,            tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     BR,            tmotorTetrix, openLoop, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "TheGreatLibrary.c"

#define DIST_SCALAR 465

int getSeeker();
int gyroCal();
float updateGyro();
void resetGyro();
int degreesToBasket(float heading);
void move( float dist, float power);
void turn( int deg, int power);
int* getButtons();
void EndDump(int left, int delay);
void Stop();

int count = 0;

int gyroOffset = 594;			// set default value

float gyroVal = 0.0;
int basketNumber = 0;

////////////////////////////////////////////////////////////////////////////// TASK MAIN!!! \(^0^)/

task main()
{
	ClearTimer(T1);
	ClearTimer(T3);
	clearDebugStream();
	nMotorEncoder[arms] = 0;
	nMotorEncoder[wrist] = 0;
	int left = 0;
	int right = 0;
	int delay = 0;
	int endDump = 0;

	gyroCal();
	waitForStart(); //*************
	wait1Msec(250);

	int* switches = getButtons();

	if(switches[3])
		left = 1;
	else
		left = 0;

	if(switches[2])
		delay = 1;
	else
		delay = 0;

	if(switches[1])
		endDump = 1;
	else
		endDump = 0;

	if(switches[0])
		right = 1;
	else
		right = 0;


	if(endDump)
		EndDump(left, delay);

	else
	{
		move(-15, 50);

		while(gyroVal < 90)
		{
			updateGyro();

			motor[FR] = -40;
			motor[BR] = -40;
			motor[FL] = 40;
			motor[BL] = 40;

			if(time1[T3] >= 50)
			{
				getSeeker();
				ClearTimer(T3);
			}
		}
		motor[FR] = 0;
		motor[BR] = 0;
		motor[FL] = 0;
		motor[BL] = 0;

		writeDebugStreamLine("Detected basket = %d", basketNumber + 1);

		int turnArray1[] = {20, 9, -12, -22};//How much we turn for each basket
		wait1Msec(500);

		if(delay)
			wait1Msec(10000);


		writeDebugStreamLine("turning %d degrees", turnArray1[basketNumber]);
		turn(turnArray1[basketNumber], 40);

		waywardSon(2, 0);
		while(!waywardSon(-1, 0)){} //Lift the arms to position "high medium"

		int moveArray1[] = {77, 65, 65, 77};//How much we turn for each basket
		move(moveArray1[basketNumber], 30);

		takeAdump(1,0);
		while(!takeAdump(0,0)){}

		int quickTurnArray[] = {-110, -99, 102, 112};
		int rightTurnArray[] = {-110, -99, -78, -68};
		int  leftTurnArray[] = { 70 ,  81, 102, 112};

		waywardSon(0, 0);
		while(!waywardSon(-1, 0)){}

		if(left)
			turn(leftTurnArray[basketNumber] , 50);
		else if(right)
			turn(rightTurnArray[basketNumber] , 50);
		else
			turn(quickTurnArray[basketNumber] , 50);

		waywardSon(4,0);
		while(!waywardSon(-1, 0)){}

		int quickMoveArray[] = {-35, -50, -50, -35};
		int rightMoveArray[] = {-35, -50, -95, -130};
		int  leftMoveArray[] = {-130, -95, -50, -35};

		if(left)
			move(leftMoveArray[basketNumber] , 100);
		else if(right)
			move(rightMoveArray[basketNumber] , 100);
		else
			move(quickMoveArray[basketNumber] , 100);

		int mult = -1;

		if(basketNumber >= 2)
			mult = 1;

		if(left)
			mult = 1;
		else if(right)
			mult = -1;

		wait1Msec(100);
		turn(mult * 30, 100);
		wait1Msec(100);
		move(-45, 100);
		wait1Msec(100);
		turn(mult * 30, 100);
		wait1Msec(100);
		move(-80, 50);
		wait1Msec(100);
		turn(mult * 85, 100);
		wait1Msec(100);
		move(-85, 100);

	}
}

int getSeeker()
{
	int _dirDC = 0;
	int _dirAC = 0;
	int dcS1, dcS2, dcS3, dcS4, dcS5 = 0;
	int acS1, acS2, acS3, acS4, acS5 = 0;

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
	count ++;
	writeDebugStreamLine("D %d %d", count, _dirAC);
	//writeDebugStreamLine("0 %d %d", dcS1, acS1);
	//writeDebugStreamLine("1 %d %d", dcS2, acS2);
	//writeDebugStreamLine("2 %d %d", dcS3, acS3);
	//writeDebugStreamLine("3 %d %d", dcS4, acS4);
	//writeDebugStreamLine("4 %d %d", dcS5, acS5);

	if(_dirAC == 4)// && acS2 < 50 && acS4 < 50) // make sure that the beacon is right in front of the sensor
	{
		static int debugPrinted = 0;
		PlayTone(440<<2, 10);
		if (!debugPrinted){
			debugPrinted = true;
			writeDebugStreamLine("Played sound at %d degrees", gyroVal);
			basketNumber = degreesToBasket(gyroVal);
		}
		return 1;
	}

	else
		return 0;

}

int degreesToBasket(float heading){
	if (heading < 14) return 3;
	else if (heading < 34) return 2;
	else if (heading < 56) return 1;
	return 0;
}

//*******************************************************
//*	gyroCal() - This routine calcaulate the gyro offset
//*	value.
//*******************************************************

int gyroCal()
{
	int sum = 0;

	// Take 50 readings and average them out
	for (int i = 0; i < 50; i++)
	{
		sum += SensorValue(S3);
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
		int gVal = SensorValue(S3) - gyroOffset;

		if( motor[FL] != 0 || motor[BR] != 0 )
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

//*******************************************************
//*
//*	move() - This routine uses the motor encoder to move the robot
//*	a specified number of centimeters.
//*
//*******************************************************

void move( float dist, float power )
{
	float deg;
	int mult;
	deg = (dist * DIST_SCALAR)/10; // scale from cm to motor rotations
	if( dist > 0 ) // account for backwards or forwards
		mult = 1;
	else
		mult = -1;

	float timelimit = (abs(dist)/(power/2))*10; // set a time limit based on distance

	nMotorEncoder[BR] = 0;
	ClearTimer(T1);
	while(abs(nMotorEncoder[BR]) < abs(deg))
	{
		if(time100[T1] > timelimit) // timeout if stalled
		{
			Stop();
		}

		motor[FL] = power * mult;
		motor[BL] = power * mult;
		motor[FR] = power * mult;
		motor[BR] = power * mult;
	}

	motor[FL] = 0;
	motor[BL] = 0;
	motor[FR] = 0;
	motor[BR] = 0;
}


//*******************************************************
//*
//*	turn() - This routine uses the gyro to turn the robot
//*	a specified number of degrees.
//*
//*******************************************************

void turn( int deg, int power )
{
	resetGyro();
	if( deg > 0 )
		power = -1 * power;
	ClearTimer(T1);
	while( abs(gyroVal) < abs(deg)) // turn until the gyro is correct
	{
		if(time1[T1] > 3000) // timeout if stalled
			Stop();
		motor[FL] = -power;
		motor[BL] = -power;
		motor[FR] = power;
		motor[BR] = power;
		updateGyro();
		//writeDebugStreamLine("gyro is reading %f" , gyroVal);
	}

	motor[FL] = 0;
	motor[BL] = 0;
	motor[FR] = 0;
	motor[BR] = 0;
}

//*******************************************************
//*
//*	getButtons() - This routine polls the touch sensor muliplexor
//*	and retuns ench sensor's value.
//*
//*******************************************************

int* getButtons()
{
	static int buttons[] = {0,0,0,0};

	//Get touch sensor inputs
	long value = 1023-SensorRaw(S2);
	long switches = 339 * value;
	switches /= (1023-value);
	switches += 5;
	switches /= 10;
	if(switches&8)
		buttons[0] = 1;
	else
		buttons[0] = 0;
	if(switches&4)
		buttons[1] = 1;
	else
		buttons[1] = 0;
	if(switches&2)
		buttons[2] = 1;
	else
		buttons[2] = 0;
	if(switches&1)
		buttons[3] = 1;
	else
		buttons[3] = 0;
	return buttons;
}

//*******************************************************
//*
//*	EndDump() - This routine delivers the block in the nearest
//*	goal and drives on the ramp.
//*
//*******************************************************

void EndDump(int left, int delay)
{
	int mult = -1;

	if(left)
		mult = 1;
	else
		mult = -1;

	int boost = 0;
	//wait for other team but go faster
	if(delay)
	{
		wait1Msec(10000);
		boost = 20;
	}

	waywardSon(2, 0);
	while(!waywardSon(-1, 0)){}//Raise the arm


	move(20, 25 + boost); //move to goal

	takeAdump(1,0);
	while(!takeAdump(0,0)){}
	wait1Msec(100);

	move(-15, 40 + boost);//back away

	waywardSon(4,0);
	while(!waywardSon(-1, 0)){}

	//drive onto ramp
	turn(mult * 90, 40 + boost);
	move(-60, 40 + boost);
	turn(mult * 70, 40 + boost);
	move(-30, 40 + boost);
	move(-70, 90);
}

void Stop()
{
	motor[FL] = 0;
	motor[BL] = 0;
	motor[FR] = 0;
	motor[BR] = 0;
	MissionImpossible();
	while(1)
	{
		motor[FL] = 0;
		motor[BL] = 0;
		motor[FR] = 0;
		motor[BR] = 0;
	}
}
