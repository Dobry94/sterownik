#include <LiquidCrystal.h>
LiquidCrystal lcd(24,25,26,27,28,29);

#include <OneWire.h>
OneWire oneWire(31);

#include <DallasTemperature.h>
DallasTemperature sensors(&oneWire);
byte addressTs[8] {0x28, 0xAF, 0xD5, 0x10, 0x0B, 0x00, 0x00, 0x13};
byte addressTk[8] {0x28, 0xFF, 0x55, 0xBC, 0x62, 0x15, 0x1, 0x2B};
byte addressTb[8] {0x28, 0xFF, 0xD2, 0xCE, 0x62, 0x15, 0x1, 0x1E};
byte addressTw[8] {0x28, 0xFF, 0x65, 0xA1, 0x73, 0x15, 0x2, 0x9};

#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel pixels(5, 17, NEO_GRB + NEO_KHZ800);  //powinien byc 18 ale jest zlutowany z 19 :(

#include <avr/wdt.h>
#include <EEPROM.h>

const uint8_t ps_button=10, pk_button=11, pc_button=12, e_button=13, g_button=14;
const uint8_t buzzer=6, Ps=19, Pk=20, Pc=21, G=22, E=23;
uint8_t ps_state=2,pk_state=2,pc_state=2,e_state=2,g_state=2;
int8_t Ts,Tk,Tb,Tw;
const int czas_t1=20;       //0,02 sekundy
const int czas_t2=5000;       //5 sekund
const int czas_t3=1000*60*5;  //5 minut
int t1=czas_t1;
int t2=czas_t2;
int t3=czas_t3;
/************************************************************************************************************/
void klawiaturka(){
  if(digitalRead(ps_button)==LOW){
    while(digitalRead(ps_button)==LOW)delay(10);
    if(ps_state>=2)ps_state=0;
    else ps_state++;
  }
  if(digitalRead(pk_button)==LOW){
    while(digitalRead(pk_button)==LOW)delay(10);
    if(pk_state>=2)pk_state=0;
    else pk_state++;
  }
  if(digitalRead(pc_button)==LOW){
    while(digitalRead(pc_button)==LOW)delay(10);
    if(pc_state>=2)pc_state=0;
    else pc_state++;
  }
  if(digitalRead(e_button)==LOW){
    while(digitalRead(e_button)==LOW)delay(10);
    if(e_state>=2)e_state=0;
    else e_state++;
  }
  if(digitalRead(g_button)==LOW){
    while(digitalRead(g_button)==LOW)delay(10);
    if(g_state>=2)g_state=0;
    else g_state++;
  }
}
/*************************************************************************************************************/
void sterowanie(){
  if(ps_state==0){
    pixels.setPixelColor(0, pixels.Color(0, 0, 0));
    pixels.show();
    digitalWrite(Ps,LOW);
    lcd.setCursor(0,3);
    lcd.print("0 ");
    }
  if(ps_state==1){
    pixels.setPixelColor(0, pixels.Color(20, 20, 20));
    pixels.show();
    digitalWrite(Ps,HIGH);
    lcd.setCursor(0,3);
    lcd.print("1 ");
    }
  if(ps_state==2){
    if(-20<Ts&&Ts<=20||120<Ts||120<Tk||20<Ts&&Ts<=120&&Ts-Tb<4) {
      pixels.setPixelColor(0, pixels.Color(30, 0, 0));
      pixels.show();
      digitalWrite(Ps,LOW);
      lcd.setCursor(0,3);
      lcd.print("A0");}
    else if(Ts<=-20||20<Ts&&Ts<=120&&Ts-Tk>=10||90<Tk&&Tk<=120||Ts==-127) {
      pixels.setPixelColor(0, pixels.Color(0, 30, 0));
      pixels.show();
      digitalWrite(Ps,HIGH);
      lcd.setCursor(0,3);
      lcd.print("A1");}
  }
  
  if(pk_state==0){
    pixels.setPixelColor(1, pixels.Color(0, 0, 0));
    pixels.show();
    digitalWrite(Pk,LOW);
    lcd.setCursor(4,3);
    lcd.print("0 ");
    }
  if(pk_state==1){
    pixels.setPixelColor(1, pixels.Color(20, 20, 20));
    pixels.show();
    digitalWrite(Pk,HIGH);
    lcd.setCursor(4,3);
    lcd.print("1 ");
    }
  if(pk_state==2){
    if(50<Tk) {
      pixels.setPixelColor(1, pixels.Color(0, 30, 0));
      pixels.show();
      digitalWrite(Pk,HIGH);
      lcd.setCursor(4,3);
      lcd.print("A1");}
    else {
      pixels.setPixelColor(1, pixels.Color(30, 0, 0));
      pixels.show();
      digitalWrite(Pk,LOW);
      lcd.setCursor(4,3);
      lcd.print("A0");
      }
  }
  
    if(pc_state==0){
    pixels.setPixelColor(2, pixels.Color(0, 0, 0));
    pixels.show();
    digitalWrite(Pc,LOW);
    lcd.setCursor(8,3);
    lcd.print("0 ");
    }
  if(pc_state==1){
    pixels.setPixelColor(2, pixels.Color(20, 20, 20));
    pixels.show();
    digitalWrite(Pc,HIGH);
    lcd.setCursor(8,3);
    lcd.print("1 ");
    }
  if(pc_state==2){
    if(75<Tb && 75<Tk || Ts>90 && Tb>80) {
      pixels.setPixelColor(2, pixels.Color(0, 30, 0));
      pixels.show();
      digitalWrite(Pc,HIGH);
      lcd.setCursor(8,3);
      lcd.print("A1");}
      else {
      pixels.setPixelColor(2, pixels.Color(30, 0, 0));
      pixels.show();
      digitalWrite(Pc,LOW);
      lcd.setCursor(8,3);
      lcd.print("A0");}
  }
  
    if(e_state==0){
    pixels.setPixelColor(3, pixels.Color(0, 0, 0));
    pixels.show();
    digitalWrite(E,LOW);
    lcd.setCursor(12,3);
    lcd.print("0 ");
    }
  if(e_state==1){
    pixels.setPixelColor(3, pixels.Color(20, 20, 20));
    pixels.show();
    digitalWrite(E,HIGH);
    lcd.setCursor(12,3);
    lcd.print("1 ");
    }
  if(e_state==2){
    if(Tb>85) {
      pixels.setPixelColor(3, pixels.Color(0, 30, 0));
      pixels.show();
      digitalWrite(E,HIGH);
      lcd.setCursor(12,3);
      lcd.print("A1");}
    else {
      pixels.setPixelColor(3, pixels.Color(30, 0, 0));
      pixels.show();
      digitalWrite(E,LOW);
      lcd.setCursor(12,3);
      lcd.print("A0");
      }
  }
 
  
    if(g_state==0){
    pixels.setPixelColor(4, pixels.Color(0, 0, 0));
    pixels.show();
    digitalWrite(G,LOW);
    lcd.setCursor(16,3);
    lcd.print("0 ");
    }
  if(g_state==1&&Tb<70){
    pixels.setPixelColor(4, pixels.Color(20, 20, 20));
    pixels.show();
    if(Tb>65)g_state=2;
    digitalWrite(G,HIGH);
    lcd.setCursor(16,3);
    lcd.print("1 ");
    }
  if(g_state==2){
    if(Tb==-127||Tb==0){
    pixels.setPixelColor(4, pixels.Color(30, 0, 0));
    pixels.show();
      digitalWrite(G,LOW);
      lcd.setCursor(16,3);
      lcd.print("A0");}
    else if(Tb<=30||Ts<=-30&&Ts!=-127) {
    pixels.setPixelColor(4, pixels.Color(0, 30, 0));
    pixels.show();
      digitalWrite(G,HIGH);
      lcd.setCursor(16,3);
      lcd.print("A1");}
    else {
      digitalWrite(G,LOW);
      lcd.setCursor(16,3);
      lcd.print("A0");}
  } 
}
/***********************************************************************************************************/
void temperatury(){
  uint8_t availab[8];
  if(oneWire.search(availab)){
  sensors.requestTemperatures();
  Ts=sensors.getTempC(addressTs);lcd.setCursor(0,1);lcd.print(Ts);lcd.print("  ");
  Tk=sensors.getTempC(addressTk);lcd.setCursor(4,1);lcd.print(Tk);lcd.print("  ");
  Tb=sensors.getTempC(addressTb);lcd.setCursor(8,1);lcd.print(Tb);lcd.print("  ");
  Tw=sensors.getTempC(addressTw);lcd.setCursor(12,1);lcd.print(Tw);lcd.print("  ");
  }
}
/**********************************************************************************************************/
void alarm(){
  if(Ts>=90)      digitalWrite(buzzer,HIGH);
  else if(Tk>=90) digitalWrite(buzzer,HIGH);
  else if(Tb>=90) digitalWrite(buzzer,HIGH);
  else if(Tw>=90) digitalWrite(buzzer,HIGH);
  else if(Ts<90||Tk<90||Tb<90||Tw<90) digitalWrite(buzzer,LOW);
}
/**********************************************************************************************************/
void zapisz_stany(){
  EEPROM.write(Ps, ps_state);
  EEPROM.write(Pk, pk_state);
  EEPROM.write(Pc, pc_state);
  EEPROM.write(E, e_state);
  EEPROM.write(G, g_state);
}
/**********************************************************************************************************/



void setup() {
  lcd.begin(20,4);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Ts  Tk  Tb  Tw "); 
  lcd.setCursor(0,2);
  lcd.print("Ps  Pk  Pc  E   G    ");
  pinMode(3,OUTPUT); //podświetlenie ekranu
  //digitalWrite(3,HIGH); //podświetlenie ekranu
  analogWrite(3,30);
  
  sensors.begin();
  
//  ps_state=EEPROM.read(Ps);
//  pk_state=EEPROM.read(Pk);
//  pc_state=EEPROM.read(Pc);
//  e_state=EEPROM.read(E);
//  g_state=EEPROM.read(G);

  wdt_enable(WDTO_2S);

  pinMode(ps_button,INPUT_PULLUP);  //ustawienie przyciskow na output 
  pinMode(pk_button,INPUT_PULLUP);  //likwiduje drgania styku "samoistne wcisniecia"
  pinMode(pc_button,INPUT_PULLUP);  //powodujace zmiany stanow
  pinMode(e_button,INPUT_PULLUP);
  pinMode(g_button,INPUT_PULLUP);

  pinMode(Ps,OUTPUT);   //zlutowany z 19 niestety
  pinMode(Pk,OUTPUT);
  pinMode(Pc,OUTPUT);
  pinMode(E,OUTPUT);
  pinMode(G,OUTPUT);
  pinMode(buzzer,OUTPUT);

  pixels.begin();
  
  temperatury();
  delay(20); //do prawidłowego odczytania temperatur
  
}

void loop() {
 
  if(!t1){//proces1 odswierzanie co czas_t1
  klawiaturka();
  sterowanie();
  wdt_reset();
  t1=czas_t1;
  }
  if(!t2){//co 30sek
  //lcd.clear();
  temperatury();
  //alarm(); 
  //zapisz_stany();
  t2=czas_t2;
  }
delay(1);
t1--;
t2--;
t3--;
}
