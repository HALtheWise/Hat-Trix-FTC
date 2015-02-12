#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Sensor, S2,     touch,          sensorTouch)
#pragma config(Sensor, S3,     gyro,           sensorI2CHiTechnicGyro)
#pragma config(Sensor, S4,     seeker,         sensorI2CCustom)
#pragma config(Motor,  motorA,          kicker1,       tmotorNXT, openLoop)
#pragma config(Motor,  motorB,          kicker2,       tmotorNXT, openLoop)
#pragma config(Motor,  motorC,          tubeGrabber,   tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     tailWinch,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     hookWinch,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     arms,          tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     wrist,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     FL,            tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C3_2,     BL,            tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C4_1,     FR,            tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     BR,            tmotorTetrix, openLoop, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "TheGreatLibrary.c"

#define JOY_THRESH 12

#define BMAP_NUMJOY 2
#define BMAP_NUMBUTTONS 24

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





//_______________________________________________________________________

int cancel = 0;
int dump = 0;




void addEvent(int keyID, BSTATE state)//ACTION act
{

	bevents[eventCnt].keyID = keyID;
	bevents[eventCnt].state = state;

	eventCnt++;
}


//________________?_______________________________________________________
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


/*
void getEvents()
{
PrevJoy = joystick;
getJoystickSettings(joystick);
eventCnt=0;
for( int i=1; i <= 12; i++ )
{
if((joystick.joy1_Buttons & (1 << i)) != (PrevJoy.joy1_Buttons & (1<< i)))
{
if(joystick.joy1_Buttons & (1 << i))
writeDebugStreamLine("B_PRSD");
else
writeDebugStreamLine("B_RLSD");

addEvent(i, (joystick.joy1_Buttons & (1 << i)) ? B_PRSD : B_RLSD);
}//acting(Bmap[0][i]);


if((joystick.joy2_Buttons & (1 << (i - 1)))!= (PrevJoy.joy2_Buttons & (1 << (i - 1))))
{
writeDebugStreamLine("%d",(joystick.joy2_Buttons & (1<<(i-1))));
if(joystick.joy2_Buttons & (1 << i))
writeDebugStreamLine("B_PRSD");
else
writeDebugStreamLine("B_RLSD");
addEvent(i+10, (joystick.joy2_Buttons & (1 << (i-1))) ? B_PRSD : B_RLSD);
}//acting(Bmap[1][i]);
}

}*/


/*void kickTheBucket(int button, int stopbutton)
{
static int Stop = 0;
static int timelimit = 0;
static int stage = 0;

if(stopbutton)
Stop = 1;
else if(button)
{
Stop = 0;
stage = 0;
timelimit = time1[T4] + 1500;
}

if(Stop)
{
motor[kicker1] = 0;
motor[kicker2] = 0;
return;
}

if(time1[T4] >= timelimit)
{
motor[kicker1] = 0;
motor[kicker2] = 0;
return;
}
if(stage == 0)
{
motor[kicker1] = 100;
motor[kicker2] = 100;
if(nMotorEncoder[kicker1] >= 550)
{
motor[kicker1] = 0;
motor[kicker2] = 0;
stage = 1;
}
}
else if(stage == 1)
{
motor[kicker1] = -50;
motor[kicker2] = -50;
if(nMotorEncoder[kicker1] <= 25)
{
motor[kicker1] = 0;
motor[kicker2] = 0;
stage = 2;
}
}
}*/

////////////////////////////////////////////////////////////////////////////// TASK MAIN!!! \(^0^)/
//struct {int z; int y;} x = { 4, 5};



task main()
{

	clearDebugStream();
	int power = 0;
	ClearTimer(T1);
	ClearTimer(T4);
	int forward = 1;
	nMotorEncoder[kicker1] = 0;
	nMotorEncoder[kicker2] = 0;
	writeDebugStreamLine("arm encoder %d" , nMotorEncoder[arms]);

	//waitForStart();
	//waywardSon(-1, 1);

	while(1)
	{
		getEvents();
		for(int i = 0; i<eventCnt;i++)
		{
			switch(bevents[i].keyID){
			case JOY1_BLUE:
				if(bevents[i].state == B_PRSD)
				{
					motor[kicker1] = -100;
					motor[kicker2] = -100;
				}
				else if(bevents[i].state == B_RLSD)
				{
					motor[kicker1] = 0;
					motor[kicker2] = 0;
				}
				break;
			case JOY1_GREEN:
				if(bevents[i].state == B_PRSD)
				{
					motor[tubeGrabber] = -75;
				}
				else if(bevents[i].state == B_RLSD)
					motor[tubeGrabber] = 0;
				break;
			case JOY1_RED:
				if(bevents[i].state == B_PRSD)
				{
					motor[kicker1] = 100;
					motor[kicker2] = 100;
				}
				else if(bevents[i].state == B_RLSD)
				{
					motor[kicker1] = 0;
					motor[kicker2] = 0;
				}
				break;
			case JOY1_YELLOW:
			if(bevents[i].state == B_PRSD)
				{
					motor[tubeGrabber] = 75;
				}
				else if(bevents[i].state == B_RLSD)
					motor[tubeGrabber] = 0;
				break;
				//cancel=1;
				break;
			case JOY1_LBUMP: break;
			case JOY1_RBUMP: break;
			case JOY1_LTRIG:
				if(bevents[i].state == B_PRSD)
					motor[tailWinch] = - 50;
				else if(bevents[i].state == B_RLSD)
					motor[tailWinch] = 0;
				break;
			case JOY1_RTRIG:
				if(bevents[i].state == B_PRSD)
					motor[tubeGrabber] = -75;
					//motor[tailWinch] = 90;
				else if(bevents[i].state == B_RLSD)
					motor[tubeGrabber] = 0;
					//motor[tailWinch] = 0;
				break;
			case JOY1_BACK: break;
			case JOY1_START: break;
				//
			case JOY2_BLUE:
				cancel=1;
				break;
			case JOY2_GREEN:
				dump = 1;
				break;
			case JOY2_RED:
				takeAbite();
				break;
			case JOY2_YELLOW:break;
			case JOY2_LBUMP:break;
			case JOY2_RBUMP:break;
			case JOY2_LTRIG:
				if(bevents[i].state == B_PRSD)
					motor[hookWinch] = 100;
				else //if(bevents[i].state == B_RLSD)
					motor[hookWinch] = 0;
				break;
			case JOY2_RTRIG:
				if(bevents[i].state == B_PRSD)
					motor[hookWinch] = -100;
				else //if(bevents[i].state == B_RLSD)
					motor[hookWinch] = 0;
				break;
			case JOY2_BACK:break;
			case JOY2_START:
				nMotorEncoder[arms] = 0;
				nMotorEncoder[wrist] = 0;
				break;
			};
		}

		//acting(bevents[i]);

		//writeDebugStreamLine("%d", joystick.joy1_TopHat);


		//if(time1[T1] >= 750)
		//{
		//	writeDebugStreamLine("BR encoder %d" , nMotorEncoder[BR]);
		//	writeDebugStreamLine("wrist encoder %d" , nMotorEncoder[wrist]);
		//	//writeDebugStreamLine("kicker encoder %d" , nMotorEncoder[kicker1]);
		//	ClearTimer(T1);
		//}

		//if(joy1Btn(9))
		//	joy1override = 1;
		//else if(joy1Btn(10))
		//	joy1override = 0;


		//takeAdump(joy2Btn(2), joy2Btn(1));
		//waywardSon(joystick.joy2, joy2Btn(1));


		//if(waywardSon(joystick.joy2_TopHat, cancel))
		//{
		//	if(abs(joystick.joy2_y1) > JOY_THRESH)
		//	{
		//		power = 80;
		//		if(joystick.joy2_y1 > 0)
		//		{
		//			power *= -1;
		//		}
		//		motor[arms] = power;
		//	}
		//	else
		//		motor[arms] = 0;
		//}

		////_________________________________


		//if(takeAdump(dump, cancel))
		//{
		//	if(abs(joystick.joy2_y2) > JOY_THRESH)
		//	{
		//		power = 30;
		//		if(joystick.joy2_y2 < 0)
		//		{
		//			power *= -1;
		//		}
		//		if(joy2Btn(5))
		//			power /= 3;

		//		motor[wrist] = power;
		//	}
		//	else
		//		motor[wrist] = 0;
		//}

		cancel = 0;
		dump = 0;



		//}



		/////////////////////////////////////////////////

		float leftpower = 0;
		float rightpower = 0;
		int slow = 0;
		int xslow = 0;

		/*if(joy1Btn(2))
		forward = 0;
		else if(joy1Btn(4))
		forward = 1;*/

		int yaxis = joystick.joy1_y2;
		int xaxis = joystick.joy1_x1;

		leftpower = yaxis + xaxis;
		rightpower = yaxis - xaxis;

		//change "gears"
		if(joy1Btn(5))
			xslow = 1;
		else if(joy1Btn(6))
			slow = 1;
		else
		{
			slow = 0;
			xslow = 0;
		}

		if(xslow)
		{
			leftpower /= 3;
			rightpower /= 3;
		}

		else if(slow)
		{
			leftpower /= 2;
			rightpower /= 2;
		}

		//scale from joystick outputs to motors
		leftpower = (leftpower*100)/128;
		rightpower = (rightpower*100)/128;

		//if a side would get insignificant power just turn it off
		if(abs(leftpower) < 10)
			leftpower = 0;
		if(abs(rightpower) < 10)
			rightpower = 0;
		if(forward)
		{
			motor[FL] = leftpower;
			motor[BL] = leftpower;
			motor[FR] = rightpower;
			motor[BR] = rightpower;
		}
		else
		{
			motor[FR] = leftpower;
			motor[BR] = leftpower;
			motor[FL] = rightpower;
			motor[BL] = rightpower;
		}
	}
}
