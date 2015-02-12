
#include "JoystickDriver.c"

task main()
{
	clearDebugStream();
	while(1)
	{
		wait10Msec(100);
		writeDebugStreamLine("X : %d" , joystick.joy1_x1);
		writeDebugStreamLine("Y : %d" , joystick.joy1_y1);
	}



}
