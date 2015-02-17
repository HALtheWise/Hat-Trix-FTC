const int cmndNone = -1;
const int elevatorUp = 0;
const int elevatorDown = 1;
const int elvStop = 2;
const int takeADump = 3;

const int grab = 4;
const int release = 5;
const int stopGrab = 6;

const int rollrStart = 7;
const int rollrStop = 8;
const int rollrRvsStrt = 9;

const int liftCar = 10;
const int lowerCar = 11;
const int stopCar = 12;

const int lessSpd = 13;
const int moreSpd = 14;
const int normalSpd = 15;

const int switchDrive = 16;

const int cancel = 17;

const int sweeperStart = 18;
const int sweeperStop = 19;
const int deploySweeper = 20;
const int deployStop = 21;
const int retractSweeper = 22;

int commandMap[][] = {
{cmndNone, cmndNone, cmndNone} ,	// JOY1_BLUE
{cmndNone, cmndNone, cmndNone} ,	// JOY1_GREEN
{cmndNone, cmndNone, cmndNone} ,	// JOY1_RED
{cmndNone, cmndNone, cmndNone} , //JOY1_YELLOW
{lessSpd, normalSpd, cmndNone} , //JOY1_LBUMP
{moreSpd, normalSpd, cmndNone} , //JOY1_RBUMP
{release, stopGrab, cmndNone} , //JOY1_LTRIG
{grab, stopGrab, cmndNone} , //JOY1_RTRIG
{cmndNone, cmndNone, cmndNone} , //JOY1_BACK
{cmndNone, cmndNone, cmndNone} , //JOY1_START
{sweeperStart, cmndNone, cmndNone} ,//JOY2_BLUE
{lowerCar, stopCar, cmndNone} ,//JOY2_GREEN
{sweeperStop, cmndNone, cmndNone} ,//JOY2_RED
{liftCar, stopCar, cmndNone} ,//JOY2_YELLOW
{takeADump, cmndNone, cmndNone} ,//JOY2_LBUMP
{rollrStop, cmndNone, cmndNone} ,//JOY2_RBUMP
{rollrRvsStrt, cmndNone, cmndNone} ,//JOY2_LTRIG
{rollrStart, cmndNone, cmndNone} ,//JOY2_RTRIG
{elevatorDown, elvStop, cmndNone} ,//JOY2_BACK
{elevatorUp, elvStop, cmndNone} ,//JOY2_START
};



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
