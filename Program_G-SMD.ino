#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial sim800l(11, 10);
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define sens A0
#define buzzer 2
#define ch1 7
#define ch2 8

String noTujuan = "087851600295";
int readSens, state = 0, lcds = 0, persentase;

void setup() {
  Serial.begin(9600);
  sim800l.begin(9600);
  lcd.begin();
  pinMode(sens, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(ch1, OUTPUT);
  pinMode(ch2, OUTPUT);
  lcd.setCursor(5,0);
  lcd.print("G-SMD");
  lcd.setCursor(2,1);
  lcd.print("by--riskyeKA");
  delay(5000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Gas Bocor: ");
}

void relayON(){
  digitalWrite(ch1, HIGH);
  digitalWrite(ch2, HIGH);
}

void relayOFF(){
  digitalWrite(ch1, LOW);
  digitalWrite(ch2, LOW);
}

void buzzBahaya(){
  tone(buzzer, 2000);
  delay(100);
  noTone(buzzer);
  delay(100);
}

void sendSMS(){
  sim800l.write("AT\r\n");
  delay(10);
  sim800l.write("AT+CMGF=1\r\n");
  delay(10);
  sim800l.println("AT+CMGS=\"" + noTujuan + "\"\r\n");
  delay(10);
  sim800l.write("BAHAYA GAS BOCOR");
  delay(10);
  sim800l.write((char)26);
  delay(10);
}

void utama(){
  readSens = analogRead(sens);
  readSens = map(readSens, 0, 1024, 0, 100);
  lcd.setCursor(11,0);
  lcd.print(readSens);
  lcd.setCursor(15,0);
  lcd.print("%");
  if(readSens > 10){
    if(lcds == 1){
      lcd.setCursor(0,1);
      lcd.print("                ");
      lcd.setCursor(4,1);
      lcd.print("BAHAYA!!");
      lcds = 0;
    }
    if(state == 0){
      sendSMS();
      state = 1;
    }
    buzzBahaya();
    relayON();
  }

  else{
    if(lcds == 0){
      lcd.setCursor(0,1);
      lcd.print("                ");
      lcd.setCursor(5,1);
      lcd.print("AMAN!!");
      lcds = 1;
    }
    noTone(buzzer);
    relayOFF();
    state = 0;
  }
  lcd.setCursor(11,0);
  lcd.print("   ");
}

void loop() {
  utama();
}
