typedef enum {
	MODE_MEDIUM_ALWAYS=0,
	MODE_NO_MOVE=1,
	MODE_CENTER_ONLY=2,
	MODE_KICKSTAND_ALWAYS=3,

	NUMBER_AUTO_MODES=4, //Not an autonomous mode. All elements above this accessable from buttons.
	MODE_DEFEND_CENTER_MEDIUM=100, //DO NOT USE. Doesn't work.
} AutoMode;

char* autoModeString(AutoMode amode){
	switch (amode)
	{
	case MODE_NO_MOVE:
		return "Do Not Drive";
		break;

	case MODE_CENTER_ONLY:
		return "Center Goal Only";
		break;

	case MODE_MEDIUM_ALWAYS:
		return "Center Then Medium";
		break;

	case MODE_KICKSTAND_ALWAYS:
		return "Center Then Kickstand";
		break;

	case MODE_DEFEND_CENTER_MEDIUM:
		return "DO NOT USE!!!";
		writeDebugStreamLine("Defensive mode triggered.");
		break;

	default:
		writeDebugStreamLine("Unknown AutoMode translated: %d", amode);
		return "ERROR";
	}
}

AutoMode getAutoMode(){
	StopTask(displayDiagnostics);
	eraseDisplay();

	AutoMode amode = 0;
	while(nNxtButtonPressed != 3){ // As long as the center button isn't pressed
		if (nNxtButtonPressed == 1){ // Right button
			amode++;
			while(nNxtButtonPressed != -1){wait1Msec(10);}
		}
		if (nNxtButtonPressed == 2){ // Left button
			amode--;
			while(nNxtButtonPressed != -1){wait1Msec(10);}
		}
		amode = (AutoMode)(abs(amode % NUMBER_AUTO_MODES));
		nxtDisplayTextLine(3, "%d%s    ", amode, autoModeString(amode));
		wait1Msec(10);


		getJoystickSettings(joystick);
		if (!joystick.StopPgm){
			break;
		}
	}
	while(nNxtButtonPressed == 3){}
	eraseDisplay();
	wait1Msec(500);
	while(nNxtButtonPressed != 3 && joystick.StopPgm){
		nxtDisplayCenteredBigTextLine(1, "Confirm:");
		nxtDisplayCenteredTextLine(4, "%s    ", autoModeString(amode));
		wait1Msec(10);
	}

	eraseDisplay();
	StartTask(displayDiagnostics);
	return amode;
}
