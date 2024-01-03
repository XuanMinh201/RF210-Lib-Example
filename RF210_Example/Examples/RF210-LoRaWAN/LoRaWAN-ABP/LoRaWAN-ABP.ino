/*
    This example shows LoRaWan protocol joining the network in ABP mode, class A, region AS923.
    Device will send uplink every 30 seconds.

*/

HardwareSerial mySerial1(1);

#define LED 4
#define EN_RAK 10
#define INTERVAL 10 // Period to send LoRaWAN packet


int rxPin = 20;
int txPin = 21;

/*-- Begin  --*/

String DEVADDR = "260B6176";
String NWKSKEY = "E4D504E6584676F60121466D581D0D71";
String APPSKEY = "F6EDCE951D507238FA18F564C8E8FC4D";

//(0 = EU433, 1 = CN470, 2 = RU864, 3 = IN865, 4 = EU868, 5 = US915, 6 = AU915, 7 = KR920, 8 = AS923-1, 9 = AS923-2, 10 = AS923-3, 11 = AS923-4, 12 = LA915)
int BAND = 9;

/*-- END  --*/

int rx_delay = 0;
int fport = 1;

void setup()
{
  Serial.begin(115200);

  pinMode(txPin, OUTPUT);
  pinMode(rxPin, INPUT);
  pinMode(EN_RAK, OUTPUT);
  pinMode(LED, OUTPUT);

  L_blink();

  digitalWrite(10, HIGH);
  delay(1000);

  mySerial1.begin(115200, SERIAL_8N1, rxPin, txPin);
  //  mySerial1.println("AT+BAUD=921600"); // Set Baud rate
  //  delay(100);
  //  mySerial1.begin(921600, SERIAL_8N1, rxPin, txPin);

  Serial.println("==============================");
  Serial.println("RF210 - LoRaWAN ABP");
  Serial.println("==============================");


  mySerial1.println("AT+NJM=0"); // Set ABP
  delay(100);
  mySerial1.println("AT+NWM=1"); // Set LoRaWan mode
  delay(100);
  mySerial1.println("AT+CLASS=A"); // Set CLASS
  delay(100);
  mySerial1.print("AT+BAND=");// Set frequency band
  mySerial1.println(BAND);
  delay(100);
  mySerial1.print("AT+DEVADDR=");
  mySerial1.println(DEVADDR);
  delay(100);
  mySerial1.print("AT+NWKSKEY=");
  mySerial1.println(NWKSKEY);
  delay(100);
  mySerial1.print("AT+APPSKEY=");
  mySerial1.println(APPSKEY);
  delay(100);
  // mySerial1.print("AT+JOIN=1:0:10:3");


  while (mySerial1.available() == 0)
  {
    Serial.println("Waiting");
    delay(500);
  }

  flush_serial_AT();
  delay(500);

}

void loop()
{


  int temp = (int) 100 * measure_temp();
  uint8_t hum = (uint8_t) measure_hum();
  int ax = 1000 * measure_acc(1);
  int ay = 1000 * measure_acc(2);
  int az = 1000 * measure_acc(3);
  int lum = measure_lum();
  int inf = measure_inf();
  
  int i = 0;
  unsigned char mydata[64];
  
  mydata[i++] = (uint8_t)(temp >> 8) & 0xFF;
  mydata[i++] = (uint8_t)temp & 0xFF;
  mydata[i++] = (uint8_t)hum & 0xFF;
  mydata[i++] = (uint8_t)(ax >> 8) & 0xFF;
  mydata[i++] = (uint8_t)ax & 0xFF ;
  mydata[i++] = (uint8_t)(ay >> 8) & 0xFF;
  mydata[i++] = (uint8_t)ay & 0xFF;
  mydata[i++] = (uint8_t)(az >> 8) & 0xFF;
  mydata[i++] = (uint8_t)az & 0xFF;
  mydata[i++] = (uint8_t)(lum >> 8) & 0xFF;
  mydata[i++] = (uint8_t)lum & 0xFF;
  mydata[i++] = (uint8_t)(inf >> 8) & 0xFF;
  mydata[i++] = (uint8_t)inf & 0xFF;
  char str[32] = "";
  array_to_string(mydata, i, str);

  flush_serial_AT();

  L_blink();
  
  mySerial1.print("AT+SEND=");
  mySerial1.print(fport);
  mySerial1.print(":");
  mySerial1.println(str);

  mySerial1.readStringUntil('\n');

  while (mySerial1.available() == 0)
  {
    rx_delay = rx_delay + 100;
    delay(100);
  }

  if (mySerial1.available())
  { // If anything comes in Serial1 (pins 4 & 5)
    while (mySerial1.available())
      Serial.write(mySerial1.read()); // read it and send it out Serial (USB)
  }
  Serial.println("AT set complete with downlink");
  int wait_time = INTERVAL * 1000 - rx_delay;
  rx_delay = 0;
  delay(wait_time);

  if (fport < 255)
    fport += 1;
  else
    fport = 1;
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
// SHTC3 available
int check_sht() {
  flush_serial_AT();
  mySerial1.println("ATC+SHT=?");
  String sht;
  delay(100);
  if (mySerial1.available()) {
    sht = mySerial1.readStringUntil('\n');

  }
  return sht.toInt();
}
// KX023 available
int check_kx023() {
  flush_serial_AT();
  mySerial1.println("ATC+KX023=?");
  String kx023;
  delay(100);
  if (mySerial1.available()) {
    kx023 = mySerial1.readStringUntil('\n');

  }
  return kx023.toInt();
}
// LTR available
int check_ltr() {
  flush_serial_AT();
  mySerial1.println("ATC+LTR=?");
  String ltr;
  delay(100);
  if (mySerial1.available()) {
    ltr = mySerial1.readStringUntil('\n');

  }
  return ltr.toInt();
}
// Return temperature value
float measure_temp() {
  flush_serial_AT();
  mySerial1.println("ATC+TEMP=?");
  String temperature;
  delay(100);
  if (mySerial1.available()) {
    temperature = mySerial1.readStringUntil('\n');
    Serial.print("Temperature:");
    Serial.println(temperature);
  }
  return temperature.toFloat();
}

// Return humidity value
float measure_hum() {
  flush_serial_AT();
  mySerial1.println("ATC+HUM=?");
  String hum;
  delay(100);
  if (mySerial1.available()) {
    hum = mySerial1.readStringUntil('\n');
    Serial.print("Humidity:");
    Serial.println(hum);
  }
  return hum.toFloat();
}

// Return Infrared value
float measure_inf() {
  flush_serial_AT();
  mySerial1.println("ATC+LUMCH1=?");
  String inf;
  delay(100);
  if (mySerial1.available()) {
    inf = mySerial1.readStringUntil('\n');
    Serial.print("Infrared:");
    Serial.println(inf);
  }
  return inf.toFloat();
}
// Return Luminosity value
float measure_lum() {
  flush_serial_AT();
  mySerial1.println("ATC+LUM=?");
  String lum;
  delay(100);
  if (mySerial1.available()) {
    lum = mySerial1.readStringUntil('\n');
    Serial.print("Luminosity:");
    Serial.println(lum);
  }
  return lum.toFloat();
}

// Return Acceleration level in G
float measure_acc(int axis) {
  flush_serial_AT();
  if (axis == 1) {
    mySerial1.println("ATC+AX=?");
  }
  else if (axis == 2) {
    mySerial1.println("ATC+AY=?");
  }
  else if (axis == 3) {
    mySerial1.println("ATC+AZ=?");
  }
  String a;
  delay(100);
  if (mySerial1.available()) {
    a = mySerial1.readStringUntil('\n');
    //Serial.print("Acc:");
    Serial.println(a);
  }
  return a.toFloat();
}
// Uart sent
void flush_serial_AT() {
  if (mySerial1.available())
  {
    while (mySerial1.available())
      mySerial1.read();
  }
  delay(100);
}
// Toggle LED
void L_blink() {
  digitalWrite(4, HIGH);
  delay(500);
  digitalWrite(4, LOW);
}
