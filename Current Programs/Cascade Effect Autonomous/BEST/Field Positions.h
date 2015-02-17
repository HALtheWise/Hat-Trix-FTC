
FieldPos GPS_floorStartingPosition;
FieldPos GPS_floorStartingPositionUS;
FieldPos GPS_awayFromWall;
FieldPos GPS_mediumGoalPosition;
FieldPos GPS_navPoint1;


CenterRelativePos GPS_prepareForCenterDump;
CenterRelativePos GPS_centerDumpPosition;
CenterRelativePos GPS_prepareForKickstand;
CenterRelativePos GPS_hitKickstand;

void initPositions(){
	GPS_floorStartingPosition.x = 22.9; GPS_floorStartingPosition.y = 213.0; GPS_floorStartingPosition.theta = 0;
	GPS_floorStartingPositionUS.x = 22.9; GPS_floorStartingPositionUS.y = 189.0; GPS_floorStartingPositionUS.theta = degreesToRadians(180);
	GPS_awayFromWall.x = 50; GPS_awayFromWall.y = 213.0; GPS_awayFromWall.theta = 0;
	GPS_mediumGoalPosition.x = 271; GPS_mediumGoalPosition.y = 35; GPS_mediumGoalPosition.theta = 0;
	GPS_navPoint1.x = 271; GPS_navPoint1.y = 255; GPS_navPoint1.theta = 0;

	GPS_prepareForCenterDump.x = -80; GPS_prepareForCenterDump.y = 10;
	GPS_centerDumpPosition.x = -50; GPS_centerDumpPosition.y = 10;
	GPS_prepareForKickstand.x = -80; GPS_prepareForKickstand.y = -35;
	GPS_hitKickstand.x = 20; GPS_hitKickstand.y = -35;
}
