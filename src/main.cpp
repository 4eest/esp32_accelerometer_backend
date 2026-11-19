#include <Wire.h>
#include <Arduino.h>

#define SDA_PIN 21
#define SCL_PIN 22
#define MPU_ADDR 0x68

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

}

void loop() {
  Serial.println("hello");
  delay(1000);
}