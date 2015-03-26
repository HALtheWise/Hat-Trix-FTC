FieldPos GPS_floorStartingPositionUS;
FieldPos GPS_awayFromWallUS;
FieldPos GPS_mediumGoalPosition;
FieldPos GPS_navPoint1;
FieldPos GPS_defendPoint;


RelativePos GPS_prepareForCenterDump;
RelativePos GPS_centerDumpPosition1;
RelativePos GPS_centerDumpPosition2;
RelativePos GPS_centerDumpPosition3;
RelativePos GPS_prepareForKickstand;
RelativePos GPS_hitKickstand;
RelativePos GPS_prepareForKickstandReverse;
RelativePos GPS_hitKickstandReverse;

RelativePos ROBOT_dumpRelativePos; //Warning: these are robot-relative, not field-relative
RelativePos ROBOT_nullRelativePos;

void set(FieldPos *pos, float x, float y, float theta = 0){	// Utility function
	pos->x = x; pos->y =  y; pos->theta = theta;
}

void set(RelativePos *pos, float x, float y, float theta = 0){	// Utility function
	pos->x = x; pos->y =  y; pos->theta = theta;
}

void initPositions(){
	set(GPS_floorStartingPositionUS, 10, 198, degreesToRadians(180)); //Where the robot starts the round
	set(GPS_awayFromWallUS, 50, 195); //Avoid knocking into the wall

	set(GPS_mediumGoalPosition, 271, 20);
	set(GPS_navPoint1, 271, 255); //Used for backside navigation

	set(GPS_defendPoint, 80, 320); //Location of opposing alliance's 60cm goal

	// These positions are defined relative to the tower, whatever orientation it may take.
	set(GPS_prepareForCenterDump, -99, 0);
	set(GPS_centerDumpPosition1, -40, 11); //Location of the 120cm "center" tube
	set(GPS_centerDumpPosition2, -36, 8);
	set(GPS_centerDumpPosition3, -38, 4);

	set(GPS_prepareForKickstand, -85, -35);
	set(GPS_prepareForKickstandReverse, 65, -35); //Unused, will be used to hit the kickstand after grabbing the 60 or 90cm goals
	set(GPS_hitKickstandReverse, -10, -35);

	// These positions are defined relative to the robot's drivetrain's center
	set(ROBOT_nullRelativePos, 0, 0);
	set(ROBOT_dumpRelativePos, -25, 0, 0);
}
