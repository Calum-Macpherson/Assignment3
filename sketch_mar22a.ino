#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif
const int signalA=15;
const int signalB=21;
const int input_button=23;
const int input_button_state=0;
const int inputVtask3


void setup() {
  // put your setup code here, to run once:
  pinMode(signalB,OUTPUT);
  pinMode(signalA, OUTPUT);
  pinMode(input_button,INPUT);
  Serial.begin(9600);
  
  xTaskCreate(task1,"task1",1024,NULL,1, NULL);
  xTaskCreate(task2,"task2",1024,NULL,1, NULL);
  xTaskCreate(task3,"task3£,1024,NULL,1,NULL);

}

void loop() {
  // put your main code here, to run repeatedly:

}

void vTask1(void *pvParameters ){
  TickType_t xLastWakeTime;
  const TickType_t xFrequency = 5000;

  // Initialise the xLastWakeTime variable with the current time.
  xLastWakeTime = xTaskGetTickCount();

  for(;;){
    digitalWrite(signalB, HIGH);
    vTaskDelay(1000);// The signal is high for 50 micro seconds, the same as in assignment 1
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
  const TickType_t xFrequency = 200;

  // Initialise the xLastWakeTime variable with the current time.
  xLastWakeTime = xTaskGetTickCount();
  for(;;){
     duration=pulseIn(inputVtask3,HIGH);
     freq=1/(duration*2*pow(10, -6));//
    
  }
}


//void task9(void *pvParameters){
 // Serial.print(input_button_state);
//}
