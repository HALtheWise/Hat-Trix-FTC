#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     gyro,           sensorI2CHiTechnicGyro)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     tailWinch,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     hookWinch,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     arms,          tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     wrist,         tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     FL,            tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     BL,            tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_1,     FR,            tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     BR,            tmotorTetrix, openLoop, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

int buttons[] = {0,0,0,0};

void getButtons()
{


	//Get touch sensor inputs
	long value = 1023-SensorRaw(S2);
	long switches = 339 * value;
	switches /= (1023-value);
	switches += 5;
	switches /= 10;
	if(switches&8)
		buttons[0] = 1;
	else
		buttons[0] = 0;
	if(switches&4)
		buttons[1] = 1;
	else
		buttons[1] = 0;
	if(switches&2)
		buttons[2] = 1;
	else
		buttons[2] = 0;
	if(switches&1)
		buttons[3] = 1;
	else
		buttons[3] = 0;
	//writeDebugStreamLine("switch1: %d" , buttons[0]);
	//writeDebugStreamLine("switch2: %d" , buttons[1]);
	//writeDebugStreamLine("switch3: %d" , buttons[2]);
	//writeDebugStreamLine("switch4: %d" , buttons[3]);

	return;
}

task main()
{
	ClearTimer(T1);
	clearDebugStream();
	while(1 && nNxtButtonPressed != 3)
	{
		getButtons();
		if(time1[T1] > 30000)
			break;

		if(buttons[3] && buttons[2])
		{
			motor[tailWinch] = 90;
			motor[arms] = 0;
			motor[hookWinch] = 0;
		}
		else if(buttons[1] && buttons[0])
		{
			motor[tailWinch] = -50;
			motor[arms] = 0;
			motor[hookWinch] = 0;
		}

		else if(buttons[3])
		{
			motor[tailWinch] = 0;
			motor[arms] = -20;
			motor[hookWinch] = 0;
		}

		else if(buttons[2])
		{
			motor[tailWinch] = 0;
			motor[arms] = 20;
			motor[hookWinch] = 0;
		}

		else if(buttons[1])
		{
			motor[tailWinch] = 0;
			motor[arms] = 0;
			motor[hookWinch] = -20;
		}

		else if(buttons[0])
		{
			motor[tailWinch] = 0;
			motor[arms] = 0;
			motor[hookWinch] = 20;
		}

		else
		{
			motor[hookWinch] = 0;
			motor[arms] = 0;
			motor[tailWinch] = 0;
		}

		if(nNxtButtonPressed == 1)
		{
			motor[wrist] = 15;  //Move wrist up
		}
		else if(nNxtButtonPressed == 2)
		{
			motor[wrist] = -15;
			//Move wrist down
		}
		else
		{
			motor[wrist] = 0;

		}
	}
nMotorEncoder[arms] = 0;
nMotorEncoder[wrist] = 0;

}