/* 
 *  Example Deep sleep for RF210
 *  Estimate: ~12uA
 */ 

#define INTERVAL 10 //second

void setup() {
  
  //Disnable LDO Rak
  pinMode (10, OUTPUT);
  digitalWrite(10, LOW);
  
  //Disnable LDO GPS
  pinMode (1, OUTPUT);
  digitalWrite(1, LOW);
  
}

void loop() {
  
  esp_sleep_enable_timer_wakeup(INTERVAL*1000000); // 10 sec
  
  gpio_hold_en((gpio_num_t)10);
  gpio_hold_en((gpio_num_t)1);
  gpio_deep_sleep_hold_en();

  esp_deep_sleep_start();
  // we never reach here
}
