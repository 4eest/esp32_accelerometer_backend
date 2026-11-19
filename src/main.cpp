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

// reads accel (g) and gyro (degrees per second)
void readSensor() {
  // read 14 bytes starting at 0x3B: accel (6), temp (2), gyro (6)
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(0x68, 14);

  // accel 
  uint8_t xFirst  = Wire.read();
  uint8_t xSecond = Wire.read();
  int16_t xWhole  = (xFirst << 8) | xSecond;

  uint8_t yFirst  = Wire.read();
  uint8_t ySecond = Wire.read();
  int16_t yWhole  = (yFirst << 8) | ySecond;

  uint8_t zFirst  = Wire.read();
  uint8_t zSecond = Wire.read();
  int16_t zWhole  = (zFirst << 8) | zSecond;

  // temperature, skip it 
  Wire.read();
  Wire.read();

  //  gyro 
  uint8_t gxFirst  = Wire.read();
  uint8_t gxSecond = Wire.read();
  int16_t gxWhole  = (gxFirst << 8) | gxSecond;

  uint8_t gyFirst  = Wire.read();
  uint8_t gySecond = Wire.read();
  int16_t gyWhole  = (gyFirst << 8) | gySecond;

  uint8_t gzFirst  = Wire.read();
  uint8_t gzSecond = Wire.read();
  int16_t gzWhole  = (gzFirst << 8) | gzSecond;

  // ---- convert ----
  data.ax = xWhole / 16384.0;   // 16384 counts per g at +/-2g
  data.ay = yWhole / 16384.0;
  data.az = zWhole / 16384.0;

  data.gx = gxWhole / 131.0;    // 131 counts per deg/s at +/-250 deg/s
  data.gy = gyWhole / 131.0;
  data.gz = gzWhole / 131.0;
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
  readSensor();

  // accel in g
  Serial.print("accel: ");
  Serial.print(data.ax);
  Serial.print(", ");
  Serial.print(data.ay);
  Serial.print(", ");
  Serial.print(data.az);

  // gyro in degrees per second
  Serial.print("   gyro: ");
  Serial.print(data.gx);
  Serial.print(", ");
  Serial.print(data.gy);
  Serial.print(", ");
  Serial.println(data.gz);

  delay(500);
}