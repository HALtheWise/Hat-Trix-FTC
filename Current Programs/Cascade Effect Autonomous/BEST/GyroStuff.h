#pragma config(Sensor, S2,     gyro,           sensorI2CHiTechnicGyro)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//---------------- Constants ----------------//

//------------- Global Variables ------------//
float gyroVal = 0.0;
float gyroOffset = 594;	// set default value

//---------- Function Declarations ----------//
int gyroCal();
float updateGyro(bool requireMotion = true);
void resetGyro();


//----------- Function Definitions ----------//




//**********************************************************
//*
//*	gyroCal() returns the current offset of the gyro sensor
//*	assumes the gyro has already been initialized
//*  Blocks for at least 250 msec.
//*
//**********************************************************

float gyroCal()
{
	long sum = 0;

	// Take 200 readings and average them out
	for (int i = 0; i < 50; i++)
	{
		sum += SensorValue(S2);
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

float updateGyro(bool requireMotion)
{
	if( time1[T2] > 2 )
	{
		float gVal = SensorValue(S2) - gyroOffset;

		//if(!requireMotion || motor[FrontR] != 0 || motor[FrontL] != 0 )
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
