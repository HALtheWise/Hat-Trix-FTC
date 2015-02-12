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

#include "JoystickDriver.c"

typedef enumWord{ B_PRSD, B_RLSD} BSTATE;

typedef struct
{
	BSTATE   state;
	//ACTION	act;
	int keyID;
} BEvent;

BEvent bevents[24];

int eventCnt = 0;

int prevBtns[20];



void addEvent(int keyID, BSTATE state)//ACTION act
{

	bevents[eventCnt].keyID = keyID;
	bevents[eventCnt].state = state;

	eventCnt++;
}



void getEvents()
{
	getJoystickSettings(joystick);
	eventCnt=0;
	for(int i = 0; i < 10; i++)
	{
		if(prevBtns[i] != joy1Btn(i+1))
		{
		addEvent(i, joy1Btn(i+1) ? B_PRSD : B_RLSD);
			prevBtns[i] = joy1Btn(i+1);
		}
	}

	for(int i = 0; i < 10; i++)
	{
		if(prevBtns[i+10] != joy2Btn(i+1))
		{
		addEvent(i+10, joy2Btn(i+1) ? B_PRSD : B_RLSD);
			prevBtns[i+10] = joy2Btn(i+1);
		}
	}
}
