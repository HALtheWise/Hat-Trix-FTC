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
#include "ButtonJoyDriver.h"

//__________________________________________________________________
/*

#define  2
#define  3
#define  4
#define  5
#define  6
#define  7
#define  8
#define  9

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
#define ACTION_DRIVE 0
#define ACTION_TRIGHT 1
#define ACTION_TLEFT 2
//#define /*NAME OF ACTION*/ 3  //JOY1_YELLOW
//#define /*NAME OF ACTION*/ 4  //JOY1_LBUMP
//#define /*NAME OF ACTION*/ 5  //JOY1_RBUMP
//#define /*NAME OF ACTION*/ 6  //JOY1_LTRIG
//#define /*NAME OF ACTION*/ 7  //JOY1_RTRIG
//#define /*NAME OF ACTION*/ 8  //JOY1_BACK
//#define /*NAME OF ACTION*/ 9  //JOY1_START

//#define /*NAME OF ACTION*/ 10  //JOY1_BLUE
//#define /*NAME OF ACTION*/ 11  //JOY1_BLUE
//#define /*NAME OF ACTION*/ 12  //JOY1_BLUE
//#define /*NAME OF ACTION*/ 13  //JOY1_BLUE
//#define /*NAME OF ACTION*/ 14  //JOY1_BLUE
//#define /*NAME OF ACTION*/ 15  //JOY1_BLUE
//#define /*NAME OF ACTION*/ 16  //JOY1_BLUE
//#define /*NAME OF ACTION*/ 17  //JOY1_BLUE
//#define /*NAME OF ACTION*/ 18  //JOY1_BLUE
//#define /*NAME OF ACTION*/ 19  //JOY1_BLUE

int aState[20];
int aStep[20];

#define AI_NONE 0
#define AI_DRIVE 1
#define AI_RTURN 2
#define AI_LTURN 3

#define MAX_RES 4

#define RES_WHEELS 0
#define RES_WRIST 1
#define RES_ARM 2
#define RES_WINCH 3

int res_owner[MAX_RES];

typedef enumWord{ C_EVENT, C_ABORT, C_RELEASE} Command;

typedef struct
{
	int boss;
	int power;
}OodMotor;

typedef struct
{
	OodMotor motX;
	OodMotor motY;
} Mots;

Mots motors;
//typedef enumWord{ACTION_DRIVE, ACTION_TRIGHT, ACTION_TLEFT} ACTION;

typedef enumWord{ A_ACTIVE, A_INACTIVE, A_DONUT} ASTATE;


void requestStateChange(int action, ASTATE newState)
{
}

int get_owner( int res_id )
{
	if( res_id >= 0 && res_id < MAX_RES )
		return res_owner[ res_id];
	else
		return AI_NONE;
}

void command_all( Command cmd, EventList *eList, int res_id )
{
	actionDrive(Command cmd, EventList *eList, int res_id);
	actionTRight(Command cmd, EventList *eList, int res_id);
	actionTLeft(Command cmd, EventList *eList, int res_id);
}


void actionTRight(Command cmd, EventList *eList, int res_id)
{
	// switch cmd

}

void actionDrive(EventList *eList)
{
	if(eList->bevents[me].state == pressed/held)
	{
		if(motors->motX.parent == me)
			...... (change the power)
	else if(me take priority&/delay reached)
		...... (change stuff)
	}

}


void processAll(EventList *eList)
{
	*all the methods*(&eList);

	//motor X
	//if(bevents[ACTION_DRIVE]
	//	||aState[ACTION_DRIVE]==(A_ACTIVE||A_DONUT))
	//actionDrive();
	//if(bevents[ACTION_TRIGHT]
	//	||AState[ACTION_TRIGHT]==(A_ACTIVE||A_DONUT))
	//motorX = actionTRight(/*X power*/);
	//if(bevents[ACTION_TLEFT]
	//	||AState[ACTION_TLEFT]==(A_ACTIVE||A_DONUT))
	//motorX = actionTLeft(/*X power*/);


	////motor Y
	//if(bevents[ACTION_DRIVE]
	//	||aState[ACTION_DRIVE]==(A_ACTIVE||A_DONUT))
	//motorY = actionDrive(/*Y power*/);

	//if(bevents[ACTION_TRIGHT]
	//	||aState[ACTION_TRIGHT]==(A_ACTIVE||A_DONUT))
	//motorY = actionTRight(/*Y power*/);

	//if(bevents[ACTION_TLEFT]
	//	||aState[ACTION_TLEFT]==(A_ACTIVE||A_DONUT))
	//motorY = actionTLeft(/*Y power*/);
}

task main()
{
	EventList eList;

	clearDebugStream();
	ClearTimer(T1);
	ClearTimer(T4);

	while(1)
	{
		getEvents(&eList);
		processAll(&eList);
		processDrive(&eList);

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