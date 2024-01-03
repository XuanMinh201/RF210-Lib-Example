/*
   RF210 get data from to sensor
   KX023  - Acceleration
   SHTC3  - Temperature/Humidity
   LTR303 - Infrared/Visible
*/
#define LED 4
#define EN_RAK 10

HardwareSerial mySerial1(1);

int rxPin = 20;
int txPin = 21;

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

  Serial.println("==============================");
  Serial.println("RF210 - Get Data SenSor");
  Serial.println("==============================");

  flush_serial_AT();
  delay(500);

}

void loop()
{
  L_blink();
  
  if (!check_sht) {
    Serial.println("Couldn't find SHTC3");
  }
  else {
    Serial.println("\n---------- SHTC3 ----------\n");
    measure_hum();
    measure_temp();
  }
  
  if (!check_kx023) {
    Serial.println("Couldn't find KX023");
  }
  else{
  Serial.println("\n---------- KX023 ----------\n");
  Serial.print ("Axis X: ");
  measure_acc(1);
  Serial.print ("Axis Y: ");
  measure_acc(2);
  Serial.print ("Axis Z: ");
  measure_acc(3);
  }
  
  if (!check_ltr) {
    Serial.println("Couldn't find LTR303");
  }
  else{
  Serial.println("\n---------- LTR303 ----------\n");
  measure_lum();
  measure_inf();
  }
  
  delay (5000);

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
