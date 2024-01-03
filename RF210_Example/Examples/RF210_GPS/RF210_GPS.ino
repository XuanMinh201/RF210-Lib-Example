/*
    This example shows LoRaWan protocol joining the network in ABP mode, class A, region AS923.
    Device will send uplink every 30 seconds.

*/

HardwareSerial mySerial1(1);

#define SLEEP    1      // Define if LoRa packet are sent to network

int rxPin = 20;
int txPin = 21;

#define LED 4
#define EN_RAK 10
#define INTERVAL 10 // Period to send LoRaWAN packet
int8_t DC=0;
/*-- Begin  --*/

String DEVADDR = "260B6176";
String NWKSKEY = "E4D504E6584676F60121466D581D0D71";
String APPSKEY = "F6EDCE951D507238FA18F564C8E8FC4D";

//(0 = EU433, 1 = CN470, 2 = RU864, 3 = IN865, 4 = EU868, 5 = US915, 6 = AU915, 7 = KR920, 8 = AS923-1, 9 = AS923-2, 10 = AS923-3, 11 = AS923-4, 12 = LA915)
int BAND = 9;

/*-- END  --*/

int rx_delay = 0;

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("ABP tracker");

  pinMode(txPin, OUTPUT);
  pinMode(rxPin, INPUT);

  pinMode(10, OUTPUT); //Rak enable
  pinMode(4, OUTPUT); // LED


  digitalWrite(4, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(4, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);

  digitalWrite(10, HIGH); // Switch on RAK
  delay(1000);
  mySerial1.begin(115200, SERIAL_8N1, rxPin, txPin);



  Serial.println("Setup at command");
  //mySerial1.println("ATR"); // Set ABP
  mySerial1.println("AT+NJM=0"); // Set ABP
  // delay(300);
  mySerial1.println("AT+BAND=9");// Set EU868 frequency band
  //  delay(300);
  mySerial1.println("AT+DR=5");// Set EU868 frequency band
  // delay(300);

  mySerial1.print("AT+DEVADDR=");
  mySerial1.println(DEVADDR);
  //delay(300);
  mySerial1.print("AT+NWKSKEY=");
  mySerial1.println(NWKSKEY);
  // delay(300);
  mySerial1.print("AT+APPSKEY=");
  mySerial1.println(APPSKEY);
  // delay(300);
  while (mySerial1.available() == 0)
  {
    Serial.println("Waiting");
    delay(200);
  }

  flush_serial_AT(false);
  mySerial1.println("ATC+GPSON=1"); // Activate GNSS
  delay(500);
  mySerial1.println("ATC+GPSON=1"); // Activate GNSS
  delay(500);
  flush_serial_AT(false);
}

void loop()
{
  int16_t t = (int16_t) 10 * measure_temp(); // return temperature in tens of degree
  uint8_t h = (uint8_t)2 * measure_hum(); // return humidity in percent
  int16_t x = 1000 * measure_acc(1);
  int16_t y = 1000 * measure_acc(2);
  int16_t z = 1000 * measure_acc(3);
  int16_t l = measure_lum();
  int16_t b = measure_bat() / 10;
  int32_t LatitudeBinary = 10000 * measure_gnss(1); //Latitude : 0.0001 ° Signed MSB
  int32_t LongitudeBinary = 10000 * measure_gnss(2); //Longitude : 0.0001 ° Signed MSB
  int32_t AltitudeBinary = 100 * measure_gnss(3); // Altitude : 0.01 meter Signed MSB
  uint16_t s = 100 * measure_gnss(4); // nb of satellite in view with GNSS



  //int blocks=7;
  int i = 0;
  unsigned char mydata[64];

  mydata[i++] = t >> 8;
  mydata[i++] = t & 0xFF;
  mydata[i++] = h;
  mydata[i++] = x >> 8;
  mydata[i++] = x & 0xFF;
  mydata[i++] = y >> 8;
  mydata[i++] = y & 0xFF;
  mydata[i++] = z >> 8;
  mydata[i++] = z & 0xFF;
  mydata[i++] = l >> 8;
  mydata[i++] = l & 0xFF;
  mydata[i++] = b >> 8;
  mydata[i++] = b & 0xFF;
  mydata[i++] = s >> 8;
  mydata[i++] = s & 0xFF;
  mydata[i++] = DC;
  mydata[i++] = ( LatitudeBinary >> 16 ) & 0xFF;
  mydata[i++] = ( LatitudeBinary >> 8 ) & 0xFF;
  mydata[i++] = LatitudeBinary & 0xFF;
  mydata[i++] = ( LongitudeBinary >> 16 ) & 0xFF;
  mydata[i++] = ( LongitudeBinary >> 8 ) & 0xFF;
  mydata[i++] = LongitudeBinary & 0xFF;
  mydata[i++] = ( AltitudeBinary >> 16 ) & 0xFF;
  mydata[i++] = ( AltitudeBinary >> 8 ) & 0xFF;
  mydata[i++] = AltitudeBinary & 0xFF;

  char str[i] = "";
  array_to_string(mydata, i, str);

  flush_serial_AT(false);

  blink();
  mySerial1.printf("AT+SEND=3:");
  mySerial1.println(str);

  blink();

  mySerial1.readStringUntil('\n');

  while (mySerial1.available() == 0)
  {
    rx_delay = rx_delay + 100;
    delay(100);
  }
  Serial.print("Rx delay : ");
  Serial.println(rx_delay);

  if (mySerial1.available())
  { // If anything comes in Serial1 (pins 4 & 5)
    while (mySerial1.available())
      Serial.write(mySerial1.read()); // read it and send it out Serial (USB)
  }
  Serial.print("AT set complete with downlink : ");
  Serial.println(str);
  int sleep_time = INTERVAL * 1000 - rx_delay; // Sleep duration in ms
  rx_delay = 0;
  sleep (sleep_time);
}



void array_to_string(byte array[], unsigned int len, char buffer[])
{
  for (unsigned int i = 0; i < len; i++)
  {
    byte nib1 = (array[i] >> 4) & 0x0F;
    byte nib2 = (array[i] >> 0) & 0x0F;
    buffer[i * 2 + 0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
    buffer[i * 2 + 1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
  }
  buffer[len * 2] = '\0';
}

// Return temperature level in degree
float measure_temp() {

  //Serial.flush();
  flush_serial_AT(false);// flush AT Serial reading buffer

  mySerial1.println("ATC+TEMP=?"); // Request bat value
  String temperature;
  while (mySerial1.available() == 0)
  {
    delay(100);
  }


  if (mySerial1.available()) {
    temperature = mySerial1.readStringUntil('\n');
    Serial.print("Temperature:");
    Serial.println(temperature);
  }

  return temperature.toFloat();
}

// Return humidity level in percent
float measure_hum() {

  //Serial.flush();
  flush_serial_AT(false);// flush AT Serial reading buffer

  mySerial1.println("ATC+HUM=?"); // Request bat value
  String hum;
  while (mySerial1.available() == 0)
  {
    delay(300);
  }


  if (mySerial1.available()) {
    hum = mySerial1.readStringUntil('\n');
    Serial.print("Humidity:");
    Serial.println(hum);
  }

  return hum.toFloat();
}

// Return humidity level in percent
float measure_lum() {

  //Serial.flush();
  flush_serial_AT(false);// flush AT Serial reading buffer

  mySerial1.println("ATC+LUM=?"); // Request bat value
  String lum;
  while (mySerial1.available() == 0)
  {
    delay(100);
  }
  delay(100);

  if (mySerial1.available()) {
    lum = mySerial1.readStringUntil('\n');
    Serial.print("Luminosity:");
    Serial.println(lum);
  }

  return lum.toFloat();
}

// Return bat level in mv
float measure_bat() {

  //Serial.flush();
  flush_serial_AT(false);// flush AT Serial reading buffer

  mySerial1.println("ATC+BAT=?"); // Request bat value
  String bat;
  while (mySerial1.available() == 0)
  {
    delay(100);
  }
  delay(100);

  if (mySerial1.available()) {
    bat = mySerial1.readStringUntil('\n');
    Serial.print("Bat:");
    Serial.println(bat);
  }

  return bat.toFloat();
}

// Return Acceleration level in G
float measure_acc(int axis) {

  //Serial.flush();
  flush_serial_AT(false);// flush AT Serial reading buffer

  if (axis == 1) {
    mySerial1.println("ATC+AX=?"); // Request bat value
  }
  else if (axis == 2) {
    mySerial1.println("ATC+AY=?"); // Request bat value
  }
  else if (axis == 3) {
    mySerial1.println("ATC+AZ=?"); // Request bat value
  }
  String a;
  while (mySerial1.available() == 0)
  {
    delay(100);
  }
  delay(100);
  if (mySerial1.available()) {
    a = mySerial1.readStringUntil('\n');
    Serial.print("Acc:");
    Serial.println(a);
  }

  return a.toFloat();
}

void flush_serial_AT(bool print) {

  if (mySerial1.available())
  { // If anything comes in Serial1 (pins 4 & 5)
    while (mySerial1.available())
      if (print) {
        Serial.write(mySerial1.read()); // read it and send it out Serial (USB)
      }
      else {
        mySerial1.read();
      }
  }
  delay(100);
}


void blink() {
  digitalWrite(4, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(70);                       // wait for a second
  digitalWrite(4, LOW);    // turn the LED off by making the voltage LOW
  delay(70);                       // wait for a second
  digitalWrite(4, HIGH);    // turn the LED off by making the voltage LOW
  delay(70);                       // wait for a second
  digitalWrite(4, LOW);    // turn the LED off by making the voltage LOW
}

// Sleep function over a period in ms
void sleep(int period) {

#if SLEEP

  Serial.print("Move to sleep : ");
  Serial.println(period / 1000);

  mySerial1.print("AT+SLEEP="); // Set Sleep for RAK3172
  mySerial1.println(period); // Set Sleep


  //gpio_hold_en((gpio_num_t)10);
  //esp_sleep_enable_ext0_wakeup(WAKEPIN,0); //1 = Low to High, 0 = High to Low. Pin pulled HIGH

  //gpio_light_sleep_hold_en();
  // wake up 1 second later and then go into deep sleep
  esp_sleep_enable_timer_wakeup(period * 1000); // in us
  delay(100);

  esp_light_sleep_start();

#else

  Serial.print("Stop during : ");
  Serial.println(period / 1000);
  // Sleep during period
  delay(period);

#endif

}

// Return 1:lat 2:long 3:alt 4:sat from GNSS
float measure_gnss(int axis) {

  //Serial.flush();
  flush_serial_AT(false);// flush AT Serial reading buffer

  if (axis == 1) {
    mySerial1.println("ATC+GPSLAT=?"); // Request lat value
    Serial.print("Lat:");
  }
  else if (axis == 2) {
    mySerial1.println("ATC+GPSLON=?"); // Request lon value
    Serial.print("Long:");
  }
  else if (axis == 3) {
    mySerial1.println("ATC+GPSALT=?"); // Request alt value
    Serial.print("Altitude:");
  }
  else if (axis == 4) {
    mySerial1.println("ATC+GPSSAT=?"); // Request sat value
    Serial.print("Sat:");
  }
  else if (axis == 5) {
    mySerial1.println("ATC+GPSPWR=?"); // Request sat value
    Serial.print("On:");
  }

  String a;
  while (mySerial1.available() == 0)
  {
    delay(100);
  }


  if (mySerial1.available())
  { // If anything comes in Serial1 (pins 4 & 5)
    a = mySerial1.readStringUntil('\n');
    Serial.println(a);
    delay(100);
    mySerial1.readStringUntil('\n');
  }


  if (a.toFloat() > 5  && DC == 0 && axis == 4)
  {
    mySerial1.println("ATC+GPSDC=1"); // Activate GNSS
    DC = 1;
  }
  if (a.toFloat() < 5  && DC == 1 && axis == 4)
  {
    mySerial1.println("ATC+GPSDC=0"); // Activate GNSS
    DC = 0;
  }


  return a.toFloat();
}
