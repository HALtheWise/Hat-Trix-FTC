#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     thing1,         sensorLightActive)
#pragma config(Sensor, S3,     thing2,         sensorLightActive)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     FrontL,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     BackL,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     FrontR,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     BackR,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     Arm,           tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C3_2,     motorI,        tmotorTetrix, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define null (void *)0
#include "ButtonJoyDriver.c"



#define AI_NONE 0
#define AI_DRIVE 1
#define AI_RTURN 2
#define AI_LTURN 3
#define AI_ARM 4

#define MAX_RES 4 //Number of resources being tracked

#define RES_WHEELS 0 //This stuff should probably be an enum
#define RES_WRIST 1
#define RES_ARM 2
#define RES_WINCH 3

int res_owner[MAX_RES];
//int res_power[MAX_RES];

typedef enumWord{ C_EVENT, C_ABORT, C_RELEASE} Command;

//typedef enumWord{ACTION_DRIVE, ACTION_TRIGHT, ACTION_TLEFT} ACTION;

// typedef enumWord{ A_ACTIVE, A_INACTIVE, A_DONUT} ASTATE;

void command_all( Command cmd, EventList *eList, int res_id );

//void requestStateChange(int action, ASTATE newState)
//{
//}

int get_owner( int res_id )
{
	if( res_id >= 0 && res_id < MAX_RES )
		return res_owner[ res_id];
	else
		return AI_NONE;
}

void set_owner( int res_id, int owner )
{
	if( res_id >= 0 && res_id < MAX_RES )
		res_owner[ res_id ] = owner;

}

void zeroHomeless()
{
	if( get_owner(RES_WHEELS) == AI_NONE)
	{
		motor[BackL] = 0;
		motor[FrontL] = 0;
		motor[BackR] = 0;
		motor[FrontR] = 0;
	}
	if( get_owner(RES_ARM) == AI_NONE)
	{
		motor[Arm] = 0;
	}
}


int isPressed( EventList *eList,  int keyID, BSTATE   state )
{
	for( int i=0; i < eList->eventCnt; i++ )
		if( eList->bevents[i].keyID == keyID && eList->bevents[i].state == state ){
		return 1;
	}
	return 0;
}



void actionArm(Command cmd, EventList *eList, int res_id )
{
	const int UP_POWER = 30;
	const int DOWN_POWER = -10;
	switch(cmd)
	{
	case C_ABORT:
		if( get_owner(RES_ARM) == AI_ARM)
		{
			motor[Arm] = 0;
			set_owner( res_id, AI_NONE );
			return;
		}
		break;
	case C_RELEASE:
		actionArm(C_ABORT, null, RES_ARM);
		return;
	case C_EVENT:
		if(get_owner(RES_ARM)!= AI_ARM && get_owner(RES_ARM) != AI_NONE)//request_ownage(AI_DRIVE,res_id))
		{
			command_all( C_RELEASE, null, RES_WHEELS );
		}
		if( get_owner(RES_ARM) == AI_NONE)
			set_owner( RES_ARM, AI_ARM );

		if(get_owner(RES_ARM) == AI_ARM)
		{
			if( isPressed( eList, JOY1_YELLOW, B_PRSD )){
				writeDebugStreamLine("arm going up");
				motor[Arm] = UP_POWER;
			}
			if( isPressed( eList, JOY1_GREEN, B_PRSD ))	{
				writeDebugStreamLine("arm going down");
				motor[Arm] = DOWN_POWER;
			}
			if( isPressed( eList, JOY1_GREEN, B_RLSD ) || isPressed( eList, JOY1_YELLOW, B_RLSD ))	{
				motor[Arm] = 0;
			}
		}
		break;

	}


}

void actionTRight(Command cmd, EventList *eList, int res_id)
{
	// switch cmd

}

void actionTLeft(Command cmd, EventList *eList, int res_id)
{
	// switch cmd

}
float getMultiplier(EventList *eList)
{
	static float speedMultiplier = 1.0;

	if(isPressed(eList, JOY1_LBUMP, B_RLSD) || isPressed(eList, JOY1_RBUMP, B_RLSD)
		speedMultiplier = 1.0;
	if(isPressed(eList, JOY1_LBUMP, B_PRSD))
		speedMultiplier = 0.25;
	if(isPressed(eList, JOY1_RBUMP, B_PRSD))
		speedMultiplier= 1.80;

		return speedMultiplier;
}

void actionDrive(Command cmd, EventList *eList, int res_id )
{
	static int waitingToDrive = 0;
	static int typeDrive = 0;
	static float lefty, righty;

	switch(cmd)
	{
	case C_ABORT:
		if( get_owner(RES_WHEELS) == AI_DRIVE)
		{
			motor[BackL] = 0;
			motor[FrontL] = 0;
			motor[BackR] = 0;
			motor[FrontR] = 0;
			set_owner( res_id, AI_NONE );
			return;
		}
		break;
	case C_RELEASE:
		if(get_owner(res_id)== AI_DRIVE ){
			//stop driving
			set_owner( res_id, AI_NONE );
		}
		break;
	case C_EVENT:

		if(isPressed(eList, JOY1_RED, B_RLSD))
			typeDrive = !typeDrive;

		if(get_owner(RES_WHEELS)!= AI_DRIVE && get_owner(RES_WHEELS) != AI_NONE)//request_ownage(AI_DRIVE,res_id))
		{
			command_all( C_RELEASE, null, RES_WHEELS );
		}
		if( get_owner(RES_WHEELS) == AI_NONE)
			set_owner( RES_WHEELS, AI_DRIVE );

		if(get_owner(RES_WHEELS) == AI_DRIVE)
		{
			if(typeDrive == 0)
			{

				if(abs(abs(((eList->joy1.y *100)/128)/2) - lefty) > abs(lefty/3))
					lefty += (((eList->joy1.y *100)/128)/2)/3;

				if(abs(abs(((eList->joy1.y *100)/128)/2) - righty) > abs(righty/3))
				  righty += (((eList->joy1.y *100)/128)/2)/3;


				lefty += -((eList->joy2.x *100)/128)/2;
				righty += ((eList->joy2.x *100)/128)/2;


				if(true){ //invert front
					float temp;
					temp = lefty;
					lefty = -righty;
					righty = -temp;
				}
			}
			else if(typeDrive == 1)
			{
				lefty = -((eList->joy1.y *100)/128)/2;
				righty = -((eList->joy2.y *100)/128)/2;
			}

			lefty *= (float)getMultiplier(eList);
			righty *= (float)getMultiplier(eList);

			motor[FrontL] =lefty;
			motor[BackL] = lefty;

			motor[BackR] = -righty;
			motor[FrontR] = -righty;

			//

			//motor[BackL] += -((eList->joy2.x *100)/128)/2;
			//motor[FrontL] += -((eList->joy2.x *100)/128)/2;

			//motor[BackR] += ((eList->joy2.x *100)/128)/2;
			//motor[FrontR] += ((eList->joy2.x *100)/128)/2;

			//motor[BackR] *= -1;
			//motor[FrontR] *= -1;
		}
		//


	}
}


void command_all( Command cmd, EventList *eList, int res_id )
{
	//if(res_id == RES_WHEELS)
	//{
	//	if(get_owner(res_id) != AI_DRIVE)
	//		res_owner[res_id] = AI_DRIVE;

	//	if(get_owner(res_id) == AI_DRIVE)
	//		actionDrive(cmd, eList, res_id);
	//}

	actionDrive(cmd, eList, res_id);
	//actionArm(cmd, eList, res_id);
	actionTRight( cmd, eList, res_id);
	actionTLeft( cmd, eList, res_id);
	zeroHomeless();
}


//void processAll(EventList *eList)
//{
//	if(abs(eList->joy1.y)>10 || abs(eList->joy2.x)>10
//		|| eList->bevents[1] || eList->bevents[3])
//		command_all( C_EVENT, eList, RES_WHEELS);
//	//*all the methods*(&eList);
//}

task main()
{
	EventList eList;

	clearDebugStream();
	ClearTimer(T1);
	ClearTimer(T4);

	while(1)
	{
		getEvents(&eList);
		command_all(C_EVENT, &eList, -1);
		//processDrive(&eList);

		//for(int i = 0; i<eventCnt;i++)
		//{
		//	switch(bevents[i].keyID){
		//		//case *name-of-button* :
		//		//*actions* ;
		//		//break;
		//	};
		//}



		//_________________________________________________________________________



	}
}
