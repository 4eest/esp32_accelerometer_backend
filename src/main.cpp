#include <Wire.h>
#include <Arduino.h>



void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  
   // call chip 
  Wire.beginTransmission(0x68);
  Wire.write(0x75);
  Wire.endTransmission(false);   

  uint8_t result = Wire.requestFrom(0x68, 1);

   if (result != 1) {
    // nobody answered, so don't bother reading
    Serial.println("sensor not found, check wiring");
  } else {
    // chip answered, so read the byte back
    uint8_t check = Wire.read();
    Serial.print("check: 0x");
    Serial.println(check, HEX);
  }

}

void loop() {
  Serial.println("hello");
  break;
}