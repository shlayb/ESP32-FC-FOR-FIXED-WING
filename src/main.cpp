#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <CONTROL.H>
#include <TELEM.H>


CONTROL control;
Telemetry telemetry;

// define Hz loop
#define loop_ahrs_Hz 100
#define loop_control_Hz 200
#define loop_radio_Hz 100
#define loop_telemetry_Hz 50
#define loop_debug_Hz 100


void printUSB();

// task for ahrs update
void ahrstask(void *pvParameters) {
  ahrs.begin();
  for (;;) {
    ahrs.update();
    vTaskDelay(pdMS_TO_TICKS(1000 / loop_ahrs_Hz));
  }
}

// task for control update
void controlUpdateTask(void *pvParameters) {
  aktuator.setupServos();
  for (;;) {
    control.control_loop();
    vTaskDelay(pdMS_TO_TICKS(1000 / loop_control_Hz));
  }
}

// radio update 
void radioTask(void *pvParameters) {
  radio.begin();
  for (;;) {
    radio.update();
    vTaskDelay(pdMS_TO_TICKS(1000 / loop_radio_Hz));
  }
}

// telemetry task
void telemetryTask(void *pvParameters) {
  telemetry.begin();
  for (;;) {
    telemetry.printData();
    vTaskDelay(pdMS_TO_TICKS(1000 / loop_telemetry_Hz));
  }
}

void printTask(void *pvParameters) {
  for (;;) {
    printUSB();
    vTaskDelay(pdMS_TO_TICKS(1000 / loop_debug_Hz));
  }
}


void setup() {
  Serial.begin(115200);
  xTaskCreate(ahrstask, "AhrsTask", 2048, NULL, 1, NULL);
  xTaskCreate(controlUpdateTask, "ControlUpdateTask", 2048, NULL, 1, NULL);
  xTaskCreate(radioTask, "RadioTask", 2048, NULL, 1, NULL); 
  xTaskCreate(telemetryTask, "TelemetryTask", 2048, NULL, 1, NULL);
}
void loop(){
  // skip loop
}

void printUSB() {
  Serial.print("ROLL:");
  Serial.print(ahrs.getRoll());
  Serial.print(",PITCH:");
  Serial.print(ahrs.getPitch());
  Serial.print(",YAW:");
  Serial.print(ahrs.getYaw());
  Serial.print(",THROTTLE:");
  Serial.print(radio.getThrottle());
  Serial.print(",YAW_CMD:");
  Serial.print(radio.getYaw());
  Serial.print(",MODE:");
  Serial.print(radio.getMode());
  Serial.print(",ARMED:");
  Serial.println(radio.isArmed() ? "YES" : "NO");
}