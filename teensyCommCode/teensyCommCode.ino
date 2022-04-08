
// TEENSY

#include <SPI.h>

#define RX 1
#define TX 0

/*
 * TODO:
 * Write communication code (UART and I2C) between the Teensy and ESP32.
 * Software will write the ESP <-> NUC code (We do not need to write this)
 * EE will write the communication between the Teensy <-> ESP32
 * Software has given us what they will be sending from the NUC -> ESP32, so we will follow that template 
 */

unsigned long data[5] = {0x00, 0xA1, 0xB2, 0xC3, 0xD4};

void setup() {
  // Note the format for setting a serial port is as follows: Serial2.begin(baud-rate, protocol, RX pin, TX pin);
  Serial.begin(115200);

  //Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial2.begin(9600, SERIAL_8N1, RX, TX);

}

void loop() {
  int i;
  if (Serial.available()) {
    Serial.write(data[i], sizeof(long));
  }

}
