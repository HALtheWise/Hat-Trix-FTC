/*#define JOY1_BLUE 0
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

#include "JoystickDriver.c"

#define JOY_THRESH 10;

typedef enumWord{ B_PRSD, B_RLSD} BSTATE;

typedef struct
{
BSTATE   state;
//ACTION	act;
int keyID;
} BEvent;

int prevBtns[20];

typedef struct
{
short x,y;
} JoyValues;

typedef struct
{
BEvent bevents[24];
int eventCnt = 0;

JoyValues joy1;
JoyValues joy2;
JoyValues joy3;
JoyValues joy4;

} EventList;*/

#include "ButtonJoyDriver.h"
#include "CommandMap.h"
#define JOY_THRESH 20


void addEvent(int keyID, BSTATE state, EventList *eList)
{
	eList->bevents[eList->eventCnt] = commandMap[keyID][state];

	eList->eventCnt++;
}


int threshControl(int power)
{
	if(abs(power)< JOY_THRESH)
		return 0;
	//return power;
	return sgn(power)*(abs(power)-JOY_THRESH)*(128.0/(128-JOY_THRESH));
}


void getEvents(EventList *eList)
{
	getJoystickSettings(joystick);
	eList->eventCnt=0;
	for(int i = 0; i < 10; i++)
	{
		if(prevBtns[i] != joy1Btn(i+1))
		{
		addEvent(i, joy1Btn(i+1) ? B_PRSD : B_RLSD, eList);
			prevBtns[i] = joy1Btn(i+1);
		}
	}

	for(int i = 0; i < 10; i++)
	{
		if(prevBtns[i+10] != joy2Btn(i+1))
		{
		addEvent(i+10, joy2Btn(i+1) ? B_PRSD : B_RLSD, eList);
			prevBtns[i+10] = joy2Btn(i+1);
		}
	}
	eList->joy1.x=threshControl(joystick.joy1_x1);
	eList->joy1.y=threshControl(joystick.joy1_y1);
	eList->joy2.x=threshControl(joystick.joy1_x2);
	eList->joy2.y=threshControl(joystick.joy1_y2);
	eList->joy3.x=threshControl(joystick.joy2_x1);
	eList->joy3.y=threshControl(joystick.joy2_y1);
	eList->joy4.x=threshControl(joystick.joy2_x2);
	eList->joy4.y=threshControl(joystick.joy2_y2);
}
