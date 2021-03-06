#pragma config(Hubs,  S1, HTMotor,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  motorA,          wrist,         tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     motorD,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     spatula,       tmotorTetrix, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"

#define JOY_THRESH 3

task main()
{


	while(1)
	{

		getJoystickSettings(joystick);

		if(joy1Btn(4))
			motor[spatula] = 40;

		else if(joy1Btn(2))
			motor[spatula] = -40;

		else
			motor[spatula] = 0;

		if(joy1Btn(1))
			motor[wrist] = 75;

		else if(joy1Btn(3))
			motor[wrist] = -75;

		else
			motor[wrist] = 0;

	}


}
