
// ESP32

#include <SPI.h>

#define RX 0
#define TX 1

/*
 * TODO:
 * Write communication code (UART and I2C) between the Teensy and ESP32.
 * Software will write the ESP <-> NUC code (We do not need to write this)
 * EE will write the communication between the Teensy <-> ESP32
 * Software has given us what they will be sending from the NUC -> ESP32, so we will follow that template 
 */

unsigned long data;

void setup() {

  // Note the format for setting a serial port is as follows: Serial2.begin(baud-rate, protocol, RX pin, TX pin);
  Serial.begin(115200);

  //Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial2.begin(9600, SERIAL_8N1, RX, TX);
  
  // Serial.println("Serial Txd is on pin: "+String(TX));
  // Serial.println("Serial Rxd is on pin: "+String(RX));
}

void loop() {
  // put your main code here, to run repeatedly:

  if (Serial.available()) {
    Serial.read(&dataByte, sizeof(long));
  }

  switch (dataByte) {
    case 0x00:
      Serial.println("call full stop function...");
    case 0xA1:
      Serial.println("call forward function...");
    case 0xB2:
      Serial.println("call backward function...");
    case 0xC3:
      Serial.println("call turn left function...");
    case 0xD4:
      Serial.println("call turn right function...");
    default:
      Serial.println("value doesn't match case :(");
  }

}