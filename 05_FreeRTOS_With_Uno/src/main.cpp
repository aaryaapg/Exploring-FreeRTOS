/* ______________________________________Libraries______________________________________ */
#include <Arduino.h>
//FreeRTOS
#include <Arduino_FreeRTOS.h>
//RFID
#include <SPI.h>
#include<MFRC522.h>
/* _______________________________________Macros________________________________________ */
/* _____________________________Declarations_and_Variables______________________________ */
//Tasks
TaskHandle_t taskRFID_handle = NULL;
TaskHandle_t taskReadSensors_handle = NULL;
int count = 0;
TaskHandle_t taskLogoutButton_handle = NULL;
TaskHandle_t taskLogoutProcedure_handle = NULL;
/* ________________________________________Tasks________________________________________ */
void taskLogoutProcedure(void * parameter){
  for(;;) {
    Serial.println("Exeute Logout Procedure");
    Serial.println("Going to sleep now");
}
}
void taskLogoutButton(void * parameter) {
  //vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println("Assume that button is pressed after a delay of 15s");
  vTaskDelay(15000 / portTICK_PERIOD_MS);
  Serial.println("Suspending ReadSensors Task");
  if(taskReadSensors_handle != NULL) {
    vTaskSuspend(taskReadSensors_handle);
  }
  Serial.println("Creating LogoutProcedure Task");
  xTaskCreate(
    taskLogoutProcedure,          /* Task function. */
    "taskLogoutProcedure",        /* String with name of task. */
    100,            /* Stack size in bytes. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    &taskLogoutProcedure_handle);            /* Task handle. */
  Serial.println("Suspending LogoutButton Task (Own)");
  vTaskSuspend( NULL );
}
void taskReadSensors(void * parameter){ 
  vTaskDelay(2000 / portTICK_PERIOD_MS);
  Serial.println("Reading Sensors");
  for(;;){
    Serial.print("Sensor count: ");
    Serial.println(count++);
    vTaskDelay(3000 / portTICK_PERIOD_MS);
  }
}
void taskRFID( void * parameter ){
  Serial.println("Inside RFID Task");
  Serial.println("Creating ReadSensors Task and LogoutButton Task");
  xTaskCreate(
    taskReadSensors,          /* Task function. */
    "taskReadSensors",        /* String with name of task. */
    100,            /* Stack size in bytes. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    &taskReadSensors_handle);            /* Task handle. */
  xTaskCreate(
    taskLogoutButton,          /* Task function. */
    "taskLogoutButton",        /* String with name of task. */
    100,            /* Stack size in bytes. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    &taskLogoutButton_handle);            /* Task handle. */
  Serial.println("Suspending RFID Task (Own)");
  vTaskSuspend( NULL );
}
/* ________________________________________Setup________________________________________ */
void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println("Inside Setup");
  Serial.println("Creating RFID Task");
  xTaskCreate(
    taskRFID,          /* Task function. */
    "taskRFID",        /* String with name of task. */
    100,            /* Stack size in bytes. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    &taskRFID_handle);            /* Task handle. */
}
/* ________________________________________Loop_________________________________________ */
void loop() {
  //delay(1000);
}