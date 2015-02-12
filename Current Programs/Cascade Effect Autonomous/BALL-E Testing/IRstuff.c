#pragma config(Sensor, S2,     seeker,         sensorI2CCustom)

#include "drivers/hitechnic-irseeker-v2.h"

int count;

typedef struct
{
	int dir[5];
	int str[5];
	int d;
} Signls;

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

int juliet(Signls *sigList){

	const int IRthresh = 25;
	kickSeeker(sigList);

	if(sigList->str[0]>IRthresh && sigList->dir[0] == 2 && sigList->str[1]<IRthresh){
		return 1;
	}
	else if(sigList->str[0]>IRthresh && sigList->dir[0] == 1 && sigList->str[1]<IRthresh){
		return 3;
	}
	else{
		return 2;
	}
}

int juliet(){
	Signls sigList;

	for(int i = 0; i < 5; i++)
	{
		sigList.str[i] = -1;
		sigList.dir[i] = -1;
	}
	sigList.d = -1;

	return juliet(&sigList);
}
