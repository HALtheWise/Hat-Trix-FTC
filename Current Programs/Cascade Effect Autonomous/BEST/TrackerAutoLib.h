#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Hubs,  S3, HTServo,  none,     none,     none)
#pragma config(Sensor, S2,     gyro,           sensorI2CHiTechnicGyro)
#pragma config(Sensor, S4,     sonar,          sensorSONAR)
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
#pragma config(Servo,  srvo_S3_C1_2,    dropperServo,         tServoStandard)
#pragma config(Servo,  srvo_S3_C1_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S3_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S3_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S3_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "GyroStuff.h"
#include "RobotTracker.h"

//---------------- Constants ----------------//
#define NO_STEERING 0.0

//------------- Global Variables ------------//
typedef enum{
	Forward = 1,
	Backward = 0,
	AutomaticDirection = 2,
} DrivingDirection;

//---------- Function Declarations ----------//
void	mot2	  (int leftPow, int rightPow);
void moveTo (FieldPos target, int power, DrivingDirection forward = Forward, float aggressiveness = 1.0);
void turnTo (FieldPos target, int power, DrivingDirection forward = Forward);
void turnToHeading (float heading, int power, DrivingDirection forward = Forward);
void turnAndMoveTo (FieldPos target, int power, DrivingDirection forward = Forward);
float neededTurn(FieldPos target, DrivingDirection forward = Forward);
DrivingDirection autoSelectDirection(DrivingDirection in, FieldPos target);

void turnAndMoveTo (RelativePos target, int power, DrivingDirection forward = Forward);
void turnTo (RelativePos target, int power, DrivingDirection forward = Forward);
void moveTo (RelativePos target, int power, DrivingDirection forward = Forward);


//----------- Function Definitions ----------//

void turnAndMoveTo (FieldPos target, int power, DrivingDirection forward){
	turnTo(target, power, forward);
	moveTo(target, power, forward);
}

DrivingDirection autoSelectDirection(DrivingDirection in, FieldPos target){
	if (in != AutomaticDirection) return in;
	if (abs(coerceAngle(neededTurn(target, Forward))) < PI/2){
		return Forward;
	}
	return Backward;
}

//This function is identical to mot(int, int) as defined in "AutoLib.h", but importing both could cause a name collision,
// so the name of this was changed to mot2
void mot2(int leftPow, int rightPow){ //Takes left and right powers, applies them to wheels
	motor[FrontL] = leftPow;
	motor[BackL] = leftPow;

	motor[FrontR] = rightPow;
	motor[BackR] = rightPow;
}

//********************************************************************************************************
//*
//*	move( float dist, float power , bool hold, bool glide)
//*	is a move function that moves 'dist' at 'power' either abruptly, or 'glide'
//*
//********************************************************************************************************

void moveTo (FieldPos target, int power, DrivingDirection forward, float aggressiveness)
{
	forward = autoSelectDirection(forward, target);
	const bool glide = true; //This will eventually become an argument to moveTo().
	//const float TURN_AGGRESSIVENESS = 2.0;

	writeDebugStreamLine("Starting move to (%d, %d) at power %d", target.x, target.y, power);

	FieldPos startingPos;
	memcpy(startingPos, tRobot, sizeof(tRobot));

	//TODO: Make timelimit dynamic
	float timelimit = distanceBetween(tRobot, target)*50+1000; // set a time limit based on distance

#ifdef BALLE
	timelimit *= 3;
#endif

	ClearTimer(T1);

	float toGo = distanceBetween(tRobot, target);

	//TODO: Don't stop while still moving toward target.
	while(abs(neededTurn(target, forward)) <= PI/2)//  toGo > OCD)
	{
		toGo = distanceBetween(tRobot, target);

		float fwdpower = power;
		if(glide){
			const float accelLimit = 0.7; //Units of % / cm
			if (toGo < 30) {
				float cap = 10 + accelLimit * toGo;
				fwdpower = (cap < fwdpower) ? cap : fwdpower;
			}
		}
		#ifdef BALLE
			fwdpower += 10;
		#endif

		float angleError = neededTurn(target, forward);

		float turnCorrection = fwdpower * angleError; //Basic proportional control
		float mult = ((toGo)/20.0); //Additional multiplier ranges from 0 to 3, making late drive less jaggy
		if (mult > 3.0){
			mult = 3.0;
	}
		turnCorrection *=  mult;
		turnCorrection *= aggressiveness;

		//turnCorrection = 0; //Disables swerving correction
		//writeDebugStreamLine("Turn Correction = %f", turnCorrection);

		float lpow = fwdpower - turnCorrection;
		float rpow = fwdpower + turnCorrection;

		if(forward == Backward){ //invert front
			float temp;
			temp = lpow;
			lpow = -rpow;
			rpow = -temp;
		}

		//if(glide){
		//	vmot(lpow, rpow);
		//}
		//else{
			mot2(lpow, rpow);
		//}
		if(time1[T1] > timelimit) // timeout if stalled
		{
			writeDebugStreamLine("Move timed out with %fcm to go.", toGo);
			mot2(0, 0);
			return;
		}

	}
	mot2(0,0);

	writeDebugStreamLine("Move completed with miss amount of %f (cm)", distanceBetween(tRobot, target));

	return;


}

float neededTurn(FieldPos target, DrivingDirection forward){
	return coerceAngle(angleBetween(tRobot, target) - tRobot.theta + (forward ? 0:PI)); //Negates if robot intends to drive backward
}

void turnTo (FieldPos target, int power, DrivingDirection forward)
{
	forward = autoSelectDirection(forward, target);
#ifdef BALLE
	power += 20;
#endif

#ifdef BALLE
	const float UNDERSHOOT = 20*PI/180.0; //30 degrees, amount by which to intentionally undershoot target.
#else
	const float UNDERSHOOT = 6*PI/180.0; //6 degrees, amount by which to intentionally undershoot target.
#endif
	power = abs(power);
	bool counterclockwise = neededTurn(target, forward) > 0;
	const int timeout = 10000; //10seconds

	if( !counterclockwise )
		power = -1 * power;
	ClearTimer(T1);

	writeDebugStreamLine("Needed turn (deg): %f", radiansToDegrees(neededTurn(target, forward)));

	while(true) // turn until the gyro is correct
	{
		mot2(-power, power);
		float angleDiff = neededTurn(target, forward);
		if(time1[T1] > timeout){ // timeout if stalled
			writeDebugStreamLine("Turn timed out");
			break;
		}
		if (!counterclockwise && angleDiff >= -UNDERSHOOT && angleDiff < PI/2){
			writeDebugStreamLine("Clockwise turn finished");
			break;
		}
		if (counterclockwise && angleDiff <= UNDERSHOOT && angleDiff > -PI/2){
			writeDebugStreamLine("Counterclockwise turn finished");
			break;
		}
		//writeDebugStreamLine("gyro is reading %f" , gyroVal);
		wait1Msec(2);
	}
	writeDebugStreamLine("Turn completed with miss amount of %f (deg)", radiansToDegrees(neededTurn(target, forward)));

	mot2(0, 0);
}

void turnToHeading(float heading, int power, DrivingDirection forward){
	FieldPos p;
	p.x = cos(heading);
	p.y = sin(heading);
	p.theta = 0;
	p.x *= 100000000.0; p.y *= 100000000.0; //create point a LONG way away.
	turnTo(p, power, forward);
}


void turnAndMoveTo (RelativePos target, int power, DrivingDirection forward){
	FieldPos target2;
	translate(target, target2);
	turnAndMoveTo(target2, power, forward);
}

void turnTo (RelativePos target, int power, DrivingDirection forward){
	FieldPos target2;
	translate(target, target2);
	turnTo(target2, power, forward);
}

void moveTo (RelativePos target, int power, DrivingDirection forward){
	FieldPos target2;
	translate(target, target2);
	moveTo(target2, power, forward);
}
