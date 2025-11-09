#ifndef RADIO_H
#define RADIO_H

#include <Arduino.h>
#include <HardwareSerial.h>

#define SBUS_RX 33  

class RADIO {
private:
  HardwareSerial* SerialSBUS;
  static const uint8_t SBUS_PACKET_SIZE = 25;
  uint8_t sbusData[SBUS_PACKET_SIZE];
  int16_t channels[16];



  float outputScaler(uint16_t ch) {
    return 0.002 * ch - 3;  // Scale from 172-1811 to ~0-1
  }

public:
  bool signal_lost;
  int16_t arm, ch_roll, ch_pitch, ch_throttle, ch_yaw, ch_mode, ch_mode_backup, ch_vehicle_mode;
  bool arming;

  bool alt_hold;
  bool mode_fbwa;
  bool pos_hold;
  bool mode_manual;
  bool mode_fbwa_plane;
  bool mode_fbwb;
  bool transition_phase1;
  bool transition_phase2;
  bool transition_phase3;
  bool mode_vtol;
  bool mode_safety;
  bool mode_vtol_plane;

  int mode_now, prev_mode;
  RADIO() : SerialSBUS(new HardwareSerial(2)), // UART2: RX=GPIO16, TX=GPIO17
        alt_hold(false), mode_fbwa(false), pos_hold(false), mode_manual(false),
        mode_fbwa_plane(false), mode_fbwb(false), transition_phase1(false),
        transition_phase2(false), transition_phase3(false), mode_vtol(false),
        mode_safety(false), mode_vtol_plane(false) {}

  void begin() {
    Serial.begin(115200);
    SerialSBUS->begin(100000, SERIAL_8E2, SBUS_RX, -1, true);
    SerialSBUS->setRxInvert(true);
  }

  void update() {
    while (SerialSBUS->available() >= SBUS_PACKET_SIZE) {
      if (SerialSBUS->peek() != 0x0F) {
        SerialSBUS->read();
        continue;
      }

      SerialSBUS->readBytes(sbusData, SBUS_PACKET_SIZE);

      signal_lost = sbusData[23] & 0x04;
      bool failsafe = sbusData[23] & 0x08;

      if (sbusData[24] != 0x00) {
        continue;
      }

      decodeSbusChannels();
      if (!validateChannels()) {
        Serial.println("Invalid SBUS frame, skipping...");
        return;
      }

      updateChannels();
      updateMode();
    }
  }

  // Getters
  int16_t getRoll() const { return ch_roll; }
  int16_t getPitch() const { return ch_pitch; }
  int16_t getThrottle() const { return ch_throttle; }
  int16_t getYaw() const { return ch_yaw; }
  bool isArmed() const { return arming; }
  int getMode() const { return mode_now; }
  bool isSignalLost() const { return signal_lost; }

private:
  void decodeSbusChannels() {
    channels[0]  = ((sbusData[1]    | sbusData[2]  << 8) & 0x07FF);
    channels[1]  = ((sbusData[2] >> 3 | sbusData[3] << 5) & 0x07FF);
    channels[2]  = ((sbusData[3] >> 6 | sbusData[4] << 2 | sbusData[5] << 10) & 0x07FF);
    channels[3]  = ((sbusData[5] >> 1 | sbusData[6] << 7) & 0x07FF);
    channels[4]  = ((sbusData[6] >> 4 | sbusData[7] << 4) & 0x07FF);
    channels[5]  = ((sbusData[7] >> 7 | sbusData[8] << 1 | sbusData[9] << 9) & 0x07FF);
    channels[6]  = ((sbusData[9] >> 2 | sbusData[10] << 6) & 0x07FF);
    channels[7]  = ((sbusData[10] >> 5 | sbusData[11] << 3) & 0x07FF);
    channels[8]  = ((sbusData[12]    | sbusData[13] << 8) & 0x07FF);
    channels[9]  = ((sbusData[13] >> 3 | sbusData[14] << 5) & 0x07FF);
    channels[10] = ((sbusData[14] >> 6 | sbusData[15] << 2 | sbusData[16] << 10) & 0x07FF);
    channels[11] = ((sbusData[16] >> 1 | sbusData[17] << 7) & 0x07FF);
    channels[12] = ((sbusData[17] >> 4 | sbusData[18] << 4) & 0x07FF);
    channels[13] = ((sbusData[18] >> 7 | sbusData[19] << 1 | sbusData[20] << 9) & 0x07FF);
    channels[14] = ((sbusData[20] >> 2 | sbusData[21] << 6) & 0x07FF);
    channels[15] = ((sbusData[21] >> 5 | sbusData[22] << 3) & 0x07FF);
  }

  bool validateChannels() {
    for (int i = 0; i < 16; i++) {
      if (channels[i] < 172 || channels[i] > 1810) {
        return false;
      }
    }
    return true;
  }

  void updateChannels() {
    ch_roll     = constrain(map(channels[0], 172, 1810, 1000, 2000), 1000, 2000);
    ch_pitch    = constrain(map(channels[1], 172, 1810, 1000, 2000), 1000, 2000);
    ch_throttle = constrain(map(channels[2], 172, 1810, 1000, 2000), 1000, 2000);
    ch_yaw      = constrain(map(channels[3], 172, 1810, 1000, 2000), 1000, 2000);
    arm         = constrain(map(channels[4], 172, 1810, 1000, 2000), 1000, 2000);

    arming = arm > 1500;

    ch_mode         = map(channels[5], 172, 1810, 1000, 2000);
    ch_mode_backup  = map(channels[6], 172, 1810, 1000, 2000);
    ch_vehicle_mode = map(channels[7], 172, 1810, 1000, 2000);
  }

  void updateMode() {
    if (ch_mode <= 1000) {
      mode_now = 1;
    } else if (ch_mode <= 1512) {
      mode_now = 2;
    } else {
      mode_now = 3;
    }
  }
};

#endif