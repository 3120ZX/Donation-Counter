#include <ESP32Servo.h>
#include <analogWrite.h>
#include <ESP32Tone.h>
#include <ESP32PWM.h>
#include <LiquidCrystal_I2C.h>

#define S0 12
#define S1 13
#define S2 26
#define S3 27
#define OUT 5
#define button_1 34
#define button_2 35
#define SERVO_PIN 15 

int lcdColumns = 16;
int lcdRows = 2;
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;
int r, g, b;
int input;
int money;

Servo servoMotor;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 

const int rows = 28;
const int columns = 4;
int moneyFreq[rows][columns] = {
  //Sisi Pertama
  {17,21,19,1000},
  {18,21,16,2000},
  {15,22,19,5000},
  {21,23,16,10000},
  {24,14,18,20000},
  {18,18,15,50000},
  {29,18,15,100000},
  
  //Sisi Kedua
  {13,17,15,1000},
  {14,15,12,2000},
  {11,15,15,5000},
  {17,19,11,10000},
  {25,25,17,20000},
  {15,13,9,50000},
  {27,23,19,100000},

  //Sisi Ketiga
  {13,16,15,1000},
  {13,14,11,2000},
  {12,18,17,5000},
  {21,26,18,10000},
  {16,23,19,20000},
  {15,15,13,50000},
  {24,23,13,100000},

  //Sisi Keempat
  {18,24,19,1000},
  {15,18,16,2000},
  {10,15,14,5000},
  {14,16,10,10000},
  {19,26,23,20000},
  {13,11,7,50000},
  {21,17,27,100000}
  };

void setup(){
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  lcd.init();                  
  lcd.backlight();
  servoMotor.attach(SERVO_PIN);
  pinMode(button_1, INPUT);
  pinMode(button_2, INPUT);
  Serial.begin(9600); 
}

void loop(){
  int IR = digitalRead(32);
  servoMotor.write(100);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Silakan Masukkan");
  lcd.setCursor(0, 1);
  lcd.print("uang anda");
  while(IR  ==  LOW){
  r = process_red_value();
  delay(200);
  g = process_green_value();
  delay(200);
  b = process_blue_value();
  delay(200);
  Serial.print("r = ");
  Serial.print(r);
  Serial.print(" ");
  Serial.print("g = ");
  Serial.print(g);
  Serial.print(" ");
  Serial.print("b = ");
  Serial.print(b);
  Serial.print(" ");
  Serial.println();
  delay(1000);
  moneyChecker();
  delay(1000);
    }
}    
int process_red_value(){
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  int pulse_length = pulseIn(OUT, LOW);
  return pulse_length;
  }

int process_green_value(){
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  int pulse_length = pulseIn(OUT, LOW);
  return pulse_length;
  }
  
int process_blue_value(){
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  int pulse_length = pulseIn(OUT, LOW);
  return pulse_length;
  }

int moneyChecker(){
  int mark = 0;
  for ( int i = 0; i < rows; ++i ) {
    if ((moneyFreq[i][0]-2)< r && r <(moneyFreq[i][0]+2) &&
        (moneyFreq[i][1]-2)< g && g <(moneyFreq[i][1]+2) &&
        (moneyFreq[i][2]-2)< b && b <(moneyFreq[i][2]+2)){
      Serial.println(moneyFreq[i][3]);
      money = moneyFreq[i][3];
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Apakah uang anda");
      lcd.setCursor(0, 1);
      lcd.print(moneyFreq[i][3]);
      lcd.setCursor(7, 1);
      lcd.print("???");
      confirmation();
      break;
      }   
    else{
      continue;
      }
    }
    
  Serial.println("Done Checking");
  }

int confirmation(){
  int status_1 = digitalRead(button_1);
  int status_2 = digitalRead(button_2);
  Serial.print("Button 1:");
  Serial.println(status_1);
  Serial.print("Button 2:");
  Serial.println(status_2);
  if(status_1 == HIGH){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Terimakasih Atas");
    lcd.setCursor(0, 1);
    lcd.print("Sumbangan Anda");
    servoMotor.write(0);
    delay(2000);
    }
  else if(status_2 == HIGH){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Silakan Dicoba");
    lcd.setCursor(0, 1);
    lcd.print("Kembali");
    }
  }
