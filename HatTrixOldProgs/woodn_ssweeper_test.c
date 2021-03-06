#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     BL,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,    FL,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     BR,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     FR,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C3_1,    sweeper,              tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"

#define JOY_THRESH 12

task main()
{

	int power = 0;

	//waitForStart();

	servoTarget[sweeper] = 0;

	while(1)
	{
		getJoystickSettings(joystick);

		if(joy1Btn(3))
		{
			servoTarget[sweeper] = ServoValue[sweeper];
			continue;
		}
		else if(joy1Btn(1))
		{
			ClearTimer(T1);
			while(time1[T1] < 150 && !joy1Btn(3))
			{
				servo[sweeper] = 20;
			}

			ClearTimer(T1);
			while(time1[T1] < 150 && !joy1Btn(3))
			{
				servo[sweeper] = 0;
			}
		}
		else if(joy1Btn(2))
		{
			servoTarget[sweeper] = 220; // grab
		}

		if( abs( joystick.joy1_x2 ) > JOY_THRESH ) //turn
		{
			power = 20;
			if( joystick.joy1_x2 > 0 )
				power = -20;
			motor[FR] = power;
			motor[BR] = power;
			motor[FL] = power;
			motor[BL] = power;
		}
		else if( abs( joystick.joy1_y1 ) > JOY_THRESH ) //straight
		{
			power = -20;
			if ( joystick.joy1_y1 > 0 )
				power = 20;
			motor[FR] = power;
			motor[BR] = power;
			motor[FL] = -power;
			motor[BL]  = -power;
		}

		else
		{
			motor[FR] = 0;
			motor[BR] = 0;
			motor[FL] = 0;
			motor[BL] = 0;
		}

	}


}
