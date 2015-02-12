#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Hubs,  S3, HTServo,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     ,               sensorI2CMuxController)
#pragma config(Motor,  motorB,          extend1,       tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          extend2,       tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     FrontR,        tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     BackR,         tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     FrontL,        tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     BackL,         tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     elevator,      tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     winch,         tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C4_1,     arm,           tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C4_2,     rollers,       tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S3_C1_1,    latch,                tServoStandard)
#pragma config(Servo,  srvo_S3_C1_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S3_C1_3,    grabber,              tServoStandard)
#pragma config(Servo,  srvo_S3_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S3_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S3_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

task main()
{
	while(1 && nNxtButtonPressed != 3)
	{
		if(nNxtButtonPressed == 1)
			motor[winch] = 25;
		else if(nNxtButtonPressed == 2)
			motor[winch] = -25;

		else
			motor[winch] = 0;
	}
	nMotorEncoder[winch] = 0;


}
