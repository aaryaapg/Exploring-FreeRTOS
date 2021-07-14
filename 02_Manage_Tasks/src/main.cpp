//Refer: https://savjee.be/videos/programming-esp32-with-arduino/manage-freertos-tasks/
/* ______________________________________Libraries______________________________________ */
#include <Arduino.h>
/* _______________________________________Macros________________________________________ */
/* _____________________________Declarations_and_Variables______________________________ */
int count1 = 0;
int count2 = 0;
//#2: Step1 - Create Task handle to terminate task1 from the outside
TaskHandle_t task1_handle = NULL;
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
      /*
      //#1: Delete this task fron inside the task
      if(count1>3){
        Serial.println("Deleting Task1");
        vTaskDelete(NULL);
      }
      */ 
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
    //#2: Step2 - add task handle pointer here
    &task1_handle      // Task handle - this is a pointer
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
  /*
  //#2: Suspend and resume task1 from loop
  if(count1==3 && task1_handle!=NULL){ //NOTE1
    //Serial.println("Suspending Task1");
    vTaskSuspend(task1_handle);
  }
  if(count2==30 && task1_handle!=NULL){
    //Serial.println("Resuming Task1");
    vTaskResume(task1_handle);
  }
  */
  
  /*NOTE1
  * vTaskSuspend(NULL) suspends the current task. 
  * Since task1_handle has been initialized to NULL, it will 
  * suspend the loop function. Therefore, we have added this 
  * condition in the if statement.
  */
}