#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif
QueueHandle_t queuePotVal;
#define signalA 15
#define signalB 21
#define input_button 23
int input_button_state=0;
#define inputVtask3 5
#define input_V 2



void setup() {
  // put your setup code here, to run once:
  pinMode(signalB,OUTPUT);
  pinMode(signalA, OUTPUT);
  pinMode(input_button,INPUT);
  pinMode(inputVtask3,INPUT);
  
  Serial.begin(9600);

  queuePotVal=xQueueCreate( 4,sizeof(float));
  
  xTaskCreate(task1,"task1",1024,NULL,1, NULL);
  xTaskCreate(task2,"task2",1024,NULL,1, NULL);
  xTaskCreate(task3,"task3",1024,NULL,1,NULL);
  xTaskCreate(task4,"task4",1024,NULL,1,NULL);
}
void loop() {
  // put your main code here, to run repeatedly:
}
void task1(void *pvParameters ){
  TickType_t xLastWakeTime;
  const TickType_t xFrequency = 5000;
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
TickType_t xLastWakeTime;
  const TickType_t xFrequency = 200;
  // Initialise the xLastWakeTime variable with the current time.
  xLastWakeTime = xTaskGetTickCount();
  for(;;){
    input_button_state= digitalRead(input_button);
    Serial.println(input_button_state);
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
     Serial.println("d: ");
     Serial.println(duration);
     
     freq = 1000000/(duration*2);//
     Serial.println("f: ");
     Serial.println(freq);
     vTaskDelayUntil(&xLastWakeTime,xFrequency);
  }
}
void task4(void *pvParameters){
  TickType_t xLastWakeTime;
    const TickType_t xFrequency = 42;
    //Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount();

    for(;;)
      if( queuePotVal !=0){
        if( xQueueSend(queuePotVal,(void * )&pot_val,(TickType_t)10))
      }
      pot_val = analogRead(input_V);

    
    vTaskDelayUntil(&xLastWakeTime,xFrequency); 
}
void task5(void *pvParameters){
  TickType_t xLastWakeTime;
    const TickType_t xFrequency = 42;
    //Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount();



  vTaskDelayUntil(&xLastWakeTime,xFrequency); 
  
}
 
//void task9(void *pvParameters){
 // Serial.print(input_button_state);
//}
