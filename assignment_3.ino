#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif
const int signalA=15;
const int input_button=23;
const int input_button_state=0;
void task1();
void task2();
void task3();
void task4();
void task5();
void task6();
void task7();
void task8();
void task9();
void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);
  

  xTaskCreatePinnedToCore(task2,"task2",1024,(void*)1,1,queue);

}

void loop() {
  // put your main code here, to run repeatedly:

}

void task1(void *pvParameters ){
  
}
void task2(void *pvParameters ){
  pinMode(signalA, OUTPUT);
  for(;;){
    input_button_state= digitalRead(input_button);
  }
}

void task9(void *pvParameters){
  Serial.print(input_button_state);
}
