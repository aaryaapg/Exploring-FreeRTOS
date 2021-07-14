/*
* This code uses 2 different tasks to blink LEDs at different speeds
* For LEDs: https://circuitdigest.com/microcontroller-projects/arduino-freertos-tutorial1-creating-freertos-task-to-blink-led-in-arduino-uno
*/

/* ______________________________________Libraries______________________________________ */
#include <Arduino.h>
/* _______________________________________Macros________________________________________ */
/* _____________________________Declarations_and_Variables______________________________ */
int count1 = 0;
int count2 = 0;
/* ______________________________________Functions______________________________________ */
void task1(void * parameters){
  pinMode(18, OUTPUT);
    while(1){ // infinite loop
        Serial.print("Task 1 counter: ");
        Serial.println(count1++);
        digitalWrite(18,HIGH);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        digitalWrite(18,LOW);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}
void task2(void * parameters){
  pinMode(19, OUTPUT);
    while(1){ // infinite loop
        Serial.print("Task 2 counter: ");
        Serial.println(count2++);
        digitalWrite(19,HIGH);
        vTaskDelay(1000 / portTICK_PERIOD_MS);  
        digitalWrite(19,LOW);
        vTaskDelay(1000 / portTICK_PERIOD_MS);  
    }
}
/* ________________________________________Setup________________________________________ */
void setup() {
  Serial.begin(115200);
  
  
  xTaskCreate(
        task1,    // Function that should be called
        "Task 1", // Name of the task (for debugging)
        1000,     // Stack size (bytes)
        NULL,     // Parameter to pass
        1,        // Task priority
        NULL      // Task handle
  );

	xTaskCreate(
        task2,    // Function that should be called
        "Task 2", // Name of the task (for debugging)
        1000,     // Stack size (bytes)
        NULL,     // Parameter to pass
        1,        // Task priority
        NULL      // Task handle
  );
}
/* ________________________________________Loop_________________________________________ */
void loop() {
  // put your main code here, to run repeatedly:
  //no code here
}