#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

#define EEPROM_SIZE 128

LiquidCrystal_I2C My_LCD (0x27, 16, 2);

char auth[] = "Tonc3spIupKUhP-ep1KZvJSMgQ9j5Rk1";
char ssid[] = "poco";  //char ssid[] = "Variasi Aluminium";
char pass[] = "1234567890";     //char pass[] = "hapisahsyukur1";

const int A = 23;
const int B = 34;
unsigned long masuk = 0;
unsigned long keluar = 0;
long total = 0;
int a = 0;
int b = 0;
int c = 0;
int d = 0;
int baca_A;
int baca_B;
int waktu;
long rom = 1;
long rom2 = 2;
long rom3 = 3;
  
unsigned long lastPublish = 0;
int interval = 1;

BlynkTimer timer;

void readSensor() {

  sensormasuk();
  c = 0;
  sensorkeluar();
  d = 0;
  if ( total < 0) {
    total = 0;
    EEPROM.write(rom3, total);
    EEPROM.commit();
  }
}

void sensormasuk() {

  if (baca_A == 0 && baca_B == 1 && a == 0 && b == 0 && c == 0) {
    a = 1;
  }
  if (baca_A == 1 && baca_B == 0 && a == 1 && b == 0) {
    a = 2;
  }

  if (baca_A == 1 && baca_B == 1 && a == 2 && b == 0) {
    c = 1;
  }
  if (a == 2 && c == 1) {

    My_LCD.clear();
    
    masuk++;
    EEPROM.write(rom, masuk);
    EEPROM.commit();

    total++;
    EEPROM.write(rom3, total);
    EEPROM.commit(); 
    
    a = 0;
    b = 0;
  }
}

void sensorkeluar() {

  if (baca_A == 1 && baca_B == 0 && a == 0 && b == 0 && c == 0) {
    b = 1;
  }
  if (baca_A == 0 && baca_B == 1 && a == 0 && b == 1) {
    b = 2;
  }
  if (baca_A == 1 && baca_B == 1 && a == 0 && b == 2) {
    d = 1;
  }
  if (b == 2 && d == 1) {

    My_LCD.clear();
    
    keluar++;
    EEPROM.write(rom2, keluar);
    EEPROM.commit();


    total--;
    EEPROM.write(rom3, total);
    EEPROM.commit();
    
    a = 0;
    b = 0;
  }
}

void sendDatas()
{
  Blynk.virtualWrite(V0, masuk);
  Blynk.virtualWrite(V1, keluar);
  Blynk.virtualWrite(V2, total);
}

void setup() {
  // put your setup code here, to run once:
  Blynk.begin(auth, ssid, pass);
  
  My_LCD.begin();
  My_LCD.clear();
    
  Serial.begin(115200);
  
  EEPROM.begin(EEPROM_SIZE);
  
  pinMode(A, INPUT_PULLUP);
  pinMode(B, INPUT_PULLUP);
 
  EEPROM.write(rom,0);
  EEPROM.write(rom2,0);
  EEPROM.write(rom3,0);

  masuk = EEPROM.read(rom);
  keluar = EEPROM.read(rom2);
  total = EEPROM.read(rom3);
  
  timer.setInterval(1000L, sendDatas);
}

void loop() {
  
  baca_A = digitalRead(A);
  baca_B = digitalRead(B);

  readSensor();

  My_LCD.setCursor(0,0);
  My_LCD.print("M :");
  My_LCD.print(masuk);
  My_LCD.setCursor(9,0);
  My_LCD.print("K :");
  My_LCD.print(keluar);

  My_LCD.setCursor(4 ,1);
  My_LCD.print("Total :");
  My_LCD.print(total);
  
  Serial.print("masuk = ");
  Serial.print(masuk);
  Serial.print("   ");
  Serial.print("keluar = ");
  Serial.print(keluar);
  Serial.print("   ");
  Serial.print("total = ");
  Serial.println(total);

  Blynk.run();
  timer.run();
}
