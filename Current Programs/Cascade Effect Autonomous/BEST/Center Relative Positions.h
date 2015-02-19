
typedef struct {
	float x; //(0,0) is at the very center of the field. Positive x extends away from our color center goal.
	float y; //Positive y extends away from our color kickstand
} CenterRelativePos;

int centerPos = -1;

float getCenterAngle(){
	if (centerPos == 1){
		return degreesToRadians(-90);
		}else if (centerPos == 2){
		return degreesToRadians(-45);
		}else if (centerPos == 1){
		return 0;
		}else{
		writeDebugStreamLine("Warning: center position not detected yet!");
		return 0;
	}
}

void translate(CenterRelativePos input, FieldPos *result){
	writeDebugStreamLine("Input was %d, %d", input.x, input.y);
	const float FIELD_SIZE = 365.76;
	float angle = getCenterAngle(); //Heading of center structure with respect to field coords.
	result->x = FIELD_SIZE/2;
	result->y = FIELD_SIZE/2;
	result->theta = angle;

	result->x = result->x + input.x * cos(angle) - input.y * sin(angle);
	result->y = result->y + input.y * cos(angle) + input.x * sin(angle);

	writeDebugStreamLine("Output was %d, %d", result->x, result->y);
}


void turnAndMoveTo (CenterRelativePos target, int power, DrivingDirection forward = Forward){
	FieldPos target2;
	translate(target, target2);
	turnAndMoveTo(target2, power, forward);
}

void turnTo (CenterRelativePos target, int power, DrivingDirection forward = Forward){
	FieldPos target2;
	translate(target, target2);
	turnTo(target2, power, forward);
}

void moveTo (CenterRelativePos target, int power, DrivingDirection forward = Forward){
	FieldPos target2;
	translate(target, target2);
	moveTo(target2, power, forward);
}
