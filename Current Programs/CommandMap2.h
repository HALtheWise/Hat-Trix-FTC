const int cmndNone = -1;
const int elevatorUp = 0;
const int elevatorDown = 1;
const int elvCenter = 2;
const int elvHigh = 3;
const int elvMedium = 4;
const int elvLow = 5;
const int elvStop = 22;
const int takeADump = 23;
const int armDown = 6;
const int armUp = 7;
const int nudgeArmDown = 26;
const int nudgeArmUp = 27;
const int armStop = 20;

const int grab = 8;
const int release = 21;
const int stopGrab = 26;

const int ladUp = 9;
const int ladDown = 10;
const int ladStop = 24;

const int rollrStart = 11;
const int rollrStop = 12;
const int rollrRvsStrt = 13;

const int extendLdr = 14;
const int stopExtension = 25;

const int lessSpd = 15;
const int moreSpd = 16;
const int normalSpd = 19;

const int switchDrive = 17;

const int cancel = 18;

//#define JOY1_BLUE 0
//#define JOY1_GREEN 1
//#define JOY1_RED 2
//#define JOY1_YELLOW 3
//#define JOY1_LBUMP 4
//#define JOY1_RBUMP 5
//#define JOY1_LTRIG 6
//#define JOY1_RTRIG 7
//#define JOY1_BACK 8
//#define JOY1_START 9

//#define JOY2_BLUE 10
//#define JOY2_GREEN 11
//#define JOY2_RED 12
//#define JOY2_YELLOW 13
//#define JOY2_LBUMP 14
//#define JOY2_RBUMP 15
//#define JOY2_LTRIG 16
//#define JOY2_RTRIG 17
//#define JOY2_BACK 18
//#define JOY2_START 19

int commandMap[][] = {
{cmndNone, cmndNone, cmndNone} ,	// JOY1_BLUE
{grab, cmndNone, cmndNone} ,	// JOY1_GREEN
{takeADump, cmndNone, cmndNone} ,	// JOY1_RED
{release, cmndNone, cmndNone} , //JOY1_YELLOW
{lessSpd, normalSpd, cmndNone} , //JOY1_LBUMP
{moreSpd, normalSpd, cmndNone} , //JOY1_RBUMP
{nudgeArmUp, armStop, cmndNone} , //JOY1_LTRIG
{nudgeArmDown, armStop, cmndNone} , //JOY1_RTRIG
{ladUp, ladStop, cmndNone} , //JOY1_BACK
{extendLdr, stopExtension, cmndNone} , //JOY1_START
{elvMedium, cmndNone, cmndNone} ,//JOY2_BLUE
{elevatorDown, elvStop, cmndNone} ,//JOY2_GREEN
{elvHigh, cmndNone, cmndNone} ,//JOY2_RED
{elevatorUp, elvStop, cmndNone} ,//JOY2_YELLOW
{armDown, cmndNone, cmndNone} ,//JOY2_LBUMP
{armUp, cmndNone, cmndNone} ,//JOY2_RBUMP
{takeADump, cmndNone, cmndNone} ,//JOY2_LTRIG
{rollrStart, cmndNone, cmndNone} ,//JOY2_RTRIG
{armStop, elvStop, cmndNone} ,//JOY2_BACK
{rollrStop, cmndNone, cmndNone} ,//JOY2_START
};
