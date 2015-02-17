
FieldPos GPS_floorStartingPosition;
FieldPos GPS_floorStartingPositionUS;
FieldPos GPS_awayFromWall;
FieldPos GPS_awayFromWallUS;
FieldPos GPS_mediumGoalPosition;
FieldPos GPS_navPoint1;


CenterRelativePos GPS_prepareForCenterDump;
CenterRelativePos GPS_centerDumpPosition1;
CenterRelativePos GPS_centerDumpPosition2;
CenterRelativePos GPS_centerDumpPosition3;
CenterRelativePos GPS_prepareForKickstand;
CenterRelativePos GPS_hitKickstand;

void initPositions(){
	GPS_floorStartingPositionUS.x = 10; GPS_floorStartingPositionUS.y = 198.0; GPS_floorStartingPositionUS.theta = degreesToRadians(180);
	GPS_awayFromWallUS.x = 50; GPS_awayFromWallUS.y = 195.0; GPS_awayFromWallUS.theta = 0;

	GPS_mediumGoalPosition.x = 271; GPS_mediumGoalPosition.y = 20; GPS_mediumGoalPosition.theta = 0;
	GPS_navPoint1.x = 271; GPS_navPoint1.y = 255; GPS_navPoint1.theta = 0;

	GPS_prepareForCenterDump.x = -99; GPS_prepareForCenterDump.y = 0;
	GPS_centerDumpPosition1.x = -66; GPS_centerDumpPosition1.y = 7;
	GPS_centerDumpPosition2.x = -63; GPS_centerDumpPosition2.y = 7;
	GPS_centerDumpPosition3.x = -66; GPS_centerDumpPosition3.y = 3;
	GPS_prepareForKickstand.x = -85; GPS_prepareForKickstand.y = -35;
	GPS_hitKickstand.x = 10; GPS_hitKickstand.y = -35;
}
