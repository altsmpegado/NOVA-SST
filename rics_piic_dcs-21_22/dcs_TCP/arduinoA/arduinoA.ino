/*code for mduino 58 plus */

/**********************LIBS*****************************/
#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include <semphr.h>
#include <SPI.h>
#include <Ethernet.h>
#include "kitA.h"

/********************TASK HANDLERS**********************/
TaskHandle_t taskSystemHandle;
TaskHandle_t taskExecutionHandle;
TaskHandle_t taskStopHandle;
TaskHandle_t taskResumeHandle;

int movestate[4];
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xFE };

IPAddress ip(192, 168, 200, 4);
EthernetClient client;
EthernetServer server = EthernetServer(8888);

SemaphoreHandle_t semExe;
SemaphoreHandle_t semStop;
SemaphoreHandle_t semResume;
QueueHandle_t mbxExe;
QueueHandle_t mbxStop;
QueueHandle_t mbxResume;

void taskSystem(void *pvParameters);
void taskSimpleRun(void *pvParameters);
void taskStop(void *pvParameters);
void taskResume(void *pvParameters);

void setup() {

  pinMode(Q1_0,OUTPUT);
  pinMode(Q1_1,OUTPUT);
  pinMode(Q1_2,OUTPUT);
  pinMode(Q1_3,OUTPUT);
  
  mbxExe = xQueueCreate(1, sizeof(int));
  mbxStop = xQueueCreate(1, sizeof(int));
  mbxResume = xQueueCreate(1, sizeof(int));
  semExe = xSemaphoreCreateMutex();
  xSemaphoreGive(semExe);
  semStop = xSemaphoreCreateMutex();
  xSemaphoreGive(semStop);
  semResume = xSemaphoreCreateMutex();
  xSemaphoreTake(semResume, portMAX_DELAY);

  xTaskCreate(taskSystem, "taskSystem", 128, NULL, 1 /*Priority*/, &taskSystemHandle);
  xTaskCreate(taskExecution, "taskExecution", 128, NULL, 1 /*Priority*/, &taskExecutionHandle);
  xTaskCreate(taskStop, "taskStop", 128, NULL, 2 /*Priority*/, &taskStopHandle);
  xTaskCreate(taskResume, "taskResume", 128, NULL, 2 /*Priority*/, &taskResumeHandle);
  
  calibration();

  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  server.begin();
}

void loop() {}

void taskSystem(void *pvParameters){
  (void) pvParameters;
  
  while(true){
    
    EthernetClient client = server.available();
    
    if (client) {
      while(client.available()){
        char cmd = client.read();
        Serial.print(cmd);
        //client.println("ACK_A");
        switch(cmd){
          case '1':
            xSemaphoreTake(semExe, portMAX_DELAY);
            xQueueSend(mbxExe,&cmd,portMAX_DELAY);
            break;
          case '2':
            conveyorStop();
            break;
          case '3':
            xSemaphoreTake(semStop, portMAX_DELAY);
            xQueueSend(mbxStop,&cmd,portMAX_DELAY);
            break;
          case '4':
            xSemaphoreTake(semResume, portMAX_DELAY);
            xQueueSend(mbxResume,&cmd,portMAX_DELAY);
            break; 
          case '5':
            transition_send();
            break;
          case '6':
            transition_rcv();
            break;
          case '7':
            if(isPartAtSensor())
              server.println("A_ACK_ON");
            else
              server.println("A_ACK_OFF");
            break;
        }
      }
    }
  }
}

void taskExecution(void *pvParameters){
  (void) pvParameters;
  char cmd;
  while(true){
    if(xQueueReceive(mbxExe, &cmd, portMAX_DELAY)){
        execution();
    }
  }
}

void taskStop(void *pvParameters){
  (void) pvParameters;
  char cmd; 
  while(true){
    if(xQueueReceive(mbxStop, &cmd, portMAX_DELAY)){
      taskENTER_CRITICAL();
      movestate[0]=digitalRead(Q1_0);
      movestate[1]=digitalRead(Q1_1);
      movestate[2]=digitalRead(Q1_2);
      movestate[3]=digitalRead(Q1_3);
      taskEXIT_CRITICAL();
       
      actuatorStop();
      conveyorStop();
      vTaskSuspend(taskExecutionHandle);
      xSemaphoreGive(semResume);
    }
  }
}

void taskResume(void *pvParameters){
  (void) pvParameters;
  
  char cmd;
  while(true){
    if(xQueueReceive(mbxResume, &cmd, portMAX_DELAY)){
      taskENTER_CRITICAL();
      digitalWrite(Q1_0, movestate[0]);
      digitalWrite(Q1_1, movestate[1]);
      digitalWrite(Q1_2, movestate[2]);
      digitalWrite(Q1_3, movestate[3]);
      taskEXIT_CRITICAL();
      vTaskResume(taskExecutionHandle);
      xSemaphoreGive(semStop);
    }
  }
}
