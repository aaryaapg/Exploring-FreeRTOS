/*
Connections
RC522         Uno
SDA  -------> Digital 10
SCK  -------> Digital 13
MOSI -------> Digital 11
MISO -------> Digital 12
IRQ  -------> unconnected
GND  -------> GND
RST  -------> Digital 9
3.3V -------> 3.3V        
*/

/* ______________________________________Libraries______________________________________ */
#include <Arduino.h>
//FreeRTOS
#include <Arduino_FreeRTOS.h>
//RFID
#include <SPI.h>
#include<MFRC522.h>
/* _______________________________________Macros________________________________________ */
//RFID
#define SS_PIN 10
#define RST_PIN 9
/* _____________________________Declarations_and_Variables______________________________ */
//Tasks
TaskHandle_t taskRFID_handle = NULL;
TaskHandle_t taskReadSensors_handle = NULL;
int count = 0;
TaskHandle_t taskLogoutButton_handle = NULL;
TaskHandle_t taskLogoutProcedure_handle = NULL;
//RFID
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;
byte uid[4]; // Init array that will store UID
String RFIDCode = ""; 
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
  for(;;) {
    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
      for (byte i = 0; i < 4; i++) {
        uid[i] = rfid.uid.uidByte[i];
      }
      //Get RFID card
      for (byte j = 0; j < rfid.uid.size; j++) {
        RFIDCode.concat(String(rfid.uid.uidByte[j] < 0x10 ? "0" : ""));
        RFIDCode.concat(String(rfid.uid.uidByte[j], HEX));
      }
      //Print RFID Tag
      RFIDCode.toUpperCase();
      RFIDCode=String(RFIDCode);
      Serial.print("RFID Code: "); Serial.println(RFIDCode);
      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
      //Turn Antenna off
      rfid.PCD_AntennaOff(); 
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
    else {
      //Serial.println("Please Scan");
    }
  }
}
/* ________________________________________Setup________________________________________ */
void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println("Inside Setup");
  //RFID
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
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