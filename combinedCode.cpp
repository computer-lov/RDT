#include <Servo.h>
/*
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
void setup() {
  Serial.begin(9600);
  oneNeo.attach(3);
  twoNeo.attach(4);
  threeNeo.attach(5);
  fourNeo.attach(6);
  // Test Functions
  turnLeft();
  delay(5000);
  deccelerate();
}
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
void loop() {
}