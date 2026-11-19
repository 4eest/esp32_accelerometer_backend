#include <Wire.h>
#include <Arduino.h>



void setup() {
  Serial.begin(115200);
  delay(1000);
  Wire.begin(21, 22);
  Serial.println("hello");
  
    // call chip and ask for the ID register
  Wire.beginTransmission(0x68);
  Wire.write(0x75);
  Wire.endTransmission(false);

  // get the byte back
  Wire.requestFrom(0x68, 1);
  uint8_t check = Wire.read();

  if (check == 0xFF || check == 0x00) {
    // 0xFF or 0x00 means nobody answered
    Serial.println("sensor not found, check wiring");
  } else {
    // real answer, print it (should be 68)
    Serial.print("check: 0x");
    Serial.println(check, HEX);
  }
}

void loop() {
 
}