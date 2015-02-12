#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Sensor, S2,     touch,          sensorTouch)
#pragma config(Sensor, S3,     gyro,           sensorI2CHiTechnicGyro)
#pragma config(Sensor, S4,     seeker,         sensorHiTechnicIRSeeker1200)
#pragma config(Motor,  motorA,          wrist1,        tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorB,          wrist2,        tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     tailwinch,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     hookwinch,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     arms,          tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     stick,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     FL,            tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     BL,            tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_1,     FR,            tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     BR,            tmotorTetrix, openLoop, encoder)

task main()
{
	clearDebugStream();
	nMotorEncoder[BR] = 0;
	while(abs(nMotorEncoder[BR]) < 2000)
	{
		motor[FL] = -50;
		motor[BL] = -50;
		motor[FR] = 50;
		motor[BR] = 50;
	}
	motor[FL] = 0;
	motor[BL] = 0;
	motor[FR] = 0;
	motor[BR] = 0;

}
