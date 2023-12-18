#include <SoftwareSerial.h>
#define rxPin 2
#define txPin 3
#define MQ2pin (0)
#define Flamepin 7
#define buzzer_pin 6
#define pinLedRed 4
#define pinLedChicken 8
#define pinRainSensor 9
#define pinTouchChicken 5
float sensorMQ2Value;
int flameValue;

SoftwareSerial sim900a(rxPin, txPin);
const String PHONE_1 = "+84819412556";
bool stateLedRed = false;
bool stateLockWindowChicken = false;
bool stateLedChicken = false;
bool stateRainSensor = false;
unsigned long startMillis1;
unsigned long startMillis2;
unsigned long startMillis3;
void setup()
{
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  //  Serial.begin(115200);
  Serial.begin(9600);
  pinMode(buzzer_pin, OUTPUT);
  digitalWrite(buzzer_pin, LOW);
  pinMode(pinLedRed, OUTPUT);
  pinMode(pinLedChicken, OUTPUT);
  pinMode(pinRainSensor, INPUT);
  pinMode(pinTouchChicken, INPUT);
  //Begin serial communication with Arduino and SIM800L
  sim900a.begin(115200);
  sim900a.println("AT");
  delay(1000);
  sim900a.println("AT+CMGF=1");
  delay(1000);

}

void loop()
{
  Serial.println("Hello Boss");
  delay(1500);
  sensorMQ2Value = analogRead(MQ2pin); // lay gia tri
  //  Serial.print("Sensor Value: ");// in gia tri ra
  //  Serial.print(sensorMQ2Value);
  //  Serial.println("");
  //  delay(500);
  flameValue = digitalRead(Flamepin); // lay gia tri
  //  Serial.print("Flame Value: ");// in gia tri ra
  //  Serial.print(flameValue);
  //  Serial.println("");
  //  delay(500);
  //  -- - bao chay
  if ((sensorMQ2Value > 500 ) || (sensorMQ2Value > 500 && flameValue == 0) || (flameValue == 0) ) { // so sanh gia tri
//    Serial.println("Co chay");
    digitalWrite(buzzer_pin, HIGH);
    //send_mess("Alo", PHONE_1);
    //  startMillis1 = millis();
    //    while (millis() - startMillis1 < 10000) {
    make_call(PHONE_1);
    //    }
    digitalWrite(pinLedRed, HIGH);
    delay(100);
    digitalWrite(pinLedRed, LOW);

  }

  else {
    digitalWrite(buzzer_pin, LOW);
  }
  //-- ket thuc bao chay
  ledChicken();
  rainSensor();
}

void rainSensor() {
  if (digitalRead(pinRainSensor) == LOW) {
    stateLockWindowChicken = true;
    if (digitalRead(pinRainSensor) == HIGH) {
      stateLockWindowChicken = false;
    }
//  } Serial.print("lock=");
//  Serial.println(stateLockWindowChicken);
}
}
void ledChicken() {
  if (digitalRead(pinTouchChicken) == HIGH) {
    stateLedChicken = !stateLedChicken;
    //    Serial.print("anh iu");
    Serial.println(stateLedChicken);
    //    Serial.println("a");
  }
  if (stateLedChicken == true) {
    digitalWrite(pinLedChicken, HIGH);
    //    Serial.println("b");
  } else {
    digitalWrite(pinLedChicken, LOW);
    //    Serial.println("c");
  }

}
void send_mess(String text, String phone) {
  sim900a.print("AT+CMGF=1\r");
  delay(1000);
  sim900a.print("AT+CMGS=\"" + phone + "\"\r");
  delay(1000);
  sim900a.print(text);
  delay(100);
  sim900a.write(0x1A);
  delay(5000);
  Serial.println("gui tn xong");

}
void make_call(String phone)
{
  //  sim900a.println("ATD" + phone + ";");
  //  delay(10000);
  while (millis() - startMillis1 < 1000) {
    // Chờ 5000ms
    sim900a.println("ATD" + phone + ";");
  }
  Serial.println("da goi xong");
  sim900a.println("ATH");
  delay(1000); //1 sec delay
  Serial.println("goi xong");

}
