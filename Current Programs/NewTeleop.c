#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Hubs,  S3, HTServo,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     gyro,           sensorI2CHiTechnicGyro)
#pragma config(Sensor, S3,     ,               sensorI2CMuxController)
#pragma config(Sensor, S4,     sonar,          sensorSONAR)
#pragma config(Motor,  motorA,          lateralSweep,  tmotorNXT, openLoop)
#pragma config(Motor,  motorB,          stuffer,       tmotorNXT, openLoop, reversed, encoder)
#pragma config(Motor,  motorC,          verticalSweep, tmotorNXT, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_1,     FrontL,        tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     BackL,         tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     FrontR,        tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     BackR,         tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     rightRoller,   tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     leftRoller,    tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C4_1,     elevator,      tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C4_2,     car,           tmotorTetrix, openLoop, encoder)
#pragma config(Servo,  srvo_S3_C1_1,    grabberServo,         tServoContinuousRotation)
#pragma config(Servo,  srvo_S3_C1_2,    dropperServo,         tServoStandard)
#pragma config(Servo,  srvo_S3_C1_3,    sweeperServo,         tServoContinuousRotation)
#pragma config(Servo,  srvo_S3_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S3_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S3_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define null (void *)0
#include "ButtonJoyDriver.c"



#define AI_NONE 0
#define AI_DRIVE 1
#define AI_RTURN 2
#define AI_LTURN 3
#define AI_ELEVATOR 4
#define AI_ROLLERS 5
#define AI_GRABBER 6
#define AI_DELIVERY 7
#define AI_CAR 8
#define AI_SWEEPER 9

#define MAX_RES 7 //Number of resources being tracked

#define RES_WHEELS 0 //This stuff should probably be an enum
#define RES_ROLLERS 1
#define RES_DELIVERY 2
#define RES_ELEV 3
#define RES_GRABBER 4
#define RES_CAR 5
#define RES_SWEEPER 6

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

void request_ownage(int action, int res_id)
{
	if(get_owner(res_id)!= action && get_owner(res_id) != AI_NONE)//
	{
		command_all( C_RELEASE, null, res_id );
	}
	if( get_owner(res_id) == AI_NONE)
		set_owner( res_id, action );
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
}


int isPressed( EventList *eList,  int event )
{
	for( int i=0; i < eList->eventCnt; i++ )
		if( eList->bevents[i] == event ){
		return 1;
	}
	return 0;
}




void actionTakeADump(Command cmd, EventList *eList, int res_id )
{
	const int DUMPING_POS = 125;
	const int HOLDING_POS = 50;
	const int INCREMENTS = 10;
	const int OPEN_TIME = 500;
	static int currentTarget = HOLDING_POS;
	static int state = 1;
	switch(cmd)
	{
	case C_ABORT:
		if( get_owner(RES_DELIVERY) == AI_DELIVERY)
		{
			servo[dropperServo] = HOLDING_POS;
			set_owner( res_id, AI_NONE );
			return;
		}
		break;
	case C_RELEASE:
		actionTakeADump(C_ABORT, null, RES_DELIVERY);
		return;
	case C_EVENT:
		request_ownage(AI_DELIVERY , RES_DELIVERY);

		if(get_owner(RES_DELIVERY) == AI_DELIVERY)
		{
			if( isPressed( eList, takeADump )){
				//writeDebugStreamLine("Dropper Opening");
				//servo[dropperServo] = DUMPING_POS;
				state *= -1;
				ClearTimer(T2);
			}

			if(state == 1)
			{
				currentTarget = HOLDING_POS;
			}

			else if(state == -1)
			{
				if(currentTarget < DUMPING_POS)
				{
					if(time1[T2] >= (OPEN_TIME/INCREMENTS))
					{
						currentTarget += ((DUMPING_POS - HOLDING_POS)/INCREMENTS);
						ClearTimer(T2);
					}
				}
			}
			servo[dropperServo] = currentTarget;

		}
		break;

	}


}


void actionRoller(Command cmd, EventList *eList, int res_id )
{
	const int UP_POWER = -100;
	const int DOWN_POWER = 100;
	switch(cmd)
	{
	case C_ABORT:
		if( get_owner(RES_ROLLERS) == AI_ROLLERS)
		{
			motor[leftRoller] = 0;
			motor[rightRoller] = 0;
			set_owner( res_id, AI_NONE );
			return;
		}
		break;
	case C_RELEASE:
		actionRoller(C_ABORT, null, RES_ROLLERS);
		return;
	case C_EVENT:
		request_ownage(AI_ROLLERS , RES_ROLLERS);

		if(get_owner(RES_ROLLERS) == AI_ROLLERS)
		{
			if( isPressed( eList, rollrStart )){
				writeDebugStreamLine("arm going up");
				motor[leftRoller] = UP_POWER;
				motor[rightRoller] = UP_POWER;
			}
			if( isPressed( eList, rollrRvsStrt ))	{
				writeDebugStreamLine("arm going down");
				motor[leftRoller] = DOWN_POWER;
				motor[rightRoller] = DOWN_POWER;
			}
			if( isPressed( eList, rollrStop ))	{
				motor[leftRoller] = 0;
				motor[rightRoller] = 0;
			}
		}
		break;

	}


}



void actionSweeper(Command cmd, EventList *eList, int res_id )
{
	const int POWER = -100;
	const int SERVO_POWER = 255;
	switch(cmd)
	{
	case C_ABORT:
		if( get_owner(RES_SWEEPER) == RES_SWEEPER)
		{
			motor[stuffer] = 0;
			motor[lateralSweep] = 0;
			motor[verticalSweep] = 0;
			servo[sweeperServo] = 127;
			set_owner( res_id, AI_NONE );
			return;
		}
		break;
	case C_RELEASE:
		actionSweeper(C_ABORT, null, RES_SWEEPER);
		return;
	case C_EVENT:
		request_ownage(AI_SWEEPER , RES_SWEEPER);

		if(get_owner(RES_SWEEPER) == AI_SWEEPER)
		{
			if( isPressed( eList, sweeperStart )){
				writeDebugStreamLine("sweepers going on");
				motor[stuffer] = POWER;
				motor[lateralSweep] = -POWER;
				motor[verticalSweep] = POWER;
				servo[sweeperServo] = SERVO_POWER;
			}
			if( isPressed( eList, sweeperStop ))	{
				writeDebugStreamLine("arm going down");
				motor[stuffer] = 0;
				motor[lateralSweep] = 0;
				motor[verticalSweep] = 0;
				servo[sweeperServo] = 127;

			}


		}
		break;

	}


}


void actionGrabber(Command cmd, EventList *eList, int res_id )
{
	const int GRABBING_POWER = 255;
	const int RELEASEING_POWER = 0;
	const int STOP_POWER = 127;

	switch(cmd)
	{
	case C_ABORT:
		if( get_owner(RES_GRABBER) == AI_GRABBER)
		{
			servo[grabberServo] = STOP_POWER;
			set_owner( res_id, AI_NONE );
			return;
		}
		break;
	case C_RELEASE:
		actionGrabber(C_ABORT, null, RES_GRABBER);
		return;
	case C_EVENT:
		request_ownage(AI_GRABBER , RES_GRABBER);

		if(get_owner(RES_GRABBER) == AI_GRABBER)
		{
			if( isPressed( eList, grab )){
				servo[grabberServo] = GRABBING_POWER;
				//state = g_down;
				//motor[grabber] = UP_POWER;
				//nMotorEncoder[grabber] = 0;
				//ClearTimer(T1);
			}
			if( isPressed( eList, release ))	{
				servo[grabberServo] = RELEASEING_POWER;
				//state = g_up;
				//motor[grabber] = DOWN_POWER;
				//nMotorEncoder[grabber] = 0;
				//ClearTimer(T1);
			}

			if( isPressed( eList, stopGrab ))	{
				servo[grabberServo] = STOP_POWER;
				//state = g_up;
				//motor[grabber] = DOWN_POWER;
				//nMotorEncoder[grabber] = 0;
				//ClearTimer(T1);
			}
			/*if(state==g_down)
			{
			if(nMotorEncoder[grabber] >= 0 || time1[T1] >= 2000)
			{
			motor[grabber] = 0;
			state = g_hold;
			}
			}
			if(state == g_up)
			{
			if(nMotorEncoder[grabber] <= -140 || time1[T1] >= 2000)
			{
			motor[grabber] = 0;
			state = g_hold;
			}
			}*/
		}

		break;

	}


}

void actionElevator(Command cmd, EventList *eList, int res_id )
{
	const int DOWN_POWER = 40;
	const int UP_POWER = -90;

	switch(cmd)
	{
	case C_ABORT:
		if( get_owner(RES_ELEV) == AI_ELEVATOR)
		{
			motor[elevator] = 0;
			set_owner( res_id, AI_NONE );
			return;
		}
		break;
	case C_RELEASE:
		actionElevator(C_ABORT, null, RES_ELEV);
		return;
	case C_EVENT:
		request_ownage(AI_ELEVATOR , RES_ELEV);

		if(get_owner(RES_ELEV) == AI_ELEVATOR)
		{
			if( isPressed( eList, elevatorUp )){
				motor[elevator] = UP_POWER;
			}
			if(isPressed( eList, elevatorDown )){
				motor[elevator] = DOWN_POWER;
			}
			if(isPressed( eList, elvStop )){
				motor[elevator] = 0;
			}

		}

		break;
	}
}

void autoLiftElevator(bool reset = false){
	static int CENTER_HEIGHT = -3800;
	static int TALL_HEIGHT = -2300;
	static int DOWN_HEIGHT = -50;

	static int positionTarget = 0;
	static bool goingUp = true;
	static bool moving = false;
	if(reset){
		moving = false;
	}

	static int lastTopHat = -1;
	int topHat = joystick.joy2_TopHat;
	//writeDebugStreamLine("%d", topHat);
	if (moving) {
		if((goingUp && nMotorEncoder[car] <= positionTarget) || (!goingUp && nMotorEncoder[car] >= positionTarget) ){
			motor[car] = 0;
			moving = false;
		}
	}
	if (topHat == -1 || topHat == lastTopHat){
		return;
	}
	lastTopHat = topHat;
	bool changed = false;
	if (topHat == 0){
		changed = true;
		positionTarget = CENTER_HEIGHT;
		actionRoller(C_ABORT, null, RES_ROLLERS);
	}
	if (topHat == 2 || topHat == 6){
		changed = true;
		positionTarget = TALL_HEIGHT;
		actionRoller(C_ABORT, null, RES_ROLLERS);
	}
	if (topHat == 4){
		changed = true;
		positionTarget = DOWN_HEIGHT;
	}

	if(changed){
		goingUp = positionTarget < nMotorEncoder[car];
		if (goingUp) motor[car] = -100;
		else motor[car] = 60;
		moving = true;
		writeDebugStreamLine("New car target is %d", positionTarget);
	}
}


void actionCar(Command cmd, EventList *eList, int res_id)
{
	const int UP_POWER = -100;
	const int DOWN_POWER = 50;

	switch(cmd)
	{
	case C_ABORT:
		if( get_owner(RES_CAR) == AI_CAR)
		{
			autoLiftElevator(true);
			motor[car] = 0;
			set_owner( res_id, AI_NONE );
			return;
		}
		break;
	case C_RELEASE:
		actionCar(C_ABORT, null, RES_CAR);
		return;
	case C_EVENT:
		request_ownage(AI_CAR , RES_CAR);

		if(get_owner(RES_CAR) == AI_CAR)
		{
			if( isPressed( eList, liftCar )){
				motor[car] = UP_POWER;
				//actionRoller(C_ABORT, null, RES_ROLLERS);
			}
			if( isPressed( eList, lowerCar )){
				motor[car] = DOWN_POWER;

			}
			if( isPressed( eList, stopCar )){
				motor[car] = 0;
				autoLiftElevator(true);
			}

		}

		break;

	}
	if(get_owner(RES_CAR) == AI_CAR) autoLiftElevator(false);
}

float getMultiplier(EventList *eList)
{
	static float speedMultiplier = 1.0;

	if(isPressed(eList, normalSpd))
		speedMultiplier = 1.0;
	if(isPressed(eList, lessSpd))
		speedMultiplier = 0.25;
	if(isPressed(eList, moreSpd))
		speedMultiplier= 1.80;

	return speedMultiplier;
}

void actionDrive(Command cmd, EventList *eList, int res_id )
{
	static int waitingToDrive = 0;
	static int typeDrive = 0;
	static float lefty, righty;

	const float accelLimit = 60;  //Limits the maximum change in wheel power per second. Units are "percent/second". 0 to disable. 60 is a good number.
	const float MIN_ACCEL_LIMIT_POW = 20; //Aceleration limiting only kicks in for powers less than this.

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

		if(isPressed(eList, switchDrive))
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
				/*if(abs(abs(((eList->joy1.y *100)/128)/2) - lefty) > abs(lefty/3))
				lefty += (((eList->joy1.y *100)/128)/2)/3;

				if(abs(abs(((eList->joy1.y *100)/128)/2) - righty) > abs(righty/3))
				righty += (((eList->joy1.y *100)/128)/2)/3;


				lefty += -((eList->joy2.x *100)/128)/2;
				righty += ((eList->joy2.x *100)/128)/2;*/

				lefty = righty = ((eList->joy2.y *100)/128);///2;


				lefty  +=   ((eList->joy1.x *100)/128);///2;
				righty +=  -((eList->joy1.x *100)/128);///2;

				if(false){ //invert front
					float temp;
					temp = lefty;
					lefty = -righty;
					righty = -temp;
				}
			}
			else if(typeDrive == 1)
			{
				lefty = -((eList->joy2.y *100)/128);///2;
				righty = -((eList->joy1.y *100)/128);///2;
			}

			float multiplier = getMultiplier(eList);

			lefty *= (float)getMultiplier(eList);
			righty *= (float)getMultiplier(eList);

			if (multiplier < 0.5){
				float turn = (righty - lefty) / 2.0;
				righty += turn;
				lefty -= turn;
			}

			if(getMultiplier(eList) >= 1){

				if (accelLimit != 0){ //Acceleration limiting code
					static float lastAvgPow = 0.0;
					static int lastTime = nPgmTime;

					int timediff = (nPgmTime - lastTime);
					float dt = timediff / 1000.0; //Time since last loop iteration, in seconds
					lastTime = nPgmTime;
					float Avg = (lefty+righty)/2.0;
					if (dt > 0.0 && dt < 1.0) {
						float dAvg = (Avg) - lastAvgPow; //Units of "%"

						if (abs(dAvg/dt) > accelLimit && (abs(Avg) < MIN_ACCEL_LIMIT_POW)){ //Acceleration limit exceeded, taking over control.
							if (abs(lastAvgPow) > MIN_ACCEL_LIMIT_POW)
							{lastAvgPow = MIN_ACCEL_LIMIT_POW*sgn(lastAvgPow);}
							float diff = (righty-lefty)/2;
							Avg = lastAvgPow + sgn(dAvg) * accelLimit * dt;
							lefty = Avg - diff;
							righty = Avg + diff;
						}
					}
					lastAvgPow = Avg;
				}
			}

			motor[FrontL] 	= lefty;
			motor[BackL] 	= lefty;

			motor[BackR] 	= righty;
			motor[FrontR] 	= righty;
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
	actionRoller(cmd, eList, res_id);
	actionGrabber(cmd, eList, res_id);
	actionSweeper(cmd, eList, res_id);
	actionElevator(cmd, eList, res_id);
	actionCar(cmd, eList, res_id);
	actionTakeADump(cmd, eList, res_id);
	//actionTRight( cmd, eList, res_id);
	//actionTLeft( cmd, eList, res_id);
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
	waitForStart();
	nMotorEncoder[car] = 0;
	EventList eList;

	clearDebugStream();
	ClearTimer(T1);
	ClearTimer(T2);
	ClearTimer(T4);


	while(1)
	{
		getEvents(&eList);
		if(!bDisconnected)
		{
			command_all(C_EVENT, &eList, -1);
		}
		else
		{
			command_all(C_ABORT, &eList, -1);
		}

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
