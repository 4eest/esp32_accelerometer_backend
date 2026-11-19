#include <Wire.h>
#include <Arduino.h>

#define SDA_PIN 21
#define SCL_PIN 22
#define MPU_ADDR 0x68

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  
  // call chip 
  Wire.beginTransmission(0x75);
  Wire.write(0x68);
  Wire.endTransmission(false);    
  
  Wire.requestFrom(0x68, 1);   
  uint8_t check = Wire.read();      
  
  Serial.print("check: 0x");
  Serial.println(check, HEX);


}

void loop() {
  Serial.println("hello");
  delay(1000);
}