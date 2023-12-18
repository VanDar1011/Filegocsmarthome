#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <EEPROM.h>
#include <AFMotor.h>
#include <NewPing.h>
#include "DHT.h"
#define LCD_I2C_ADDR 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);
//sensor_led
#define TRIG_PIN 12
#define ECHO_PIN 13
#define MAX_DISTANCE 200
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
const int pinLedGround = 6 ;
const int pinTouchLedGround = 8;
const int pinLedGuest = 11;
const int pinTouchLedGuest = 10;
const int pinDHT = 9;
const int DHTTYPE = DHT11;
const int pinResistanceOptical = 7;
DHT dht(pinDHT, DHTTYPE);
float h;
float t;
// ----
bool stateLedGround = false;
bool stateLedGuest = false;
bool flagResistance = true;
bool flagLedGroundDay = true;
bool loopState = true;
// -----
bool stateLock = false;
int distance = 0;
//end sensor_led

//keypad
const byte ROW_NUM = 4;
const byte COL_NUM = 4;
boolean cf = true;
char keys[ROW_NUM][COL_NUM] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
byte pin_rows[ROW_NUM] = { 5, 4, 3, 2 };
byte pin_cols[COL_NUM] = { A3, A2, A1, A0 };
Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_cols, ROW_NUM,
                       COL_NUM);
String password;
String passwordChange = "";
String passwordOld = "";
bool codeEntered = false; // trang thai pass da dung hay chua
String pwd = ""; // khai bao chuoi de luu pass nhap tu ban phim
int n = 0; // khai bao so gia tri duoc nhap vao tu ban phim
int resetPwd = 0; // khai bao gia tri so lan nhap pass
int nhay = 0;
//----- end keypad


void setup() {
  Serial.begin(115200);
  pinMode(pinLedGuest, OUTPUT);
  pinMode(pinLedGround, OUTPUT);
  
  pinMode(pinTouchLedGuest, INPUT);
  pinMode(pinTouchLedGround, INPUT);
  pinMode(pinResistanceOptical, INPUT);
  lcd.init();
  lcd.backlight();
  introScreen();
  delay(2000);
}

void loop() {
  password = readStringFromEEPROM(0);
  // Serial.println(readStringFromEEPROM(0));
  if (!codeEntered) {
    checkCode();
  }
  else {
    grantAccess();
  }
  readValue_LedGround();
  readValue_LedGuest();
  distanceHcsr04();
  readTemp();

}
// funciton sensor
void readValue_LedGuest() {
  if (digitalRead(pinTouchLedGuest) == HIGH ) {
    stateLedGuest = ! stateLedGuest;
  }
  if (stateLedGuest == true) { 
    digitalWrite(pinLedGuest, HIGH);
  } else {
    digitalWrite(pinLedGuest, LOW);
  }
}
void readValue_LedGround() {
  if ((digitalRead(pinResistanceOptical) == LOW) && (flagResistance == true )) {
    stateLedGround = true;
    if (digitalRead(pinTouchLedGround) == HIGH) {
      flagResistance = false;
      stateLedGround = false;
    }
    loopState = true;
  }
  if ((digitalRead(pinResistanceOptical) == LOW) && (flagResistance == false )) {
    if (digitalRead(pinTouchLedGround) == HIGH) {
      stateLedGround = !stateLedGround;
    }
    if (digitalRead(pinResistanceOptical) == HIGH) {
      flagLedGroundDay = true;
    }
    loopState = true;
  }
  if (digitalRead(pinResistanceOptical) == HIGH) {
    if (stateLedGround == true && loopState == true ) {
      stateLedGround = false;
      loopState = false;
    }
    else {
      if (flagResistance == false) {
        flagResistance = true;
      }

      if (true) {
        if (digitalRead(pinTouchLedGround) == HIGH) {
          stateLedGround = !stateLedGround;
        }

      }
      if (stateLedGround == true && flagLedGroundDay == true) {
        stateLedGround = false;
        flagLedGroundDay  = false;
      }
    }

  }

  Serial.println(stateLedGround);
  if (stateLedGround == true) {
    digitalWrite(pinLedGround, HIGH);
  }
  else {
    digitalWrite(pinLedGround, LOW);
  }

}



//int readPing() {
//  delay(70);
//  int cm = sonar.ping_cm();
//  return cm;
//}
void readTemp() {
  h = dht.readHumidity();    //Đọc độ ẩm
  t = dht.readTemperature(); //Đọc nhiệt độ
  Serial.print("Nhiet do: ");
  Serial.println(t);               //Xuất nhiệt độ
  Serial.print("Do am: ");
  Serial.println(h);               //Xuất độ ẩm
  Serial.println();
  if (t > 37) {
    Serial.println("Bat dieu hoa di");
  }
}
void distanceHcsr04() {
  distance = (sonar.ping_cm());
  while (distance == 0) //NewPing is supposed to return 0
  { //when ping is out of range
    distance = 255;
    delay(50);
  }
  unsigned long startMillis = millis();
  while (millis() - startMillis > 3000) {
    // Chờ 50ms
    stateLock = true;
  }  stateLock = false;
}
// end function sensor
//function keypad
void checkCode() {
  char key = keypad.getKey(); // lay gia tri nhap tu keypad
  if (key != NO_KEY) {
    if (key == '*') {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Change password");
      lcd.setCursor(0, 1);
      lcd.print("1.Ok");
      lcd.setCursor(8, 1);
      lcd.print("2.Exit");
      while (cf) {
        key = keypad.getKey();
        if (key != NO_KEY) {
          cf = false;
        }
      }
      if (key == '1') {
        changePass();
      }
      else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Enter password"); // nhap mat khau
        lcd.setCursor(0, 1);

      }

    }
    else {
      cf = true;
      if (key == '#') {

        if (pwd.equalsIgnoreCase(password)) {
          codeEntered = true; // dung thi thi set tragn thai codeEntered = true de mo cua
        } else {


          passIncorrect();
        }
        n = 0; // set gia tri do dai mk ve 0 de nhap lai
      }

      else  { // neu la key
        //delay(50);
        unsigned long startMillis2 = millis();
        while (millis() - startMillis2 < 50) {
          // Chờ 50ms
        }
        pwd += key; // noi key vao password
        lcd.setCursor(n, 1); // set vi tri hien thi
        lcd.print("*"); // hien thi dau * thay vi key
        n++;
      }
    }

  }
}


void grantAccess() { // ham mo cua
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("Access");
  lcd.setCursor(4, 1);
  lcd.print("Granted!");
  codeEntered = false; // setcodeEntered ve khong de nguoi khac co the mo
  pwd = ""; // set pwd ve rong de nhap pass lai

  unsigned long startMillis = millis();
  while (millis() - startMillis < 3000) {
    // Chờ 50ms
    stateLock = true;
  }
  stateLock = false;
  

  
  //-------------------------
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter password:");
}
void introScreen() { // hien thi nhung trang thai ban dau
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Smart Home");
  lcd.setCursor(2, 1);
  lcd.print("Welcome back");
  // delay(600);
  unsigned long startMillis = millis();
  while (millis() - startMillis < 600) {
    // Chờ 50ms
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter password:");
  lcd.setCursor(0, 1);
}
void changePass() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter old pwd:");
  char keyChange;

  do {
    keyChange = keypad.getKey();
    if (keyChange == '#') {
      break;
    }
    if (keyChange) {

      lcd.setCursor(nhay, 1);
      lcd.print("*");
      passwordOld += keyChange;
      nhay++;
    }

  } while (keyChange != '#');


  nhay = 0;
  if (passwordOld.equalsIgnoreCase(password)) {

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("New password");
    do {
      keyChange = keypad.getKey();
      if (keyChange) {
        lcd.setCursor(nhay, 1);
        lcd.print("*");
        passwordChange += keyChange;
        nhay++;
      }
    } while (passwordChange.length() < 4);
    delay(50);
    password = passwordChange;
    writeStringIntoEEPROM(0, passwordChange);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Change success");
    delay(150);

  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Incorrect.Not Change");
    delay(150);
  }
  nhay = 0;
  passwordOld = "";
  passwordChange = "";
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter password:"); // nhap mat khau
  lcd.setCursor(0, 1);

}
void passIncorrect() {
  lcd.setCursor(0, 0); // set vi tri de hien thi chu "Mat khau sai" h1c1
  lcd.print("Incorrect"); // hien thi chu
  resetPwd++;  // so lan go mk tang len
  if (resetPwd == 4) { // qua 4 lan go mk sai thi thong bao go sai, bat cho 5s de co the go tiep
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Try again after 5s");
    // delay(5000); // cho 5s
    unsigned long startMillis = millis();
    while (millis() - startMillis < 5000) {
      // Chờ 5000ms
    }
    resetPwd = 0;// dat so lan go sai password ve 0 de co the tiep tuc nhap

  }
  lcd.setCursor(0, 1);
  lcd.print("Try again"); // nhap lai mat khau
  //delay(500);
  unsigned long startMillis1 = millis();
  while (millis() - startMillis1 < 600) {
    // Chờ 500ms
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter password:"); // nhap mat khau
  lcd.setCursor(0, 1);
  pwd = "";  // pwd ve chuoi rong de nhap lai
}
void writeStringIntoEEPROM(int address, String str) {
  byte len = str.length();
  EEPROM.write(address, len);
  for (int i = 0; i < len; i++) {
    EEPROM.write(address + 1 + i, str[i]);
  }
}
String readStringFromEEPROM(int address) {
  int len = EEPROM.read(address);
  char data[len + 1];
  for (int i = 0 ; i < len ; i++) {
    data[i] = EEPROM.read(address + 1 + i);
  }
  data[len] = '\0';
  return String(data);
}
//end function keypad
