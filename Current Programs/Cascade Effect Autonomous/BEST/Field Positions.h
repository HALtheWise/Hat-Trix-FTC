FieldPos GPS_floorStartingPositionUS;
FieldPos GPS_awayFromWallUS;
FieldPos GPS_mediumGoalPosition;
FieldPos GPS_tallGoalPosition;
FieldPos GPS_navPoint1;
FieldPos GPS_navPoint2;
FieldPos GPS_navPoint3;
FieldPos GPS_navPoint4;
FieldPos GPS_navPoint5;
FieldPos GPS_defendPoint;


RelativePos GPS_prepareForCenterDump;
RelativePos GPS_centerDumpPosition1;
RelativePos GPS_centerDumpPosition2;
RelativePos GPS_centerDumpPosition3;
RelativePos GPS_prepareForKickstand;
RelativePos GPS_hitKickstand;

RelativePos ROBOT_dumpRelativePos; //Warning: these are robot-relative, not field-relative
RelativePos ROBOT_nullRelativePos;

void set(FieldPos *pos, float x, float y, float theta = 0){
	pos->x = x; pos->y =  y; pos->theta = theta;
}

void set(RelativePos *pos, float x, float y, float theta = 0){
	pos->x = x; pos->y =  y; pos->theta = theta;
}

void initPositionsRamp();

void initPositions(){
	set(GPS_floorStartingPositionUS, 10, 198, degreesToRadians(180));
	set(GPS_awayFromWallUS, 50, 195);

	set(GPS_mediumGoalPosition, 326, 20); //X coordinate should be 291, but white lies are sort of OK.
	set(GPS_tallGoalPosition, 355, 90);

	set(GPS_navPoint1, 291, 255); //Between their ramp and the center structure, for backside navigation
	set(GPS_navPoint2, 350, 220);
	set(GPS_navPoint3, 350, 150);
	set(GPS_navPoint4, 291, 90);

	set(GPS_navPoint5, 125, 130); //Between our ramp and the center structure, for frontside navigation

	set(GPS_defendPoint, 80, 320);

	set(GPS_prepareForCenterDump, -99, 20);
	set(GPS_centerDumpPosition1, -37, 22);
	set(GPS_centerDumpPosition2, -31, 24);
	set(GPS_centerDumpPosition3, -32.5, 6);

	set(GPS_prepareForKickstand, -85, -35);
	set(GPS_hitKickstand, 10, -35);


	set(ROBOT_nullRelativePos, 0, 0);
	set(ROBOT_dumpRelativePos, -25, 0, 0);
	initPositionsRamp();
}

FieldPos GPS_rampStartingPosition;
FieldPos GPS_offRamp;
FieldPos GPS_mediumGoalPositionRamp;
FieldPos GPS_almostMediumGoalPositionRamp;

FieldPos GPS_parkGoal;

RelativePos GPS_prepareForKickstandReverse;
RelativePos GPS_hitKickstandReverse;
RelativePos GPS_defendPointRamp;


void initPositionsRamp(){
	set(GPS_rampStartingPosition,
	34.1 - 12.8, 	//Robot distance from wall when aligned to front of platform, minus 5inches due to the slope of the ramp and trigonometry.
	60, 		//12in, on ramp half way from wall. This is supposed to be 30.2, but tweaked to make things work.
	degreesToRadians(180));

	set(GPS_offRamp, 155, GPS_rampStartingPosition.y);

	set(GPS_mediumGoalPositionRamp, 270, 30);
	set(GPS_almostMediumGoalPositionRamp, GPS_mediumGoalPositionRamp.x - 25, GPS_mediumGoalPositionRamp.y);

	set(GPS_defendPointRamp, 80, -10);
	set(GPS_prepareForKickstandReverse, 65, -35);
	set(GPS_hitKickstandReverse, -10, -35);

	set(GPS_parkGoal, 30, 155);
}
