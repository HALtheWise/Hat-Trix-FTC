#include "ButtonJoyDriver.c"

#define SHORT_TUBE 0
#define MEDIUM_TUBE 1
#define TALL_TUBE 2

typedef struct
{
	int centerGoal;
	int tubes[3];
	string notes;
}Aut;

typedef struct
{
	int tubes[3];
}Tel;

typedef struct
{
	int teamName;
	Aut autonomous;
	Tel teleop;
	int defence;
	int speed;
	int size;
} Team;

typedef struct
{
	Team   red1;
	Team red2;
	Team blue1;
	Team blue2;

	int matchType;
} Match;

Match matches[50];
int isPressed( EventList *eList,  int keyID, BSTATE   state )
{
	for( int i=0; i < eList->eventCnt; i++ )
		if( eList->bevents[i].keyID == keyID && eList->bevents[i].state == state ){
		return 1;
	}
	return 0;
}

int getNumber()
{
	int number;
	for(int i = 0; i++; i<4)
	{
		for(int e = 4; e++; e>0)
		{
			while(!isPressed(&eList, JOY1_BLUE, B_RLSD))
			{
				if(isPressed(&eList, JOY1_YELLOW, B_RLSD))

			}
		}
	}
}

int beforeMatch(EventList *eList, int currentMatch)
{
	static int team;
}

int autonomous(

task main()
{
	EventList eList;

	int p = 0; //the period in the match
	while(true)
	{
		getEvents(&eList);
		if(p == 0)
		{
		}
		else if(p == 1)
		{
		}
		else if(p == 2)
		{
		}
		else if(p == 3)
		{
		}
	}

}
