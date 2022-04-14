#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif
#include <semphr.h>
QueueHandle_t queuePotVal;
QueueHandle_t queueFilteredVal;
#define signalA 15
#define signalB 21
#define input_button 23
#define inputVtask3 5
#define input_V 2
struct output_csv {
  int input_button_state;
  int filtered_val;
  int error_code;
};
static SemaphoreHandle_t mutex;


void setup() {
  // put your setup code here, to run once:
  pinMode(signalB,OUTPUT);
  pinMode(signalA, OUTPUT);
  pinMode(input_button,INPUT);
  pinMode(inputVtask3,INPUT);
  
  Serial.begin(9600);

  queuePotVal=xQueueCreate(1,sizeof(float));
  queueFilteredVal=xQueueCreate(1,sizeof(int));
  
  xTaskCreate(task1,"task1",1024,NULL,1,NULL);
  xTaskCreate(task2,"task2",1024,NULL,1,NULL);
  xTaskCreate(task3,"task3",1024,NULL,1,NULL);
  xTaskCreate(task4,"task4",1024,NULL,1,NULL);
  xTaskCreate(task5,"task5",1024,NULL,1,NULL);
  xTaskCreate(task6,"task6",1024,NULL,1,NULL);
  xTaskCreate(task7,"task7",1024,NULL,1,NULL);
  xTaskCreate(task8,"task8",1024,NULL,1,NULL);
  xTaskCreate(task9,"task9",1024,NULL,1,NULL);
  
}
void loop() {
  // put your main code here, to run repeatedly:
}
void task1(void *pvParameters ){
  TickType_t xLastWakeTime;
  const TickType_t xFrequency = 16.35;
  // Initialise the xLastWakeTime variable with the current time.
  xLastWakeTime = xTaskGetTickCount();
  for(;;){
    digitalWrite(signalB, HIGH);
    vTaskDelay(10);// The signal is high for 50 micro seconds, the same as in assignment 1
    digitalWrite(signalB,LOW);
    //Serial.println("hello");
    vTaskDelayUntil(&xLastWakeTime,xFrequency);
  }
 
  
}
void task2(void *pvParameters ){
  int input_button_state=0;  
  TickType_t xLastWakeTime;
    const TickType_t xFrequency = 200;
    // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount();
    for(;;){
      input_button_state= digitalRead(input_button);
      //Serial.println(input_button_state);
      vTaskDelayUntil(&xLastWakeTime,xFrequency);
    }
}
void task3(void *pvParameters ){
TickType_t xLastWakeTime;
  const TickType_t xFrequency = 1000;
  // Initialise the xLastWakeTime variable with the current time.
  xLastWakeTime = xTaskGetTickCount();
  float duration;
  float freq;
  for(;;){
     duration = pulseIn(inputVtask3,LOW);
    // Serial.println("d: ");
     //Serial.println(duration);
     
     freq = 1000000/(duration*2);//
    // Serial.println("f: ");
     //Serial.println(freq);
     vTaskDelayUntil(&xLastWakeTime,xFrequency);
  }
}
void task4(void *pvParameters){
  float pot_val;
  TickType_t xLastWakeTime;
    const TickType_t xFrequency = 42;
    //Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount();

    for(;;){
      pot_val = analogRead(input_V);      
      xQueueSend(queuePotVal,&pot_val,100);
      
      //pot_val = analogRead(input_V);

    
      vTaskDelayUntil(&xLastWakeTime,xFrequency); 
    }
}
void task5(void *pvParameters){
  float potValArray[4]={0, 0, 0, 0};
  float total=0;
  int filtered_val=0;
  float pot_val;
  TickType_t xLastWakeTime;
    const TickType_t xFrequency = 42;
    //Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount();
    for(;;){
      total=0;
      if(queuePotVal!=NULL){
        if (xQueueReceive(queuePotVal,&pot_val,(TickType_t)10) ==pdPASS){
        
        for (int i=0;i<4;i++){
          potValArray[i]=potValArray[i+1];
          if (i=3){
            potValArray[3]=potValArray[0];
            potValArray[0]=pot_val;
          }
          else{
            potValArray[i]=potValArray[i+1];
          }     
        }
        for(int j=0;j<4;j++){
          total+=potValArray[j];
        }
        filtered_val=total/4;
        Serial.println(filtered_val);
        xQueueSend(queueFilteredVal,&filtered_val,100);        
      } 
    }
    vTaskDelayUntil(&xLastWakeTime,xFrequency);
  }        
}
void task6(void *pvParameters){
 TickType_t xLastWakeTime;
    const TickType_t xFrequency = 100;
    //Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount();
  
    for(;;){
      for (int j=0;j<1000;j++){
        __asm__ __volatile__ ("nop");
      }
      vTaskDelayUntil(&xLastWakeTime,xFrequency);
    }  
}
void task7(void *pvParameters){
 TickType_t xLastWakeTime;
    const TickType_t xFrequency = 300;
    //Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount();

    for(;;){
      if(queueFilteredVal!=NULL){
        if (xQueueReceive(queueFilteredVal,&filtered_val,(TickType_t)10) ==pdPASS){
          if(xSemaphoreTake(mutex,portMAX_DELAY)==pdTRUE){
            fil_val=filtered_val     
            if (fil_val >(4095/2)){
              error_code = 1;
            }
            else if (fil_val<(4095/2)){
              error_code = 0;
            }    
            xSemaphoreGive(mutex);
            
          } 
        }
      }  
     vTaskDelayUntil(&xLastWakeTime,xFrequency);  
    }
}
void task8(void *pvParameters){
TickType_t xLastWakeTime;
    const TickType_t xFrequency = 300;
    //Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount();

    for(;;){
    
      if (error_code==1){
        digitalWrite(signalA, HIGH);// The yellow LED on my circuit will turn on when the error code is high.
      }
      else{
       digitalWrite(signalA, LOW);
      }
      vTaskDelayUntil(&xLastWakeTime,xFrequency);   
    }
}
 
//void task9(void *pvParameters){
 // Serial.print(input_button_state);
//}
