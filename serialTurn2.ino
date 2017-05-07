/*

  Moves the robot forwads, backswards, left or right using
  serial commands

  Dileepa Ranawake)

  April 2017
  Version 2.1

 */

int motorDelay;

byte left1,left2,left3,left4;

byte right1,right2,right3,right4;

float wheelDiameter = 68.5;
float stepsPerRevolution = 512;
float mmsPerStep = (wheelDiameter * 3.1416) / stepsPerRevolution;
float wheelDistance = 90;
float turnCircleDist = (wheelDistance * 3.1416);
float stepsPerTurn = turnCircleDist / mmsPerStep;

int moveCount;
int moveSteps;// number of steps the motor is to move
int angle;
int distanceInMM;
int distance;

int number = 0;
String direction = "";
String instruction = "";
String confirmationMessage = "";
int directionTrigger = 0;

String get_direction(void) {

  direction = "";
  Serial.print("\nType a direction and press enter\n\nA = Turn Left\nD = Turn Right\nW = Move Forward\nS = Move Backwards.\n");

  while(Serial.available() > 0) {  //flush the buffer to prevent junk data being passed anywhere if greater than 0
    Serial.read(); delay (100);
  }
  while(Serial.available() == 0) {/*do nothing*/}

  while(Serial.available() > 0) {

    byte incoming = Serial.read();
    String input = "";
    input = char(incoming);
    input.toUpperCase();

    if (input == "A" ||input == "D" ||input == "S" ||input == "W" ) { //looks for a value ( valid number - outside 0 -9) in the specified askii range

      direction = input;
      Serial.print ("\n\nThanks you've set the direction to: ");
      Serial.println (direction);
      directionTrigger = 1;
      while(Serial.available() > 0) {  //flush the buffer to prevent junk data being passed anywhere if greater than 0
        Serial.read(); delay (100);
      }
    }
    else {

      Serial.println ("\nInvalid entry! Try again.\n"); //if not valid number returns an error
      while(Serial.available() > 0) {  //flush the buffer to prevent junk data being passed anywhere if greater than 0
        Serial.read(); delay (100);
      }
    }
  }
  return direction;
}

int get_md_number(void) {

  Serial.print(instruction);
  number = 0;

  while(Serial.available() > 0) {  //flush the buffer to prevent junk data being passed anywhere if greater than 0
    Serial.read(); delay (100);
  }
  while(Serial.available() == 0) {/*do nothing*/}

  while(Serial.available() > 0) {

    byte incoming = Serial.read();

    if (incoming >= '0' && incoming <= '9') { //looks for a value ( valid number 0 -9) in the specified askii range

      number = (number * 10) + (incoming - '0');
      delay(50);
      //Serial.print (confirmationMessage);
      //Serial.println (number);
      directionTrigger = 0;
      }
    else {

      Serial.println ("\nInvalid entry! Try again.\n"); //if not valid number returns an error
      while(Serial.available() > 0) {  //flush the buffer to prevent junk data being passed anywhere if greater than 0
        Serial.read(); delay (100);
        directionTrigger = 1;
      }
    }
  }
  return number;
}


void turnAntiClockWise(){
  left1=4; left2=5; left3=6; left4=7;
  right1=8; right2=9; right3=10; right4=11;
}

void turnClockWise(){
  left1=7; left2=6; left3=5; left4=4;
  right1=11; right2=10; right3=9; right4=8;
}

void forwards(){
  left1=7; left2=6; left3=5; left4=4;
  right1=8; right2=9; right3=10; right4=11;
}

void reverse(){
  left1=4; left2=5; left3=6; left4=7;
  right1=11; right2=10; right3=9; right4=8;
}

void leftForwards(){
  left1=7; left2=6; left3=5; left4=4;
}

void leftReverse(){
  left1=4; left2=5; left3=6; left4=7;
}

void rightForwards(){
  right1=8; right2=9; right3=10; right4=11;
}

void rightReverse(){
  right1=11; right2=10; right3=9; right4=8;
}

int calculateDistanceSteps(float distanceInMM){
  return distanceInMM / mmsPerStep + 0.5;
}

int calculateAngleSteps (float angle){
  return stepsPerTurn / 360 * angle + 0.5;
}

void setup() {
  leftForwards();
  rightForwards();

  pinMode(left1,OUTPUT);
  pinMode(left2,OUTPUT);
  pinMode(left3,OUTPUT);
  pinMode(left4,OUTPUT);
  //digitalWrite(left1,HIGH);

  pinMode(right1,OUTPUT);
  pinMode(right2,OUTPUT);
  pinMode(right3,OUTPUT);
  pinMode(right4,OUTPUT);
  //digitalWrite(right1,HIGH);

  motorDelay=2000;
  angle = 0;
  moveCount=0;

  Serial.begin(9600);



}

void loop(){

  if (directionTrigger == 0) {
    direction = get_direction();
  }
  if (directionTrigger ==1){

      if (direction == "A" || direction == "D"){

        instruction = "\n\nEnter the angle you'd like to turn in degrees\n";
        confirmationMessage = "\n\nThanks you've set the angle in degrees to ";
        number = get_md_number();
        angle = number;

        if (number>0){
          Serial.print (confirmationMessage);
          Serial.println (number);
          Serial.print("\n\nmoving...\n\n");
          delay(1000);
        }

        if (direction == "A"){
          moveSteps = calculateAngleSteps(angle);
          turnAntiClockWise();
        }
        else if (direction == "D") {
          moveSteps = calculateAngleSteps(angle);
          turnClockWise();
        }
      }
      else if (direction == "W" || direction == "S") {

        instruction = "\n\nEnter the distance you'd like to move in mm\n";
        confirmationMessage = "\n\nThanks you've set the distance in mm to ";
        number = get_md_number();
        distanceInMM = number;

        if (number>0){
          Serial.print (confirmationMessage);
          Serial.println (number);
          Serial.print("\n\nmoving...\n\n");
          delay(1000);
        }

        if (direction == "W"){
          moveSteps = calculateDistanceSteps(distanceInMM);
          forwards();
        }
        else if (direction == "S") {
          moveSteps = calculateDistanceSteps(distanceInMM);
          reverse();
      }
    }
  }

  while (moveCount<moveSteps) {

    moveCount = moveCount + 1;

    digitalWrite(left2,HIGH);
    digitalWrite(right2,HIGH);
    delayMicroseconds(motorDelay);
    digitalWrite(left1,LOW);
    digitalWrite(right1,LOW);
    delayMicroseconds(motorDelay);
    digitalWrite(left3,HIGH);
    digitalWrite(right3,HIGH);
    delayMicroseconds(motorDelay);
    digitalWrite(left2,LOW);
    digitalWrite(right2,LOW);
    delayMicroseconds(motorDelay);
    digitalWrite(left4,HIGH);
    digitalWrite(right4,HIGH);
    delayMicroseconds(motorDelay);
    digitalWrite(left3,LOW);
    digitalWrite(right3,LOW);
    delayMicroseconds(motorDelay);
    digitalWrite(left1,HIGH);
    digitalWrite(right1,HIGH);
    delayMicroseconds(motorDelay);
    digitalWrite(left4,LOW);
    digitalWrite(right4,LOW);
    delayMicroseconds(motorDelay);
  }
  moveCount = 0;
  moveSteps = 0;
  angle = 0;
  distance = 0;
  distanceInMM = 0;
  instruction = "";
  confirmationMessage = "";
  digitalWrite(left1,LOW);
  digitalWrite(right1,LOW);
}
