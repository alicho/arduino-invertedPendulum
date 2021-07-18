#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <stdio.h>


LiquidCrystal_I2C lcd(0x27,16,2);
const byte potPin=A15;
double potDeger;
double aciDeger;
double aciMutlakDeger;
long hizDeger,hizglobal;

long derivative=0,prop=0,integ=0,deriv=0,power_difference=0,error=0,last_error=0;
double kp=0,kd=0,ki=0;//konfigürasyon değerleri
 int minhiz=0,maxhiz=255;
const int in8=8;
const int in9=9;
const int in10=10;
const int in11=11;
const int in12=12;
const int in13=13;//hız

void setup() {
  // put your setup code here, to run once:
  
  //Serial.begin(9600);
  lcd.begin();
  pinMode(in8, INPUT);
  pinMode(in9, OUTPUT);
  pinMode(in10, OUTPUT);
  pinMode(in11, OUTPUT);
  pinMode(in12, OUTPUT);
  pinMode(in13, INPUT);

}

void loop() {
 
  kp=5000,kd=400;ki=0,0001;//konfigürasyon test değerleri
  //potOku();
  aciHesapla();
  pid();
  ekran();
}

void pid(){
  potDeger=analogRead(potPin);
  error=0-map(potDeger,154,946,-255,255);//
  derivative=error-last_error;//hata değişimi
  prop=long(error)*kp/long(1000);
  deriv=derivative*kd/long(100);
  integ=integ+long(error);
  if(abs(error<50))integ=0;
  integ=integ*ki;
  power_difference=prop+deriv+integ;
  last_error=error;
  
  set_motors(-power_difference*10);
  
  }
void set_motors(long hiz){
  
  if(hiz<0){
    hiz=-hiz;
  if(hiz<minhiz)hiz=minhiz;
  if(hiz>maxhiz)hiz=maxhiz;
  hizglobal=hiz;
  analogWrite(in8,hiz);
  digitalWrite(in9,0);
  digitalWrite(in10,1);
  digitalWrite(in11,1);
  digitalWrite(in12,0);
  analogWrite(in13,hiz);
  }else{
    if(hiz<minhiz)hiz=minhiz;
    if(hiz>maxhiz)hiz=maxhiz;
    hizglobal=hiz;
  analogWrite(in8,hiz);
  digitalWrite(in9,1);
  digitalWrite(in10,0);
  digitalWrite(in11,0);
  digitalWrite(in12,1);
  analogWrite(in13,hiz);
  }
}

void potOku(){
  potDeger=analogRead(potPin);
 // Serial.print("Pot Degeri:");
 // Serial.println(potDeger);
}

void aciHesapla(){
  aciDeger=map(potDeger,154,946,-90,90);
  //aciMutlakDeger=abs(aciDeger);
}

void ekran(){
    lcd.setCursor(0,0);
    lcd.print("P:");
    lcd.setCursor(2,0);
    lcd.print(potDeger);
    //------------------
    lcd.setCursor(0,1);
    lcd.print("A:");
    lcd.setCursor(2,1);
    lcd.print(aciDeger);
    //-----------------
    lcd.setCursor(9,1);
    lcd.print("V:");
    lcd.setCursor(11,1);
    lcd.print(abs(hizglobal));
}
