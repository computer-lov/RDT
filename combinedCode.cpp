#include <Servo.h>

/* 
// setup code for locomotion
#define forwardSpeed 1570
#define backwardSpeed 1430
#define zeroSpeed 1500
//1000 to go back at max speed, 1500 to not move, 2000 for max speed forward
//1575 for slow, catchable speed
*/
#define forwardSpeed 1600
#define backwardSpeed 1400
#define zeroSpeed 1500
// Motors starting from bottom left counter clockwise
Servo oneNeo;
Servo twoNeo;
Servo threeNeo;
Servo fourNeo;
int current1Speed = 1500;
int current2Speed = 1500;
int current3Speed = 1500;
int current4Speed = 1500;
int currentState = 0;
// State 0 = No movement
// State 1 = forward
// State 2 = backward
// State 3 = turning right
// State 4 = turning left

/*
  setup code for excavation
  RDT_EXCAV

  This code contains the functions nessesary to run Neo Motors via PWM

  ComponentsL
  * 6 motors
  * 12 Hall Effect Sensors
  * 4 Load Cells
  *

  Created 09 April 2022
  By Beatriz Perez (bp1590@nyu.edu)
  Modified day month year
  By author's name
*/

/*
  Neo Motors w/ SPARK MAX Motor Controller:
  * Input: PWM
  * Values: 1000-2000 us
  * 2000-1501 spins motor forward
  * 1000-1499 spins motor backward
  * 1500 stops motor
  */

 // Motor Values
  #define fullForward 2000
  #define fullBackward 1000
  #define zeroSpeed 1500
  #define startForward 1515
  #define startBackward 1485


/*
  Motor Layout
  6 Motors 
 
                TOP
               AUGER 
*           ╓--------╖
*           ║        ║ 
*           ║        ║
*         ╓-╨--------╨╖
* Linear  ║║         ║║   Linear 
* Purple  ║║         ║║   Red
*         ║║         ║║
*         ║║         ║║
*         ║║         ║║
*         ║║         ║║
*         ╙-╥-------╥-╜
*  Pivot    ║        ║    Pivot
*  Purple   ╙--------╜    Red
              BOTTOM
        
        Bucket
            ╓--------╖
*           ║        ║ 
*           ║        ║
            ╙--------╜

    REFERNCE TO WHOLE ROBOT FRAME

               FRONT
*           ╓--------╖
*      Blue ║        ║ Green
*           ║        ║
*         ╓-╨--------╨-╖
*         ║            ║
*         ║            ║
*         ║            ║
*         ╟------------╢
*         ║            ║
*         ║   EE Box   ║
*         ╙-╥--------╥-╜
*           ║        ║
*    Purple ║        ║ Red
*           ╙--------╜
*              BACK


// Include Load cell + Hall Effect Sensor Placement

*/

// Motor Declaration

Servo augerNeo;
Servo linpurpleNeo;
Servo linredNeo;
Servo pivpurpleNeo;
Servo pivredNeo;
Servo bucketNeo;

/*
  Active Variables:
  * These variables are LIVE values of their respective motor
  

*/

int currentBlueSpeed = 1500;
int currentPurpleSpeed = 1500;
int currentState = 0;

unsigned char inByte;
unsigned char id;
unsigned char speedLevel;


// recieve code first
void setup() {
  Serial.begin(9600);
  Serial1.begin(115200);
  
  unsigned char commands[2] = {'A', '1'}; // should we read in array here?
  
  unsigned char id = (commands[0] - 0x37) << 4;
  unsigned char speed = (commands[1] - 0x30);
  unsigned char com = id|speed;

  unsigned char commands1[2] = {'1', '0'};
  
  unsigned char id1 = (commands1[0] - 0x30);
  unsigned char speed1 = (commands1[1] - 0x30);
  unsigned char com1 = id1|speed1;

  // locomotion
  Serial.begin(9600);
  oneNeo.attach(3);
  twoNeo.attach(4);
  threeNeo.attach(5);
  fourNeo.attach(6);

  // excavation
  Serial.begin(9600); // Establish serial communication between Development Board and Serial Monitor
  //Serial1.begin(115200); // Establish serial communication between Development Board and the ESP32 Development Board

  // Attaching motors to their pins
  linpurpleNeo.attach(1);
  linredNeo.attach(2);
  pivpurpleNeo.attach(3);
  pivredNeo.attach(4);
  bucketNeo.attach(5);
  augerNeo.attach(6);
}


// LOCOMOTION CODE ************************************************************


void forward() {
  while (current1Speed != forwardSpeed && current3Speed != backwardSpeed && current2Speed != forwardSpeed && current4Speed != backwardSpeed) {
    delay(50);
    oneNeo.writeMicroseconds(current1Speed);
    twoNeo.writeMicroseconds(current2Speed);
    threeNeo.writeMicroseconds(current3Speed);
    fourNeo.writeMicroseconds(current4Speed);
    current1Speed++;
    current2Speed++;
    current3Speed--;
    current4Speed--;
    Serial.print("Forward: ");
    Serial.print(current1Speed);
    Serial.print(", ");
    Serial.print(current2Speed);
    Serial.println(current3Speed);
    Serial.print(", ");
    Serial.print(current4Speed);
  }
  currentState = 1;
}
void backward() {
  while (current1Speed != backwardSpeed && current3Speed != forwardSpeed && current2Speed != backwardSpeed && current4Speed != forwardSpeed) {
    delay(50);
    oneNeo.writeMicroseconds(current1Speed);
    twoNeo.writeMicroseconds(current2Speed);
    threeNeo.writeMicroseconds(current3Speed);
    fourNeo.writeMicroseconds(current4Speed);
    current3Speed++;
    current4Speed++;
    current1Speed--;
    current2Speed--;
    Serial.print("backward: ");
    Serial.print(current1Speed);
    Serial.print(", ");
    Serial.print(current2Speed);
    Serial.println(current3Speed);
    Serial.print(", ");
    Serial.print(current4Speed);
  }
  currentState = 2;
}
void turnRight() {
   while (current1Speed != forwardSpeed && current3Speed != forwardSpeed && current2Speed != forwardSpeed && current4Speed != forwardSpeed) {
    oneNeo.writeMicroseconds(current1Speed);
    twoNeo.writeMicroseconds(current2Speed);
    threeNeo.writeMicroseconds(current3Speed);
    fourNeo.writeMicroseconds(current4Speed);
    current1Speed++;
    current2Speed++;
    current3Speed++;
    current4Speed++;
    Serial.print("Turn Right: ");
    Serial.print(current1Speed);
    Serial.print(", ");
    Serial.print(current2Speed);
    Serial.println(current3Speed);
    Serial.print(", ");
    Serial.print(current4Speed);
   }
   currentState = 3;
}
void turnLeft() {
  while (current1Speed != backwardSpeed && current3Speed != backwardSpeed && current2Speed != backwardSpeed && current4Speed != backwardSpeed) {
    oneNeo.writeMicroseconds(current1Speed);
    twoNeo.writeMicroseconds(current2Speed);
    threeNeo.writeMicroseconds(current3Speed);
    fourNeo.writeMicroseconds(current4Speed);
    current1Speed--;
    current2Speed--;
    current3Speed--;
    current4Speed--;
    Serial.print("Turn Left: ");
    Serial.print(current1Speed);
    Serial.print(", ");
    Serial.print(current2Speed);
    Serial.println(current3Speed);
    Serial.print(", ");
    Serial.print(current4Speed);
   }
   currentState = 4;
}
void deccelerate() {
  if (currentState == 1) { // forward
    while (current1Speed != zeroSpeed && current2Speed != zeroSpeed && current3Speed != zeroSpeed && current4Speed != zeroSpeed) {
      delay(50);
      oneNeo.writeMicroseconds(current1Speed);
      twoNeo.writeMicroseconds(current2Speed);
      threeNeo.writeMicroseconds(current3Speed);
      fourNeo.writeMicroseconds(current4Speed);
      current3Speed++;
      current4Speed++;
      current1Speed--;
      current2Speed--;
      Serial.print("Deccelerating: ");
      Serial.print(current1Speed);
      Serial.print(", ");
      Serial.print(current2Speed);
      Serial.println(current3Speed);
      Serial.print(", ");
      Serial.print(current4Speed);
    }
  }
  if (currentState == 2) { // backward
   while (current1Speed != zeroSpeed && current2Speed != zeroSpeed && current3Speed != zeroSpeed && current4Speed != zeroSpeed) {
      delay(50);
      oneNeo.writeMicroseconds(current1Speed);
      twoNeo.writeMicroseconds(current2Speed);
      threeNeo.writeMicroseconds(current3Speed);
      fourNeo.writeMicroseconds(current4Speed);
      current1Speed++;
      current2Speed++;
      current3Speed--;
      current4Speed--;
      Serial.print("Deccelerating: ");
      Serial.print(current1Speed);
      Serial.print(", ");
      Serial.print(current2Speed);
      Serial.println(current3Speed);
      Serial.print(", ");
      Serial.print(current4Speed);
    }
  }
  if (currentState == 3) { // turn right
    while (current1Speed != zeroSpeed && current2Speed != zeroSpeed && current3Speed != zeroSpeed && current4Speed != zeroSpeed) {
      delay(50);
      oneNeo.writeMicroseconds(current1Speed);
      twoNeo.writeMicroseconds(current2Speed);
      threeNeo.writeMicroseconds(current3Speed);
      fourNeo.writeMicroseconds(current4Speed);
      current3Speed--;
      current4Speed--;
      current1Speed--;
      current2Speed--;
      Serial.print("Deccelerating: ");
      Serial.print(current1Speed);
      Serial.print(", ");
      Serial.print(current2Speed);
      Serial.println(current3Speed);
      Serial.print(", ");
      Serial.print(current4Speed);
    }
  }
  if (currentState == 4) { // turn left
    while (current1Speed != zeroSpeed && current2Speed != zeroSpeed && current3Speed != zeroSpeed && current4Speed != zeroSpeed) {
      delay(50);
      oneNeo.writeMicroseconds(current1Speed);
      twoNeo.writeMicroseconds(current2Speed);
      threeNeo.writeMicroseconds(current3Speed);
      fourNeo.writeMicroseconds(current4Speed);
      current3Speed++;
      current4Speed++;
      current1Speed++;
      current2Speed++;
      Serial.print("Deccelerating: ");
      Serial.print(current1Speed);
      Serial.print(", ");
      Serial.print(current2Speed);
      Serial.println(current3Speed);
      Serial.print(", ");
      Serial.print(current4Speed);
    }
  }
  currentState = 0;
}

// EXCACVATION CODE *****************************************************
/*
 * 
 * The following functions will control the excavation system
 * 
 */

// used to rotate auger either 90 or 45 degrees
// 0 means 45 degrees, 1 means 90
void pivotAuger(int ispeed, int pos){
  if (pos == 0){
    // Degree is 45
    delay(1000);
    forward(pivpurpleNeo, ispeed);
    forward(pivredNeo, ispeed);
    delay(3000); // will change to hall effect sensor once added 
    stop(pivpurpleNeo);
    stop(pivredNeo);
    }
  else{
  // Degree is 90
    delay(1000);
    forward(pivpurpleNeo, ispeed);
    forward(pivredNeo, ispeed);
    delay(5000); // will change to hall effect sensor once added
    stop(pivpurpleNeo);
    stop(pivredNeo); 
  }
}

// used to rotate bucket in or out
// 0 means flipping in, 1 means flipping out
void moveBucket(int ispeed, int pos){
  // Bucket Flipping in
  if (pos == 0){
    delay(1000);
    forward(bucketNeo, ispeed);
    delay(3000);
    stop(bucketNeo);
  }
  else{
    delay(1000);
    backward(bucketNeo, ispeed);
    delay(3000);
    stop(bucketNeo);
  } 
  }

// used to move auger up or down
void moveAuger(int ispeed, int pos){
  // Moving to Top
  if (pos == 0){ 
    delay(1000);
    forward(linpurpleNeo, ispeed);
    forward(linredNeo, ispeed);
    delay(3000);
    stop(linpurpleNeo);
    stop(linredNeo); 
  }
  else{
    delay(1000);
    backward(linpurpleNeo, ispeed);
    backward(linredNeo, ispeed);
    delay(3000);
    stop(linpurpleNeo);
    stop(linredNeo); 
  }
}

// rotates Auger/Shovel
void rotateShovel(int ispeed, int direct){
  if (direct == 0){
    delay(1000); 
    backward(augerNeo, ispeed);
  }
  else{
    delay(1000); 
    forward(augerNeo, ispeed);
  }
}

// Stops auger from rotating
void shovelStop(){
  stop(augerNeo);
}
  
// Shakes the auger
void shovelShake(){
  for (int i = 0; i < 5; i++){
    forward(augerNeo, 10);
    delay(1000);
    shovelStop();
    backward(augerNeo, 10);
    shovelStop();
  }
  
}

void EMERGENCYSTOP(){
  stop(augerNeo);
  stop(linpurpleNeo);
  stop(linredNeo);
  stop(pivpurpleNeo);
  stop(pivredNeo);
  stop(bucketNeo);
}


void forward(Servo& motor, int inputSpeed){
  int motorSpeed = map(inputSpeed, 0, 100, startForward, fullForward);
  motor.writeMicroseconds(motorSpeed);
}

void backward(Servo& motor, int inputSpeed){
  int motorSpeed = map(inputSpeed, 0, 100, startBackward, fullBackward);
  motor.writeMicroseconds(motorSpeed);
}

void stop(Servo &motor){
  motor.writeMicroseconds(zeroSpeed);
}


void loop() {
  id = 0;
  speedLevel = 0;
  if (true){
  //if (Serial1.available() != 0) {
    //inByte = Serial1.read();

    id = (inByte & 0b11110000) >> 4;
    speedLevel = (inByte & 0b1111);

    Serial.println("Incoming Byte \t ID \t Speed");
    Serial.print(inByte, BIN);
    Serial.print("\t");
    Serial.print(id, HEX);
    Serial.print("\t");
    Serial.print(speedLevel, HEX);
    Serial.println();

  }
  
  switch (id) {
    // cases for locomotion
    case 0x11:
      // full stop
      deccelerate();
      break;

    case 0xA1:
      // forward
      foward();
      break;

    case 0xB1:
      // backward
      backward();
      break;

    case 0xC1:
      // turn left
      turnLeft();
      break;

    case 0xD1:
      // turn right
      turnRight();
      break;

    // cases for excavation
    case 0xE0:
      // Pivot Auger 45
      pivotAuger(10, 0);
      break;
      
    case 0xE1:
      // Pivot Auger 90
      pivotAuger(10, 1);
      break;
      
    case 0x20:
      // Flip Bucket In
      moveBucket(10, 0);
      break;

      case 0x21:
      // Flip Bucket Out
      moveBucket(10, 1);
      break;

      case 0x31:
      // Move Auger Up
      moveAuger(10, 0);
      break;

      case 0x41:
      // Move Auger Down
      moveAuger(10, 1);
      break;

      case 0x51:
      // Shovel Rotate Counterclockwise
      rotateShovel(10, 0);
      break;

      case 0x61:
      // Shovel Rotate Clockwise
      rotateShovel(10, 1);
      break;

      case 0x70:
      // Shovel Stop
      shovelStop();
      break;

      case 0x71:
      // Shovel Shake
      shovelShake();
      break;

      case 0x90:
      // STOP EVERYTHING
      EMERGENCYSTOP();
      break;
      
  }
}