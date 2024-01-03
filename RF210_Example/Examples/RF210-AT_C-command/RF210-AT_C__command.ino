// ESP32 C3 SERIAL1 (second UART)
#define LED 4
#define EN_RAK 10

HardwareSerial mySerial1(1);

int rxPin = 20;
int txPin = 21;


void setup()
{
  Serial.begin(115200);  
  
  Serial.println("----------------------------------");
  Serial.println("RF210 UART - AT/ATC COMMAND");
  Serial.println("----------------------------------");
  
  pinMode(txPin, OUTPUT);
  pinMode(rxPin, INPUT);
  pinMode(EN_RAK, OUTPUT);
  pinMode(LED, OUTPUT); 
  pinMode(9, INPUT_PULLUP);
  
  //Blink led
  digitalWrite(LED, HIGH);   
  delay(1000);                       
  digitalWrite(LED, LOW);   
  delay(1000);

  //Enable module RAK3172
  digitalWrite(10, HIGH); 
  delay(1000);
  mySerial1.begin(115200, SERIAL_8N1, rxPin, txPin);
  delay(1000);
  
  mySerial1.println("ATE");
  delay(100);

  while (mySerial1.available()) {
    Serial.write(mySerial1.read()); // read it and send it out Serial (USB)
  }
}

void loop()
{

  if (Serial.available()) {      // If anything comes in Serial (USB),
    mySerial1.write(Serial.read());   // read it and send it out mySerial1 (pins 0 & 1)
  }

  if (mySerial1.available()) {     // If anything comes in Serial1 (pins 0 & 1)
    Serial.write(mySerial1.read());   // read it and send it out Serial (USB)
  }
  
}
