/*
Connections
RFID RC522    ESP32
SDA  -------> 15         
SCK  -------> 18
MOSI -------> 23
MISO -------> 19
IRQ  -------> NC
GND  -------> GND
RST  -------> 27
VCC  -------> 3.3V
Pushbutton -> 22
RGB LED          ESP32
Red     -------> 13
Anode   -------> 3.3V
Green   -------> 12
Blue    -------> 14

-> https://github.com/espressif/arduino-esp32/issues/595#issuecomment-413829444
-> https://gist.github.com/SaheblalBagwan/5e538a727a5f1b0c129da36786d67159
*/

/* ______________________________________Libraries______________________________________ */
#include <Arduino.h>
//RFID
#include <SPI.h>
#include<MFRC522.h>
//WD
#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"

/* _______________________________________Macros________________________________________ */
//RFID
#define SS_PIN 15
#define RST_PIN 27
//Button
#define buttonPin 22
//For RGB LEDs
#define RED 13
#define GREEN 12
#define BLUE 14
/* _____________________________Declarations_and_Variables______________________________ */
//RFID
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;
byte uid[4]; // Init array that will store UID
String RFIDCode = "";
//Button
int buttonState; //Logout will be initiated if button state is HIGH
//Tasks
TaskHandle_t taskRFID_handle = NULL;
TaskHandle_t taskReadSensors_handle = NULL;
TaskHandle_t taskLogoutButton_handle = NULL;

/* ______________________________________Functions______________________________________ */
//Function to get RGB colours
void RGB_color(bool r, bool g, bool b){
  digitalWrite(RED, r);
  digitalWrite(GREEN, g);
  digitalWrite(BLUE, b);
  delay(1000);
}
/* ________________________________________Tasks________________________________________ */
void taskLogoutButton(void * parameter){
  for(;;){
    buttonState = digitalRead(buttonPin);
    if(buttonState==HIGH){
      //Suspend taskReadSensors
      Serial.println("Suspending ReadSensors Task");
      if(taskReadSensors_handle != NULL) {
        vTaskSuspend(taskReadSensors_handle);
      }
      //Create tastLogoutProcedure
      Serial.println("Creating LogoutProcedure Task");
      //Suspend own task
      Serial.println("Suspending LogoutButton Task");
      vTaskSuspend( NULL );
    }
  }
}
void taskReadSensors(void * parameter){ 
  delay(1000);
  for(;;){
    Serial.println("Reading Sensors");
    vTaskDelay(3000 / portTICK_PERIOD_MS);
  }
  //Serial.println("Ending task 2");
  //vTaskDelete( NULL );
}
void taskRFID( void * parameter ){
  for(;;){
    TIMERG0.wdt_wprotect=TIMG_WDT_WKEY_VALUE;
    TIMERG0.wdt_feed=1;
    TIMERG0.wdt_wprotect=0;
    //Check for an RFID Card
    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()){
      for (byte i = 0; i < 4; i++) {
        uid[i] = rfid.uid.uidByte[i];
      }
      //Get RFID card
      for (byte j = 0; j < rfid.uid.size; j++) 
      {
        RFIDCode.concat(String(rfid.uid.uidByte[j] < 0x10 ? "0" : ""));
        RFIDCode.concat(String(rfid.uid.uidByte[j], HEX));
      }
      //Print RFID Tag
      RFIDCode.toUpperCase();
      RFIDCode=String(RFIDCode);
      Serial.print("RFID Code: "); Serial.println(RFIDCode);
      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
      //Get login time
      rfid.PCD_AntennaOff();  //Turn Antenna off
      Serial.println("Antenna Off "); //Successfully logged in
      RGB_color(0, 1, 1); //RED - occupied
      xTaskCreate(
        taskReadSensors,          /* Task function. */
        "taskReadSensors",        /* String with name of task. */
        10000,            /* Stack size in bytes. */
        NULL,             /* Parameter passed as input of the task */
        1,                /* Priority of the task. */
        &taskReadSensors_handle);            /* Task handle. */
      xTaskCreate(
        taskLogoutButton,          /* Task function. */
        "taskLogoutButton",        /* String with name of task. */
        10000,            /* Stack size in bytes. */
        NULL,             /* Parameter passed as input of the task */
        1,                /* Priority of the task. */
        &taskLogoutButton_handle);            /* Task handle. */
      Serial.println("Suspending RFID Task");
      vTaskSuspend( NULL );
    }
    else{
      Serial.println("Please Scan");
    }
  }
}
/* ________________________________________Setup________________________________________ */
void setup() {
  Serial.begin(112500);
  //RFID
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  //RGB LED
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(buttonPin, INPUT);  
  digitalWrite(RED,HIGH); //Note: Common Anode LEDs work on active LOW signals
  digitalWrite(GREEN,HIGH);
  digitalWrite(BLUE,HIGH);
  RGB_color(1, 0, 1); //Green - unoccupied at the start
  delay(1000);
  xTaskCreate(
    taskRFID,          /* Task function. */
    "taskRFID",        /* String with name of task. */
    10000,            /* Stack size in bytes. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    &taskRFID_handle);            /* Task handle. */
}
/* ________________________________________Loop_________________________________________ */
void loop() {
  delay(1000);
}