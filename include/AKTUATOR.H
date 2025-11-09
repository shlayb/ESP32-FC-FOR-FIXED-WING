#ifndef AKTUATOR_H
#define AKTUATOR_H

#include <ESP32Servo.h>

#define R_aileronPin 5
#define L_aileronPin 6
#define elevatorPin 9
#define rudderPin 10
#define motorPin 11

#define max_radio 2000
#define min_radio 1000

#define max_pwm 2000
#define min_pwm 1000


Servo R_aileron;
Servo L_aileron;
Servo elevator;
Servo rudder;
Servo motor;

class AKTUATOR {
public:
  void setupServos();
};

void AKTUATOR::setupServos() {
  R_aileron.attach(R_aileronPin);
  L_aileron.attach(L_aileronPin);
  elevator.attach(elevatorPin);
  rudder.attach(rudderPin);
  motor.attach(motorPin);

  R_aileron.writeMicroseconds(1500);
  L_aileron.writeMicroseconds(1500);
  elevator.writeMicroseconds(1500);
  rudder.writeMicroseconds(1500);
  motor.writeMicroseconds(1000); // Motor off
}


#endif // AKTUATOR_H