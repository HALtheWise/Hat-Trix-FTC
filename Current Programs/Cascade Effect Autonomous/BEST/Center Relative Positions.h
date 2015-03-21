
typedef struct {
	float x; //(0,0) is at the very center of the field. Positive x extends away from our color center goal.
			 // Positive x can alternately extend from the robot's center of rotation toward the "front"
	float y; //Positive y extends away from our color kickstand
	float theta;
} RelativePos;

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

void getCenterFieldPos(FieldPos *result){
	const float FIELD_SIZE = 365.76;
	float angle = getCenterAngle();
	result.x = FIELD_SIZE/2;
	result.y = FIELD_SIZE/2;
	result.theta = angle;
}

void add(FieldPos base, RelativePos relative, FieldPos *result){
	result.x = base.x;
	result.y = base.y;
	result.theta = base.theta;

	result->x = result->x + relative.x * cos(result.theta) - relative.y * sin(result.theta);
	result->y = result->y + relative.y * cos(result.theta) + relative.x * sin(result.theta);
	result->theta = result->theta + relative.theta;
}


void subtract(FieldPos sum, RelativePos relative, FieldPos *result){
	result.x = sum.x;
	result.y = sum.y;
	result.theta = base.theta - relative.theta;

	result->x = result->x - relative.x * cos(result.theta) + relative.y * sin(result.theta);
	result->y = result->y - relative.y * cos(result.theta) - relative.x * sin(result.theta);
}

void translate(RelativePos input, FieldPos *result){
	writeDebugStreamLine("Input was %d, %d", input.x, input.y);
	
	FieldPos centerAbsPos;
	getCenterFieldPos(centerAbsPos);
	add(centerAbsPos, input, result);

	writeDebugStreamLine("Output was %d, %d", result->x, result->y);
}


void turnAndMoveTo (RelativePos target, int power, DrivingDirection forward = Forward){
	FieldPos target2;
	translate(target, target2);
	turnAndMoveTo(target2, power, forward);
}

void turnTo (RelativePos target, int power, DrivingDirection forward = Forward){
	FieldPos target2;
	translate(target, target2);
	turnTo(target2, power, forward);
}

void moveTo (RelativePos target, int power, DrivingDirection forward = Forward){
	FieldPos target2;
	translate(target, target2);
	moveTo(target2, power, forward);
}
