#pragma config(Sensor, S2,     seeker,         sensorI2CCustom)
#pragma config(Sensor, S4,     gyro,           sensorI2CHiTechnicGyro)
#pragma config(Motor,  motorA,          auxMotor,      tmotorNXT, openLoop)
#pragma config(Motor,  motorB,          FrontL,        tmotorNXT, openLoop, reversed, encoder)
#pragma config(Motor,  motorC,          FrontR,        tmotorNXT, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "GyroStuff.h"
#include "RobotTracker.h"

//---------------- Constants ----------------//
#define NO_STEERING 0.0

//------------- Global Variables ------------//
typedef enum{
	Forward = 1,
	Backward = 0,
} DrivingDirection;

//---------- Function Declarations ----------//
void	mot2	  (int leftPow, int rightPow);
void moveTo (FieldPos target, int power, DrivingDirection forward = Forward, float aggressiveness = 1.0);
void turnTo (FieldPos target, int power, DrivingDirection forward = Forward);
void turnToHeading (float heading, int power, DrivingDirection forward = Forward);
void turnAndMoveTo (FieldPos target, int power, DrivingDirection forward = Forward);
void grabGoal    ();
void releaseGoal ();
float neededTurn(FieldPos target, DrivingDirection forward = Forward);


//----------- Function Definitions ----------//
void grabGoal(){
	const int GRABBED_POSITION = 0;
	servo[grabberServo] = GRABBED_POSITION;
}

void releaseGoal(){
	const int RELEASED_POSITION = 70;
	servo[grabberServo] = RELEASED_POSITION;
}

void turnAndMoveTo (FieldPos target, int power, DrivingDirection forward){
	turnTo(target, power, forward);
	moveTo(target, power, forward);
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
	const bool glide = true; //This will eventually become an argument to moveTo().
	//const float TURN_AGGRESSIVENESS = 2.0;

	writeDebugStreamLine("Starting move to (%d, %d) at power %d", target.x, target.y, power);

	FieldPos startingPos;
	memcpy(startingPos, robot, sizeof(robot));

	//TODO: Make timelimit dynamic
	float timelimit = distanceBetween(robot, target)*50+1000; // set a time limit based on distance

#ifdef BALLE
	timelimit *= 3;
#endif

	ClearTimer(T1);

	float toGo = distanceBetween(robot, target);

	//TODO: Don't stop while still moving toward target.
	while(abs(neededTurn(target, forward)) <= PI/2)//  toGo > OCD)
	{
		toGo = distanceBetween(robot, target);

		float fwdpower = power;
		if(glide){
			const float accelLimit = 1.0; //Units of % / cm
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

	writeDebugStreamLine("Move completed with miss amount of %f (cm)", distanceBetween(robot, target));

	return;


}

float neededTurn(FieldPos target, DrivingDirection forward){
	return coerceAngle(angleBetween(robot, target) - robot.theta + (forward ? 0:PI)); //Negates if robot intends to drive backward
}

void turnTo (FieldPos target, int power, DrivingDirection forward)
{
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
