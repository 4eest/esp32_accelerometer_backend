#include <Arduino.h>
#include <Wire.h>

#define SDA_PIN 21
#define SCL_PIN 22
#define MPU_ADDR 0x68
#define ID_REG 0x75   // WHO_AM_I in the datasheet

// read one byte from a register, returns 0xFF if nobody answered
uint8_t readReg(uint8_t reg) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);
  // plain endTransmission() really sends on ESP32, so this check is trustworthy
  if (Wire.endTransmission() != 0) {
    return 0xFF;
  }
  Wire.requestFrom(MPU_ADDR, 1);
  return Wire.read();
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Wire.begin(SDA_PIN, SCL_PIN);
}

void loop() {
  Serial.println("========== debug ==========");

  // ---- 1. scan every address ----
  int found = 0;
  for (uint8_t addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.print("found device at 0x");
      Serial.println(addr, HEX);
      found++;
    }
  }
  if (found == 0) {
    Serial.println("no devices found");
  }

  // ---- 2. read the ID 3 times ----
  uint8_t a = readReg(ID_REG);
  uint8_t b = readReg(ID_REG);
  uint8_t c = readReg(ID_REG);
  Serial.print("id reads: 0x"); Serial.print(a, HEX);
  Serial.print(" 0x");          Serial.print(b, HEX);
  Serial.print(" 0x");          Serial.println(c, HEX);

  // ---- 3. verdict ----
  if (found == 0) {
    Serial.println("VERDICT: nothing on the bus. check wires, 21/22, power, ground");
  } else if (a != b || b != c) {
    Serial.println("VERDICT: reads keep changing. loose wire or no shared ground");
  } else if (a == 0x68) {
    Serial.println("VERDICT: all good, real MPU6050");
  } else if (a == 0x70 || a == 0x72 || a == 0x98) {
    Serial.println("VERDICT: working, clone chip");
  } else {
    Serial.println("VERDICT: steady but weird ID. check address (AD0) and wiring");
  }

  delay(2000);
}