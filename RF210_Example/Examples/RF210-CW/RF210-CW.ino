/*
   RF210 test LoRa continuous wave
*/
#define LED 4
#define EN_RAK 10

#define F_CENTER  900000000 // Set frequency
#define F_SPAN  30000000 // Set frequency span
#define F_STEP  1000000 // Set Step Frequency
#define TX_Power 14  // 14-22 dBm
#define Time_Step 2 // Second

HardwareSerial mySerial1(1);

int rxPin = 20;
int txPin = 21;

int Frequency = F_CENTER - F_SPAN/2;

void setup()
{
  Serial.begin(115200);

  pinMode(txPin, OUTPUT);
  pinMode(rxPin, INPUT);
  pinMode(EN_RAK, OUTPUT);
  pinMode(LED, OUTPUT);

  digitalWrite(4, HIGH);   
  delay(1000);                      
  digitalWrite(4, LOW);    
  delay(1000);

  digitalWrite(10, HIGH);
  delay(1000);
  mySerial1.begin(115200, SERIAL_8N1, rxPin, txPin);

  Serial.println("==============================");
  Serial.println("RF210 - CW");
  Serial.println("==============================");
  Serial.print("Frequency: ");
  Serial.print(F_CENTER - F_SPAN/2);
  Serial.print("->");
  Serial.println(F_CENTER + F_SPAN/2);
  Serial.print("Step: ");
  Serial.println(F_STEP);

}

void loop()
{

  digitalWrite(4, HIGH);
  delay(1000);
  
  if (Frequency <= F_CENTER + F_SPAN/2  )
  {
    Frequency += F_STEP;
  }
  else 
  {
    Frequency = F_CENTER - F_SPAN/2;
  }
  
  mySerial1.print("AT+CW=");
  mySerial1.print(Frequency);
  mySerial1.print(":");
  mySerial1.print(TX_Power);
  mySerial1.print(":");
  mySerial1.println(Time_Step);
  mySerial1.print("AT+CW=?");
  delay(Time_Step*1000);

}
