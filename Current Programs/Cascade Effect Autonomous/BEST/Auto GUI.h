typedef enum {
	MODE_MEDIUM_ALWAYS,
	MODE_NO_MOVE,
	MODE_CENTER_ONLY,
	MODE_KICKSTAND_ALWAYS,

	NUMBER_AUTO_MODES, //Not an autonomous mode. All elements above this accessable from buttons.
	MODE_DEFEND_CENTER_MEDIUM, //DO NOT USE. Doesn't work.
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
	while(nNxtButtonPressed != 2){ // As long as the center button isn't pressed
		if (nNxtButtonPressed == 1) amode--;
		if (nNxtButtonPressed == 3) amode++;
		amode = (AutoMode)(amode % NUMBER_AUTO_MODES);
		nxtDisplayCenteredTextLine(3, "    %s    ", autoModeString(amode));
		wait1Msec(10);
	}
	while(nNxtButtonPressed == 2){}
	eraseDisplay();
	wait1Msec(2000);
	while(nNxtButtonPressed != 2){
		nxtDisplayCenteredBigTextLine(1, "Confirm:");
		nxtDisplayCenteredTextLine(4, "    %s    ", autoModeString(amode));
		wait1Msec(10);
	}


	StartTask(displayDiagnostics);
	return amode;
}
