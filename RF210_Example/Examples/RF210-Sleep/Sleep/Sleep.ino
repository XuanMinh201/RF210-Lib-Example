/* 
 *  Example sleep for RF210 
 *  Estimate: ~20uA
 */

#define INTERVAL 10 // Time: second
#define LED 4
#define EN_RAK 10

HardwareSerial mySerial1(1);

int rxPin = 20;
int txPin = 21;

void setup() {
  
  mySerial1.begin(115200, SERIAL_8N1, rxPin, txPin);
  delay(1000);
  //Enable LDO Rak
  pinMode (EN_RAK, OUTPUT);
  digitalWrite(10, HIGH);
  
  //Disnable GPS
  pinMode (1, OUTPUT);
  digitalWrite(1, LOW);

  
}

void loop() {
  // Sleep module RAK312
  mySerial1.print("AT+SLEEP=");
  mySerial1.println(INTERVAL*1000);
  
  // Sleep ESP32-C3
  esp_sleep_enable_timer_wakeup(INTERVAL*1000000); // 10 sec
  
  gpio_hold_en((gpio_num_t)10);
  gpio_hold_en((gpio_num_t)1);
  gpio_deep_sleep_hold_en();

  esp_deep_sleep_start();
  // we never reach here
}
