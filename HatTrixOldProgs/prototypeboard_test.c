#pragma config(Sensor, S1,     board,          sensorI2CCustomFastSkipStates9V)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "C:\Program Files (x86)\Robomatter Inc\ROBOTC Development Environment\Sample Programs\NXT\3rd Party Sensor Drivers\drivers\hitechnic-protoboard.h"

task main()
{

	clearDebugStream();
	HTPBsetupIO(S1, 0x00);

	while(1)
	{
		int value = HTPBreadIO(S1, 0x4C);
		wait10Msec(50);
		writeDebugStreamLine(" board value: %d" , value);
}

}