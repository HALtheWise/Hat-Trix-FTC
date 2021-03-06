#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Hubs,  S3, HTServo,  none,     none,     none)
#pragma config(Sensor, S2,     gyro,           sensorI2CHiTechnicGyro)
#pragma config(Sensor, S4,     HTSMUX,         sensorLowSpeed)
#pragma config(Motor,  motorA,          lateralSweep,  tmotorNXT, openLoop)
#pragma config(Motor,  motorB,          stuffer,       tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorC,          verticalSweep, tmotorNXT, openLoop, reversed)
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

#include "gyrostuff.h"
//---------------- Constants ----------------//

//------------- Global Variables ------------//
typedef struct {
	float x; // Measured in centimeters
	float y; // Measured in centimeters
	float theta; // Measured in counterclockwise radians
				// (0, 0, 0) is the position on the corner by the ramp facing down to the field.
				// (0, 0, pi/2) is in the same corner, facing toward the cliff
				// Positive x extends down the ramp and positive z extends off the cliff.
				// Robot positions are measured to the center of the robot's drivetrain and through the cannonical front.
} FieldPos;

void copy(FieldPos *from, FieldPos *to){
	to->x = from->x;
	to->y = from->y;
	to->theta = from->theta;
}

#include "Center Relative Positions.h"

FieldPos robot;

RelativePos navOffset; //
FieldPos tRobot; //This tracks the robot's position, but transformed by the navOffset

//---------- Function Declarations ----------//
void resetTracker();
float getGyro();
float getEncoder();
float angleBetween(FieldPos pos1, FieldPos pos2);
float distanceBetween(FieldPos pos1, FieldPos pos2);
float coerceAngle(float angle);

//----------- Function Definitions ----------//

float angleBetween(FieldPos pos1, FieldPos pos2){ //heading needed to go from pos1 to pos2
	float ang = atan2(pos2.y - pos1.y, pos2.x - pos1.x);
	return coerceAngle(ang);
}

float distanceBetween(FieldPos pos1, FieldPos pos2){ //
	return sqrt((pos1.x - pos2.x)*(pos1.x - pos2.x) + (pos1.y - pos2.y)*(pos1.y - pos2.y)); //Pythagorean Formula
}

float coerceAngle(float angle){
	while (angle <= -PI)  angle += 2*PI;
	while (angle >   PI)  angle -= 2*PI;
	return angle;
}

void updateTRobot(bool print = false){
	add(robot, navOffset, tRobot);
	if(print){
		writeDebugStreamLine("offset=(%d, %d, %d)", navOffset.x, navOffset.y, navOffset.theta);
	}
}

void resetTracker(){
	robot.x = 0;
	robot.y = 0;
	robot.theta = 0;
	navOffset.x = 0;
	navOffset.y = 0;
	navOffset.theta = 0;
	tRobot.x = 0;
	tRobot.y = 0;
	tRobot.theta = 0;

	nMotorEncoder[FrontL] = 0;
	nMotorEncoder[FrontR] = 0;
	resetGyro();
}

task dispTrack(){
	while(true){
		wait1Msec(500);
		writeDebugStreamLine("Track: (x, y, theta in deg) = (%d, %d, %.2f)", robot.x, robot.y, robot.theta*180/PI);
		if (robot.x != tRobot.x)
			writeDebugStreamLine("Offset Track: (x, y, theta in deg) = (%d, %d, %.2f)", tRobot.x, tRobot.y, tRobot.theta*180/PI);
	}
}

bool useGoodEncoderMeasurement = false;
float getEncoder(){ //Returns total average encoder distance in cm.
	const float SCALAROLD = 3125; // Motor clicks per meter. Used on last year's robot, incorrect by about 7%.
	const float SCALARNEW = 3356.47; // Motor clicks per meter. Measured on an acutal mat with two trials.
	float SCALAR2 = useGoodEncoderMeasurement ? SCALARNEW : SCALAROLD;
	return (nMotorEncoder[FrontL] + nMotorEncoder[FrontR])/2.0/SCALAR2*100;
}

float getGyro(){ //Returns the integrated gyro reading in counterclockwise radians
	updateGyro(false);
	return -gyroVal * PI / 180.0;
}

task trackRobot(){
	const float OVERESTIMATION_RATIO = 1+1/36.0; //The gyro seems to think it turns an extra ~10deg per full rotation.
	static float oldEncoder, oldGyro;

	//resetTracker();
	oldGyro = getGyro();
	oldEncoder = getEncoder();

	StartTask(dispTrack);
	while(true){
		wait1Msec(20); //50hz

		float dEncoder = getEncoder() - oldEncoder;
		oldEncoder += dEncoder;

		float dGyro = getGyro() - oldGyro;
		oldGyro += dGyro;

		dGyro /= OVERESTIMATION_RATIO;

		hogCPU(); //Protection against race conditions.
		robot.theta += dGyro;
		robot.theta = coerceAngle(robot.theta);

		robot.x += (cos(robot.theta)*dEncoder);
		robot.y += (sin(robot.theta)*dEncoder);
		updateTRobot();
		releaseCPU();
	}
}
