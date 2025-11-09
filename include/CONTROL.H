#ifndef CONTROL_H
#define CONTROL_H

#include <Arduino.h>
#include <FW_CONTROL.H>

FW_CONTROL fw_control;

class CONTROL {
public:
  void control_loop();
};

void CONTROL::control_loop() {
  if(radio.mode_now == 1) {
    fw_control.manual();
  } else if(radio.mode_now == 2) {
    fw_control.fbwa();
  }
}

#endif
