#ifndef AHRS_H
#define AHRS_H

#include <Wire.h>
#include <Arduino.h>

class AHRS {
private:
  const int MPU = 0x68; // MPU6050 I2C address
  int16_t tempAccX, tempAccY, tempAccZ;
  float AccX, AccY, AccZ;
  int16_t tempGyroX, tempGyroY, tempGyroZ;
  float accAngleX, accAngleY, gyroAngleX, gyroAngleY;
  float AccErrorX, AccErrorY, GyroErrorX, GyroErrorY, GyroErrorZ;
  unsigned long previousTime;
  const int numCalibrationSamples = 2000;
  const float alpha = 0.98; // Complementary filter constant
  float alertThreshold = 40;
  
  void readAcc();
  void readGyro();
  
public:
  float Gx, Gy, Gz;
  float roll, pitch, yaw;
  AHRS();  // Constructor
  void begin();  // Initialize the AHRS
  void update(); // Update sensor readings
  
  // Getter methods
  float getRoll() const { return roll; }
  float getPitch() const { return pitch; }
  float getYaw() const { return yaw; }
};

// Implementation
AHRS::AHRS() {
  AccErrorX = 0;
  AccErrorY = 0;
  GyroErrorX = 0;
  GyroErrorY = 0;
  GyroErrorZ = 0;
}

void AHRS::begin() {
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission(true);

  // Calibration
  for (int i = 0; i < numCalibrationSamples; i++) {
    Wire.beginTransmission(MPU);
    Wire.write(0x43);
    Wire.endTransmission(false);
    Wire.requestFrom((uint8_t)MPU, (uint8_t)6, (uint8_t)true);
    tempGyroX = (Wire.read() << 8 | Wire.read());
    tempGyroY = (Wire.read() << 8 | Wire.read());
    tempGyroZ = (Wire.read() << 8 | Wire.read());

    GyroErrorX += ((float)tempGyroX / 131.0);
    GyroErrorY += ((float)tempGyroY / 131.0);
    GyroErrorZ += ((float)tempGyroZ / 131.0);
  }
  GyroErrorX /= numCalibrationSamples;
  GyroErrorY /= numCalibrationSamples;
  GyroErrorZ /= numCalibrationSamples;
  
  roll = pitch = yaw = 0;
  previousTime = millis();
}

void AHRS::readAcc() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom((uint8_t)MPU, (uint8_t)6, (uint8_t)true);
  tempAccX = (Wire.read() << 8 | Wire.read());
  tempAccY = (Wire.read() << 8 | Wire.read());
  tempAccZ = (Wire.read() << 8 | Wire.read());

  AccX = ((float)tempAccX) / 16384.0;
  AccY = ((float)tempAccY) / 16384.0;
  AccZ = ((float)tempAccZ) / 16384.0;

  accAngleX = (atan2(AccY, sqrt(pow(AccX, 2) + pow(AccZ, 2))) * 180 / PI) - AccErrorX;
  accAngleY = (atan2(-AccX, sqrt(pow(AccY, 2) + pow(AccZ, 2))) * 180 / PI) + AccErrorY;
}

void AHRS::readGyro() {
  Wire.beginTransmission(MPU);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom((uint8_t)MPU, (uint8_t)6, (uint8_t)true);
  tempGyroX = (Wire.read() << 8 | Wire.read());
  tempGyroY = (Wire.read() << 8 | Wire.read());
  tempGyroZ = (Wire.read() << 8 | Wire.read());

  Gx = ((float)tempGyroX) / 131.0 - GyroErrorX;
  Gy = ((float)tempGyroY) / 131.0 - GyroErrorY;
  Gz = ((float)tempGyroZ) / 131.0 - GyroErrorZ;
}

void AHRS::update() {
  unsigned long currentTime = millis();
  float elapsedTime = (currentTime - previousTime) / 1000.0;
  previousTime = currentTime;

  readAcc();
  readGyro();

  gyroAngleX += Gx * elapsedTime;
  gyroAngleY += Gy * elapsedTime;

  roll = alpha * (roll + Gx * elapsedTime) + (1 - alpha) * accAngleX;
  pitch = alpha * (pitch + Gy * elapsedTime) + (1 - alpha) * accAngleY;
  yaw += Gz * elapsedTime;
}

#endif // AHRS_H