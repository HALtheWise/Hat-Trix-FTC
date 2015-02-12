#pragma config(Hubs,  S1, HTMotor,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     thing1,         sensorLightActive)
#pragma config(Sensor, S3,     thing2,         sensorLightActive)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     fan1,          tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     fan2,          tmotorTetrix, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//#include "TheGreatLibrary.c"
#include "ButtonDriver.h"


task main()
{

	clearDebugStream();
	ClearTimer(T1);
	ClearTimer(T4);

	while(1)
	{
		getEvents();

		for(int i = 0; i<eventCnt;i++)
		{
			switch(bevents[i].keyID){
			case JOY1_GREEN:
				if(bevents[i].state == B_PRSD)
				{
					motor[fan1] = 100;
					motor[fan2] = 100;
				}
				else if(bevents[i].state == B_RLSD)
				{
					motor[fan1] = 0;
					motor[fan2] = 0;
				}
				break;
			case JOY1_BLUE:
				if(bevents[i].state == B_PRSD)
				{
					motor[fan1] = 30;
					motor[fan2] = 30;
				}
				else if(bevents[i].state == B_RLSD)
				{
					motor[fan1] = 0;
					motor[fan2] = 0;
				}
				break;
			};
		}



		//_________________________________________________________________________



	}
}
