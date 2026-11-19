#include <Wire.h>
#include <Arduino.h>


// data we send to the wifi side
struct sensorData {
  float ax;   // accel in g
  float ay;
  float az;
  float gx;   // gyro in degrees per second
  float gy;
  float gz;
};

sensorData data;   // the latest reading, any function can use it



// checks the sensor is there, returns true if found
bool checkSensor() {
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
    return false;
  } else {
    // real answer, print it (should be 68)
    Serial.print("check: 0x");
    Serial.println(check, HEX);
    return true;
  }
}

// wakes the chip up (it starts asleep)
void wakeSensor() {
  // write 0x00 to register 0x6B
  Wire.beginTransmission(0x68);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();
}

// reads x, y, z in g
void readAccel() {
  // read 6 bytes starting at 0x3B, turn them into g
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(0x68, 6);

  uint8_t xFirst  = Wire.read();
  uint8_t xSecond = Wire.read();
  int16_t xWhole  = (xFirst << 8) | xSecond;

  uint8_t yFirst  = Wire.read();
  uint8_t ySecond = Wire.read();
  int16_t yWhole  = (yFirst << 8) | ySecond;

  uint8_t zFirst  = Wire.read();
  uint8_t zSecond = Wire.read();
  int16_t zWhole  = (zFirst << 8) | zSecond;

  data.ax = xWhole / 16384.0;
  data.ay = yWhole / 16384.0;
  data.az = zWhole / 16384.0;
}



void setup() {
  Serial.begin(115200);
  delay(1000);
  Wire.begin(21, 22);
  Serial.println("hello");

  if (checkSensor()) {
    wakeSensor();
  }
}

void loop() {
  readAccel();

  Serial.print(data.ax);
  Serial.print(", ");
  Serial.print(data.ay);
  Serial.print(", ");
  Serial.println(data.az);

  delay(500);
}