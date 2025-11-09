#ifndef FW_CONTROL_H
#define FW_CONTROL_H

#include <Arduino.h>
#include <AHRS.H>
#include <RADIO.H>
#include <AKTUATOR.H>

#define MAX_ROLL 30.0
#define MIN_ROLL -30.0
#define MAX_PITCH 30.0
#define MIN_PITCH -30.0


RADIO radio;
AHRS ahrs;
AKTUATOR aktuator;

class FW_CONTROL {
private:
  struct gain_control{
    float kp_roll;
    float ki_roll;
    float kd_roll;
    float kp_pitch;
    float ki_pitch;
    float kd_pitch;
    float kp_yaw;
    float ki_yaw;
    float kd_yaw;
  };

  float pid_roll, pid_pitch, pid_yaw;
  float roll_cmd, pitch_cmd, yaw_cmd;
  float i_roll, i_pitch, i_yaw;
  gain_control gain;
public:
  void manual();
  void fbwa();
};

void FW_CONTROL::manual() {
  // Manual control implementation
  R_aileron.writeMicroseconds(radio.ch_roll);
  L_aileron.writeMicroseconds(radio.ch_roll);
  elevator.writeMicroseconds(radio.ch_pitch);
  rudder.writeMicroseconds(radio.ch_yaw);
  motor.writeMicroseconds(radio.ch_throttle);
};

void FW_CONTROL::fbwa() {
  roll_cmd = map(radio.ch_roll, 1000, 2000, MIN_ROLL, MAX_ROLL);
  pitch_cmd = map(radio.ch_pitch, 1000, 2000, MIN_PITCH, MAX_PITCH);
  yaw_cmd = radio.ch_yaw;

  // FBWA control implementation (PID control can be added here)
  float dt = 0.005; // 200Hz control loop
  float roll_error = roll_cmd - ahrs.getRoll();
  float pitch_error = pitch_cmd - ahrs.getPitch();
  float yaw_error = yaw_cmd - ahrs.getYaw();

  // update integral terms
  i_roll += roll_error * dt;
  i_pitch += pitch_error * dt;
  i_yaw += yaw_error * dt;

  pid_roll = (gain.kp_roll * roll_error) + (gain.ki_roll * i_roll) + (gain.kd_roll * ahrs.Gx);
  pid_pitch = (gain.kp_pitch * pitch_error) + (gain.ki_pitch * i_pitch) + (gain.kd_pitch * ahrs.Gy);
  pid_yaw = (gain.kp_yaw * yaw_error) + (gain.ki_yaw * i_yaw) + (gain.kd_pitch * ahrs.Gz);

  // limit integral windup
  i_roll = constrain(i_roll, -20.0, 20.0);
  i_pitch = constrain(i_pitch, -20.0, 20.0);
  i_yaw = constrain(i_yaw, -20.0, 20.0);

  // limit PID outputs
  pid_roll = constrain(pid_roll, -400.0, 400.0);
  pid_pitch = constrain(pid_pitch, -400.0, 400.0);
  pid_yaw = constrain(pid_yaw, -400.0, 400.0);

  // send commands to actuators
  R_aileron.writeMicroseconds(1500 + pid_roll);
  L_aileron.writeMicroseconds(1500 + pid_roll);
  elevator.writeMicroseconds(1500 + pid_pitch);
  rudder.writeMicroseconds(1500 + pid_yaw);
  motor.writeMicroseconds(radio.ch_throttle);
}
#endif