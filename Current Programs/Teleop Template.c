#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S2,     thing1,         sensorLightActive)
#pragma config(Sensor, S3,     thing2,         sensorLightActive)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     FL,            tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     FR,            tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     SpinnyDu,      tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     Arm,           tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     BL,            tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     BR,            tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C4_1,    Lux,                  tServoStandard)
#pragma config(Servo,  srvo_S1_C4_2,    TheThing,             tServoStandard)
#pragma config(Servo,  srvo_S1_C4_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "TheGreatLibrary.c"
#include "ButtonDriver.h"

//__________________________________________________________________
/*
#define JOY1_BLUE 0
#define JOY1_GREEN 1
#define JOY1_RED 2
#define JOY1_YELLOW 3
#define JOY1_LBUMP 4
#define JOY1_RBUMP 5
#define JOY1_LTRIG 6
#define JOY1_RTRIG 7
#define JOY1_BACK 8
#define JOY1_START 9

#define JOY2_BLUE 10
#define JOY2_GREEN 11
#define JOY2_RED 12
#define JOY2_YELLOW 13
#define JOY2_LBUMP 14
#define JOY2_RBUMP 15
#define JOY2_LTRIG 16
#define JOY2_RTRIG 17
#define JOY2_BACK 18
#define JOY2_START 19
*/
//__________________________________________________________________

int AState[20];
int AStep[20];


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
			//case *name-of-button* :
			//*actions* ;
			//break;
			};
		}



		//_________________________________________________________________________



	}
}
