#pragma config(Hubs,  S1, HTMotor,  none,     none,     none)
#pragma config(Motor,  mtr_S1_C1_1,     right,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     left,          tmotorTetrix, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"

#define JOY_THRESH 50

task main()
{
	while(1)
	{
		getJoystickSettings(joystick);

		int power = joystick.joy1_y1;
		int forward = 0;
		int left = 0;

		if(abs(joystick.joy1_y2) > JOY_THRESH)//straight
		{
			if(joystick.joy1_y2 > 0)
				forward = 1;
			else
				forward = 0;
			if(forward)
			{
				motor[left] = power;
				motor[right] = -power;
			}
			else
			{
				motor[left] = -power;
				motor[right] = power;
			}
		}

		else if(abs(joystick.joy1_x2) > JOY_THRESH)//turn
		{
			if(joystick.joy1_x2 > 0)
				left = 1;
			else
				left = 0;
			if(left)
			{
				motor[left] = power;
				motor[right] = power;
			}
			else
			{
				motor[left] = -power;
				motor[right] = -power;
			}
		}
		else
		{
			motor[left] = 0;
			motor[right] = 0;
		}
	}


}
