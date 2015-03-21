
FieldPos GPS_floorStartingPosition;
FieldPos GPS_floorStartingPositionUS;
FieldPos GPS_awayFromWall;
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

void set(FieldPos *pos, float x, float y, float theta = 0){
	pos->x = x; pos->y =  y; pos->theta = theta;
}

void set(RelativePos *pos, float x, float y, float theta = 0){
	pos->x = x; pos->y =  y; pos->theta = theta;
}

void initPositions(){
	set(GPS_floorStartingPositionUS, 10, 198, degreesToRadians(180));
	set(GPS_awayFromWallUS, 50, 195);
	//GPS_floorStartingPositionUS.x = 10; GPS_floorStartingPositionUS.y = 198.0; GPS_floorStartingPositionUS.theta = degreesToRadians(180);
	//GPS_awayFromWallUS.x = 50; GPS_awayFromWallUS.y = 195.0; GPS_awayFromWallUS.theta = 0;

	set(GPS_mediumGoalPosition, 271, 20);
	set(GPS_navPoint1, 271, 255);
	// GPS_mediumGoalPosition.x = 271; GPS_mediumGoalPosition.y = 20; GPS_mediumGoalPosition.theta = 0;
	// GPS_navPoint1.x = 271; GPS_navPoint1.y = 255; GPS_navPoint1.theta = 0;

	set(GPS_defendPoint, 80, 320);
	// GPS_defendPoint.x = 80; GPS_defendPoint.y = 320; GPS_defendPoint.theta = 0;

	set(GPS_prepareForCenterDump, -99, 0);
	set(GPS_centerDumpPosition1, -65, 8);
	set(GPS_centerDumpPosition2, -61, 8);
	set(GPS_centerDumpPosition3, -63, 4);
	// GPS_prepareForCenterDump.x = -99; GPS_prepareForCenterDump.y = 0;
	// GPS_centerDumpPosition1.x = -65; GPS_centerDumpPosition1.y = 8;
	// GPS_centerDumpPosition2.x = -61; GPS_centerDumpPosition2.y = 8;
	// GPS_centerDumpPosition3.x = -63; GPS_centerDumpPosition3.y = 4;
	set(GPS_prepareForKickstand, -85, -35);
	set(GPS_hitKickstand, 10, -35);
	// GPS_prepareForKickstand.x = -85; GPS_prepareForKickstand.y = -35;
	// GPS_hitKickstand.x = 10; GPS_hitKickstand.y = -35;
}
