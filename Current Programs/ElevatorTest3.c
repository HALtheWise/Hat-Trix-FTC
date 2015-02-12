#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Hubs,  S3, HTServo,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     gyro,           sensorI2CHiTechnicGyro)
#pragma config(Sensor, S3,     ,               sensorI2CMuxController)
#pragma config(Sensor, S4,     seeker,         sensorI2CCustom)
#pragma config(Motor,  motorA,          stuffer,       tmotorNXT, openLoop)
#pragma config(Motor,  motorB,          deploySweep,   tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorC,          sweeper,       tmotorNXT, openLoop, reversed)
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

#include "JoystickDriver.c"

task main()
{

	clearDebugStream();
	nMotorEncoder[deploySweep] = 0;

	while(1)
	{
		getJoystickSettings(joystick);
		//writeDebugStreamLine("encoder value: %d" , nMotorEncoder[elevator] );

		if(joy1Btn(10))
			nMotorEncoder[elevator] = 0;

		if(joy1Btn(4))
		{
			writeDebugStreamLine("down");
			servo[dropperServo] = 130;
			//motor[elevator] = -90;
		}
		else if(joy1Btn(2))
		{
			writeDebugStreamLine("up");
			servo[dropperServo] = 55;
			//motor[elevator] = 90;
		}
		else
		{
			//motor[deploySweep] = 0;
			//motor[arm] = 0;
			//motor[elevator] = 0;
		}

		//if(joy1Btn(1))
		//	motor[pin] = 100;
		//else if(joy1Btn(3))
		//	motor[pin] = 100;
		//else
		//	motor[pin] = 0;

		//getEvents();

		//for(int i = 0; i<eventCnt;i++)
		//{
		//	switch(bevents[i].keyID){
		//	case JOY1_YELLOW :
		//		if(bevents[i].state == B_PRSD)
		//			motor[elevator] = 10;
		//		else if(bevents[i].state == B_RLSD)
		//			motor[elevator] = 0;
		//		break;
		//		case JOY1_GREEN:
		//		if(bevents[i].state == B_PRSD)
		//			motor[elevator] = -10;
		//		else if(bevents[i].state == B_RLSD)
		//			motor[elevator] = 0;
		//	break;
		//	};
		//}



		//_________________________________________________________________________



	}
}
