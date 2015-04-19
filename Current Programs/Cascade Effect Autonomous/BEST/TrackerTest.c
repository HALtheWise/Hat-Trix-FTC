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

//#include "BALL-E overrides.c"
#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.

#include "TrackerAutoLib.h"
#include "AutoLib.h"
#include "USstuff.h"
#include "Field Positions.h"



/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                    initializeRobot
//
// Prior to the start of tele-op mode, you may want to perform some initialization on your robot
// and the variables within your program.
//
// In most cases, you may not have to add any code to this function and it will remain "empty".
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

void initializeRobot()
{
	// Place code here to sinitialize servos to starting positions.
	// Sensors are automatically configured and setup by ROBOTC. They may need a brief time to stabilize.
	initPositions();
	useGoodEncoderMeasurement = true;
	clearDebugStream();
	gyroCal();
	nMotorEncoder[FrontL] = 0;
	nMotorEncoder[FrontR] = 0;
	//mot(20, 20);
	//wait1Msec(14000);
	//mot(0,0);
	//writeDebugStreamLine("Drive motor encoders: (%d, %d)", nMotorEncoder[FrontL], nMotorEncoder[FrontR]);

	StartTask(USmagic);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                         Main Task
//
// The following is the main code for the tele-op robot operation. Customize as appropriate for
// your specific robot.
//
// Game controller / joystick information is sent periodically (about every 50 milliseconds) from
// the FMS (Field Management System) to the robot. Most tele-op programs will follow the following
// logic:
//   1. Loop forever repeating the following actions:
//   2. Get the latest game controller / joystick settings that have been received from the PC.
//   3. Perform appropriate actions based on the joystick + buttons settings. This is usually a
//      simple action:
//      *  Joystick values are usually directly translated into power levels for a motor or
//         position of a servo.
//      *  Buttons are usually used to start/stop a motor or cause a servo to move to a specific
//         position.
//   4. Repeat the loop.
//
// Your program needs to continuously loop because you need to continuously respond to changes in
// the game controller settings.
//
// At the end of the tele-op period, the FMS will autonmatically abort (stop) execution of the program.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

void debugOutputs(){
	//writeDebugStreamLine("Sonars: (front, %d) || (side, %d)", USreadDist(frontSonar), getSideSensorReading());
	//writeDebugStreamLine("Drive motor encoders: (%d, %d)", nMotorEncoder[FrontL], nMotorEncoder[FrontR]);
}

void drive(){
	float lefty = 0;
	float righty = 0;
	lefty = righty = ((joystick.joy1_y2 *100)/128);


	lefty  +=   ((joystick.joy1_x1 *100)/128);
	righty +=  -((joystick.joy1_x1 *100)/128);

	if(false){ //invert front
		float temp;
		temp = lefty;
		lefty = -righty;
		righty = -temp;
	}



	const float accelLimit = 60;  //Limits the maximum change in wheel power per second. Units are "percent/second". 0 to disable. 60 is a good number.
	const float MIN_ACCEL_LIMIT_POW = 20; //Aceleration limiting only kicks in for powers less than this.

	if (accelLimit != 0){ //Acceleration limiting code
		static float lastAvgPow = 0.0;
		static int lastTime = nPgmTime;

		int timediff = (nPgmTime - lastTime);
		float dt = timediff / 1000.0; //Time since last loop iteration, in seconds
		lastTime = nPgmTime;
		float Avg = (lefty+righty)/2.0;
		if (dt > 0.0 && dt < 1.0) {
			float dAvg = (Avg) - lastAvgPow; //Units of "%"

			if (abs(dAvg/dt) > accelLimit && (abs(Avg) < MIN_ACCEL_LIMIT_POW)){ //Acceleration limit exceeded, taking over control.
				if (abs(lastAvgPow) > MIN_ACCEL_LIMIT_POW) {lastAvgPow = MIN_ACCEL_LIMIT_POW*sgn(lastAvgPow);}
				float diff = (righty-lefty)/2;
				Avg = lastAvgPow + sgn(dAvg) * accelLimit * dt;
				lefty = Avg - diff;
				righty = Avg + diff;
			}
		}
		lastAvgPow = Avg;
	}

	if(abs(lefty)  < 7) lefty  = 0;
	if(abs(righty) < 7) righty = 0;

	motor[FrontL] = lefty;
	motor[BackL] = lefty;
	motor[FrontR] = righty;
	motor[BackR] = righty;

}

task main()
{
	initializeRobot();

	waitForStart();   // wait for start of tele-op phase

	StartTask(trackRobot);

	while (true)
	{
		getJoystickSettings(joystick);
		drive();
		debugOutputs();
		wait1Msec(10);


		if (joy1Btn(3)){//Red button
			writeDebugStreamLine("Testing Stage 1 lift procedure");
			liftFirstStage();
		}
		if (joy1Btn(1)){//Blue button
			writeDebugStreamLine("Testing dump procedure");
			dumpBalls();
		}
		if (joy1Btn(2)){ //Green button

			mot2(0,0);
			FieldPos target;
			target.x = 149;
			target.y = 95;
			turnAndMoveTo(target, 50, Backward);

			target.x = 260;
			target.y = 0;
			turnAndMoveTo(target, 50, Backward);

			target.x = 149;
			target.y = -95;
			turnAndMoveTo(target, 50, Backward);

			target.x = 0;
			target.y = 0;
			turnAndMoveTo(target, 50, Backward);
			turnToHeading(0, 50);
		}
	}
}
