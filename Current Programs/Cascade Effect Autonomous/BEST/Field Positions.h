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

void set(FieldPos *pos, float x, float y, float theta = 0){
	pos->x = x; pos->y =  y; pos->theta = theta;
}

void set(RelativePos *pos, float x, float y, float theta = 0){
	pos->x = x; pos->y =  y; pos->theta = theta;
}

void initPositions(){
	set(GPS_floorStartingPositionUS, 10, 198, degreesToRadians(180));
	set(GPS_awayFromWallUS, 50, 195);

	set(GPS_mediumGoalPosition, 291, 20);
	set(GPS_navPoint1, 291, 255);

	set(GPS_defendPoint, 80, 320);

	set(GPS_prepareForCenterDump, -99, 0);
	set(GPS_centerDumpPosition1, -40, 11);
	set(GPS_centerDumpPosition2, -34, 12);
	set(GPS_centerDumpPosition3, -33, 6);

	set(GPS_prepareForKickstand, -85, -35);
	set(GPS_hitKickstand, 10, -35);
	set(GPS_prepareForKickstandReverse, 65, -35);
	set(GPS_hitKickstandReverse, -10, -35);


	set(ROBOT_nullRelativePos, 0, 0);
	set(ROBOT_dumpRelativePos, -25, 0, 0);
}
