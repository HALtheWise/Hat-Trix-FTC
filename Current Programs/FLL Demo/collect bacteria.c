#pragma config(Sensor, S1,     light1,         sensorCOLORFULL)
#pragma config(Sensor, S2,     light2,         sensorLightActive)
#pragma config(Sensor, S3,     gyro,           sensorI2CHiTechnicGyro)
#pragma config(Motor,  motorA,          lift,          tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorB,          right,         tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          left,          tmotorNXT, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// This is for the NXT model, not TETRIX

/************************************\
|*  ROBOTC Virtual World            *|
|*                                  *|
|*  DO NOT OVERWRITE THIS FILE      *|
|*  MAKE SURE TO "SAVE AS" INSTEAD  *|
\************************************/

int gyroOffset = 594;			// set default value

float gyroVal = 0.0;
int gyroCal();

float updateGyro();
void resetGyro();
void raise(int power = 30);
void lower();
void turn( int deg, int power, bool ramp = false);
void move( int dist, int power, bool ramp = false); //Move for specified distance, in cm
void tmove( int moveTime, int power ); //Move for specified time, in ms, negative time for backward
void pmove( int dist, int powerB, int powerC );//Move for specified distance, measured on the wheel turning faster, powers > 0
void findLine( int thresh, int power );
void shake( int shakeCnt, int shakeDist, int power );

task main()
{
	gyroCal();
	move(50, 90, true);
	pmove(13, 20, 30);
	wait1Msec(100);
	move(-65, 90, true);

}

int gyroCal()
{
	int sum = 0;

	// Take 50 readings and average them out
	for (int i = 0; i < 50; i++)
	{
		sum += SensorValue(S3);
		writeDebugStreamLine("SensorValue(S3): %d", SensorValue(S3) );
		wait1Msec(5);
	}

	// Store new offset
	gyroOffset = (sum / 50.0);

	ClearTimer(T2);			// reset gyro timer

	// Return new offset value
	return gyroOffset;
}

float updateGyro()
{
	if( time1[T2] > 4 )
	{
		int gVal = SensorValue(S3) - gyroOffset;
		if( motor[left] != 0 || motor[right] != 0 )
			gyroVal += (time1[T2] * gVal) / 1000.0;
		//writeDebugStreamLine( "updateGyro gyroVal: %f gVal: %d Timer: %d", gyroVal, gVal, time1[T2] );
		ClearTimer(T2);														// reset gyro timer
	}
	return gyroVal;
}

void resetGyro()
{
	gyroVal = 0;
	ClearTimer(T2);
}

void raise(int power)
{
	nMotorPIDSpeedCtrl[lift] = mtrNoReg;
	ClearTimer(T1);
	while( time1[T1] < 1500 )
		motor[lift] = -power;

	motor[lift] = 0;
	//nMotorPIDSpeedCtrl[lift] = mtrSpeedReg;
}

void lower()
{
	ClearTimer(T1);
	while( time1[T1] < 1000 )
		motor[lift] = 30;

	motor[lift] = 0;
}

float rampFunc(float fraction, float rampPortion = .2, float min = .3){
	if(fraction	< rampPortion)
		return (1-min)/rampPortion * fraction + min;
	else if(fraction < (1-rampPortion))
		return 1.0;
	else
		return (1-fraction)* (1-min)/rampPortion + min;
}

void turn( int deg, int power, bool ramp)
{
	resetGyro();
	float rampMult = 1.0;
	if( deg < 0 )
		power = -1 * power;

	while( abs(gyroVal) < abs(deg) )
	{
		if(ramp){rampMult = rampFunc(abs(gyroVal)/(float)abs(deg));}
		motor[right] = power * rampMult;
		motor[left] = -1 * power * rampMult;
		updateGyro();
	}

	motor[right] = 0;
	motor[left] = 0;
}

void move( int dist, int power, bool ramp ) //Move for specified distance, in cm
{
	float rampMult = 1.0;
	int deg, mult;
	deg = (dist * 266)/10;
	if( dist > 0 )
		mult = 1;
	else
		mult = -1;

	nMotorEncoder[motorC] = 0;
	while(abs(nMotorEncoder[motorC]) < abs(deg) )
	{
		if(ramp){rampMult = rampFunc(abs(nMotorEncoder[motorC]) /(float) abs(deg));}
		motor[motorC] = power * mult * rampMult;
		motor[motorB] = power * mult * rampMult;
	}

	motor[motorC] = 0;
	motor[motorB] = 0;
}

void tmove( int moveTime, int power ) //Move for specified time, in ms
{
	int mult;
	if( moveTime > 0 )
		mult = 1;
	else
		mult = -1;

	ClearTimer(T1);
	while(time1[T1] < abs(moveTime) )
	{
		motor[motorC] = power * mult;
		motor[motorB] = power * mult;
	}

	motor[motorC] = 0;
	motor[motorB] = 0;
}

void pmove( int dist, int powerB, int powerC ) //Move for specified distance, measured on the wheel turning faster, powers > 0
{
	int deg, mult, enc;
	deg = (dist * 266)/10;
	if( dist > 0 )
		mult = 1;
	else
		mult = -1;

	if( powerB > powerC )
		enc = motorB;
	else
		enc = motorC;
	nMotorEncoder[enc] = 0;
	while(abs(nMotorEncoder[enc] * mult) < abs(deg) )
	{
		motor[motorC] = powerC * mult;
		motor[motorB] = powerB * mult;
	}

	motor[motorC] = 0;
	motor[motorB] = 0;
}

void findLine( int thresh, int power )
{
	int mult;
	if( thresh > 0 )
		mult = 1;
	else
		mult = -1;


	PlayTone(1000, 20);
	while(SensorValue(S2) > abs(thresh) )
	{
		motor[motorC] = power * mult;
		motor[motorB] = power * mult;
	}

	PlayTone(1500, 20);
	motor[motorC] = 0;
	motor[motorB] = 0;
}

void shake( int shakeCnt, int shakeDist, int power )
{
	for( int i; i < shakeCnt; i++ )
	{
		move (-1*shakeDist, power);
		move (shakeDist, power);
	}

}
