#pragma config(Sensor, S4,     sonar,          sensorSONAR)


//********************************************************************************************************
//*
//*	We don't know where "juliet" came into this, but...
//* julietUS() reads the ultrasonic sensor and attempts to determine the orientation of
//* the tower as an integer 1, 2, or 3. It reads from the GPS track so that it works from multiple
//* positions on the field. It is called once or twice, depending on the exact details of the field setup.
//*
//********************************************************************************************************
int julietUS(){
	int reading = SensorValue[sonar];
	int centerPosition = 0;

	float rPos = robot.x - 10.0;

	if(reading >= 130 - rPos && reading < 150 - rPos){ //Sees the long side of the tower
		centerPosition = 1;
	}
	else if(reading < 130 - rPos && reading > 110 - rPos){ //Sees the short side of the tower
		centerPosition = 3;
	}else{
		centerPosition = 2;
	}

	writeDebugStreamLine("DETECTED CENTER STRUCTURE POSITION %d", centerPosition);
	return centerPosition;
}

//********************************************************************************************************
//*
//*	This thread doesn't quite work yet, but we merged it into the judging branch anyway because
//* we thought it was interesting. It endeavors to constantly monitor the robot position, and identify
//* any time that the sonar just happens to be facing a wall. Then, if it thinks it sees the wall,
//* it will use that to update the robot's positon estimate.
//*
//********************************************************************************************************
task USmagic(){
	const float FIELD_SIZE = 365.76;

	RelativePos USrelativePos;
	FieldPos USabsolutePos
	set(USrelativePos, -6, 0, degreesToRadians(180))

	while(true){
		wait1msec(200){
			float hackedRobotAngle = coerceAngle(robot.theta*4);
			if (abs(hackedRobotAngle) > 4*degreesToRadians(10)){
				continue;
			}

			add(robot, USrelativePos, USabsolutePos);

			float expectedDistance;
			if (coerceAngle(USabsolutePos) < PI/4){ //US is facing positive x
				expectedDistance = FIELD_SIZE - USabsolutePos.x;
			}else if (coerceAngle(USabsolutePos - PI) < PI/4){ //US is facing negative x
				expectedDistance = USabsolutePos.x;
			}else if (coerceAngle(USabsolutePos - PI/2) < PI/4){ //US is facing positive y
				expectedDistance = FIELD_SIZE - USabsolutePos.y;
			}else {
				expectedDistance = USabsolutePos.y;
			}

			float reading = SensorValue[sonar];

			if (expectedDistance < 10 || abs(reading - expectedDistance) > 15){ //Either you are really close to the wall, or you are seeing something else
				continue;
			}

			if (coerceAngle(USabsolutePos) < PI/4){ //US is facing positive x
				USabsolutePos.x = FIELD_SIZE - reading;
			}else if (coerceAngle(USabsolutePos - PI) < PI/4){ //US is facing negative x
				USabsolutePos.x = reading;
			}else if (coerceAngle(USabsolutePos - PI/2) < PI/4){ //US is facing positive y
				USabsolutePos.y = FIELD_SIZE - reading;
			}else {
				USabsolutePos.y = reading;
			}

			subtract(USabsolutePos, USrelativePos, robot);
			wait1msec(3000); //Don't spam the reading by updating things too frequently.
		}		
	}
}