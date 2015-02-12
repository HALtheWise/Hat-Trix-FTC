#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Hubs,  S3, HTServo,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     seeker,         sensorI2CCustom)
#pragma config(Sensor, S3,     ,               sensorI2CMuxController)
#pragma config(Sensor, S4,     gyro,           sensorI2CHiTechnicGyro)
#pragma config(Motor,  motorA,          sweeper1,      tmotorNXT, openLoop, reversed)
#pragma config(Motor,  motorB,          sweeper2,      tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     FrontL,        tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     BackL,         tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     FrontR,        tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     BackR,         tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     rightRoller,   tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     leftRoller,    tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C4_1,     elevator,      tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C4_2,     car,           tmotorTetrix, openLoop, encoder)
#pragma config(Servo,  srvo_S3_C1_1,    grabberServo,         tServoStandard)
#pragma config(Servo,  srvo_S3_C1_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S3_C1_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S3_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S3_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S3_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.
#include "TrackerAutoLib.h"

#define MAX_STEPS 10

typedef enum{
	StepDrive,
	StepTurnDrive,
	StepTurn,
	StepGrab,
	StepRelease,
	StepDone,
} StepType;

typedef struct{
	string name;
	StepType type;
	float power;
	bool forward;
	FieldPos target;
} Step;

typedef struct{
	string name;
	int numSteps;
	Step *steps[MAX_STEPS];
} Plan;

void initializeRobot()
{
	clearDebugStream();
	gyroCal();
	//servo[grabberServo]=???;
	return;
}

void executeStep(Step *step);

void executePlan(Plan *plan){
	for (int i = 0; i < plan->numSteps; i++)
     {
       executeStep(plan->steps[i]);
     }
}

void executeStep(Step *step){
	writeDebugStreamLine("Beginning execution of step \"%s\"", step->name);
	switch (step->type)
	{
	case StepDrive:
		moveTo(step->target, step->power, step->forward);
		break;
	case StepTurnDrive:
		turnAndMoveTo(step->target, step->power, step->forward);
		break;
	case StepTurn:
		turnTo(step->target, step->power, step->forward);
		break;
	case StepGrab:
		grabGoal();
		wait1Msec(500);
		break;
	case StepRelease:
		releaseGoal();
		wait1Msec(500);
		break;
	case StepDone:
		writeDebugStreamLine("Finished executing Autonomous program");
		StopAllTasks();
		break;

	default:
		writeDebugStreamLine("Unknown step type detected (%d). Quitting now.", step->type);
		StopAllTasks();
	}

}
Plan planA;

void setupPlans(){
	Step *t;
	planA.name = "squareTest";
	planA.numSteps = 6;
	Step aSteps[6];

	memset(&aSteps, 0, sizeof(aSteps));

	t = &aSteps[0];
	t->name = "Step 1"; t->type = StepTurnDrive;
	t->target.x = 149; t->target.y = 95; t->power = 50;

	t = &aSteps[1];
	t->name = "Step 2"; t->type = StepTurnDrive;
	t->target.x = 260; t->target.y = 0;  t->power = 50;

	t = &aSteps[2];
	t->name = "Step 3"; t->type = StepTurnDrive;
	t->target.x = 149; t->target.y = -95; t->power = 50;

	t = &aSteps[3];
	t->name = "Step 4"; t->type = StepTurnDrive;
	t->target.x = 0; t->target.y = 0; t->power = 50;

	t = &aSteps[4];
	t->name = "Turn to 0"; t->type = StepTurn;
	t->target.x = 10000; t->target.y = 0; t->power = 40;

	t = &aSteps[5];
	t->name = "Done!"; t->type = StepDone;
	wait1Msec(5000);
	for(int i = 0; i < planA.numSteps; i++){
		planA.steps[i] = &aSteps[i];
	}
}

task main()
{
	initializeRobot();
	setupPlans();

	waitForStart(); // Wait for the beginning of autonomous phase.

	resetTracker();
	StartTask(trackRobot);

	executePlan(&planA);
}