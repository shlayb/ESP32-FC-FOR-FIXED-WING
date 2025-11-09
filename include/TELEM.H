#ifndef TELEM_H
#define TELEM_H
#include <Arduino.h>
// #include <CONTROL.H>

class Telemetry {
public:
  void begin() {
    // use 16 and 17 for telemetry serial communication
    Serial1.begin(115200, SERIAL_8N1, 16, 17);
  }
  void printData();
};


void Telemetry::printData() {
  Serial1.print("ROLL:");
  Serial1.print(ahrs.getRoll());
  Serial1.print(",PITCH:");
  Serial1.print(ahrs.getPitch());
  Serial1.print(",YAW:");
  Serial1.print(ahrs.getYaw());
  Serial1.print(",THROTTLE:");
  Serial1.print(radio.getThrottle());
  Serial1.print(",YAW_CMD:");
  Serial1.print(radio.getYaw());
  Serial1.print(",MODE:");
  Serial1.print(radio.getMode());
  Serial1.print(",ARMED:");
  Serial1.println(radio.isArmed() ? "YES" : "NO");
}


#endif // TELEM_H