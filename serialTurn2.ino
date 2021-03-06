/*
  Moves the robot forwads, backswards, left or right
  and set speed useing serial commands.
  Has improved stop move in situ feature
  Has repeat last action feature
  Has dynamic change of instruction in motion
  Dileepa Ranawake)
  May 2017
  Version 2.8

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
int speed;
int repeatCount;

int number = 0;
String command = "";
String instruction = "";
String confirmationMessage = "";
int commandTrigger = 0;

String get_command(void) {

  command = "";
  Serial.print("\nType a command and press enter\n\nA = Turn Left.\nD = Turn Right.\nW = Move Forward.\nS = Move Backwards.\nX = Set speed.\nR = Repeat last action.\n");

  while(Serial.available() > 0) {  //flush the buffer to prevent junk data being passed anywhere if greater than 0

    Serial.read(); delay (100);
  }
  while(Serial.available() == 0) {/*do nothing*/}

  while(Serial.available() > 0) {

    byte incoming = Serial.read();
    String input = "";
    input = char(incoming);
    input.toUpperCase();

    if (input == "A" ||input == "D" ||input == "S" ||input == "W"||input == "X" ||input =="R") { //looks for a value ( valid number - outside 0 -9) in the specified askii range

      command = input;
      Serial.print ("\n\nThanks this command you've chosen is: ");
      Serial.println (command);
      commandTrigger = 1;
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
  return command;
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
      commandTrigger = 0;
      }
    else {

      Serial.println ("\nInvalid entry! Try again.\n"); //if not valid number returns an error
      while(Serial.available() > 0) {  //flush the buffer to prevent junk data being passed anywhere if greater than 0
        Serial.read(); delay (100);
        commandTrigger = 1;
      }
    }
  }
  return number;
}

int motorDelayCalc(void){
  if (number>10){
    number = number/number;
  }
  motorDelay = 2200-(1000*number/10);
  return motorDelay;
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

void move(){
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

//problem function - this should stop motion by making the while loop untrue - wasn't working becuase of the == instead of =
int stopMove(){
  moveCount=moveSteps;
  Serial.print("\n\nMovement has been stopped!\n\n");
  while(Serial.available() > 0) {  //flush the buffer to prevent junk data being passed anywhere if greater than 0
    Serial.read(); delay (100);
  }
}


void setup() {

  while(Serial.available() > 0) {  //flush the buffer to prevent junk data being passed anywhere if greater than 0
    Serial.read(); delay (100);
  }
  leftForwards();
  rightForwards();

  pinMode(left1,OUTPUT);
  pinMode(left2,OUTPUT);
  pinMode(left3,OUTPUT);
  pinMode(left4,OUTPUT);

  pinMode(right1,OUTPUT);
  pinMode(right2,OUTPUT);
  pinMode(right3,OUTPUT);
  pinMode(right4,OUTPUT);

  motorDelay=2000;
  angle = 0;
  moveCount=0;

  Serial.begin(9600);
}

void loop(){

  if (commandTrigger == 0) {
    command ="";
    number =0;
    command = get_command();
  }
  if (commandTrigger ==1){


    if (command == "A" || command == "D"){

      instruction = "\n\nEnter the angle you'd like to turn in degrees\n";
      confirmationMessage = "\n\nThanks you've set the angle in degrees to ";
      number = get_md_number();
      angle = number;

      if (number>0){
        Serial.print (confirmationMessage);
        Serial.println (number);
        Serial.print("\n\nType 'Q' and press enter to stop motion\n\nmoving...\n\n");
        delay(1000);
      }

      if (command == "A"){
        moveSteps = calculateAngleSteps(angle);
        turnAntiClockWise();
      }
      else if (command == "D") {
        moveSteps = calculateAngleSteps(angle);
        turnClockWise();
      }
    }
    else if (command == "W" || command == "S") {

      instruction = "\n\nEnter the distance you'd like to move in mm\n";
      confirmationMessage = "\n\nThanks you've set the distance in mm to ";
      number = get_md_number();
      distanceInMM = number;

      if (number>0){
        Serial.print (confirmationMessage);
        Serial.println (number);
        Serial.print("\n\nType 'Q' and press enter to stop motion\n\nmoving...\n\n");
        delay(1000);
      }

      if (command == "W"){
        moveSteps = calculateDistanceSteps(distanceInMM);
        forwards();
      }
      else if (command == "S") {
        moveSteps = calculateDistanceSteps(distanceInMM);
        reverse();
      }
    }
    else if (command == "X") {

      instruction = "\n\nEnter the speed you'd like to set from 0-10\n";
      confirmationMessage = "\n\nThanks you've set the speed to ";
      number = get_md_number();
      motorDelay = motorDelayCalc();
      Serial.print (confirmationMessage);
      Serial.println (number);
    }
    else if (command == "R"){
      Serial.print("\n\nRepeating last action\n\nmoving...\n\n");
      moveSteps=repeatCount;
      commandTrigger=0;
    }
  }
  repeatCount = moveCount;

  while (moveCount<moveSteps) {

    moveCount = moveCount + 1;

    move();
    if (Serial.available() > 0) {
        byte incoming = Serial.read();
        String input = "";
        input = char(incoming);
        input.toUpperCase();

        if (input == "Q") {
          stopMove();
        }
        else if (input == "A") {
          turnAntiClockWise();
        }
        else if (input == "D") {
          turnClockWise();
        }
        else if (input == "W") {
          forwards();
        }
        else if (input == "S") {
          reverse();
        }
        else if (input == "X") {
          instruction = "\n\nEnter the speed you'd like to set from 0-10\n";
          confirmationMessage = "\n\nThanks you've set the speed to ";
          number = get_md_number();
          motorDelay = motorDelayCalc();
          Serial.print (confirmationMessage);
          Serial.println (number);
        }
        else{
            while(Serial.available() > 0) {  //flush the buffer to prevent junk data being passed anywhere if greater than 0
            Serial.read(); delay (100);
        }
      }
    }
  }
  repeatCount=moveSteps;
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
