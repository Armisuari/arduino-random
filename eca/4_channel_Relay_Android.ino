#include <SoftwareSerial.h> 
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>

#define ONE_WIRE_BUS 6

SoftwareSerial module_bluetooth(0, 1); // pin RX | TX
OneWire oneWire (ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(0x27,16,2);

float suhuAtas, suhuBawah;
char data = 0;
int a,b;
            
void setup() {
  Serial.begin(9600);     
  lcd.init(); 
  lcd.backlight();     
  pinMode(2, OUTPUT);  //inisialisasi PIN 2 Menjadi Output
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  digitalWrite(2, HIGH); // pada kondisi awal dibuat high supaya relay mati (karena modul relay aktif low)
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH); 
}

void loop(){
  sensors.setResolution(10);
  sensors.requestTemperatures();
  suhuAtas  = (sensors.getTempCByIndex(0));
  suhuBawah = (sensors.getTempCByIndex(1));

    //if(suhuAtas >= 20){
    //digitalWrite(2, LOW); //nyalakan relay
    //}
    //else if(suhuAtas <= 16){      
    //digitalWrite(2, HIGH); //matikan relay
    //} 

    if (suhuAtas >= 20) 
       {
         digitalWrite(2, LOW);
         a = 1;
       }
    
    if (suhuAtas >16 && suhuAtas <20) 
       {
        if (a == 1)
          {
            digitalWrite(2, LOW);
          }
       }

    if (suhuAtas <= 16) 
       {
         digitalWrite(2, HIGH);
         a = 0;
       }

    else if(suhuBawah <= 45){
    digitalWrite(3, LOW); 
    }
    else if(suhuBawah >= 50){      
    digitalWrite(3, HIGH); 
    } 
   
  if(Serial.available() > 0)  
  {
    data = Serial.read(); //baca data yang dikirim      
    if(data == '5'){
    digitalWrite(4, LOW); 
    }
    else if(data == '6'){      
    digitalWrite(4, HIGH); 
    } 
    else if(data == '7'){
    digitalWrite(5, LOW); 
    }
    else if(data == '8'){      
    digitalWrite(5, HIGH); 
    } 
  }
   lcd.setCursor(0,0);
   lcd.print("SuhuA:");
   lcd.setCursor(7,0);
   lcd.print(suhuAtas);
   lcd.setCursor(10,0);
   lcd.print(" C");
   lcd.setCursor(0,1);
   lcd.print("SuhuB:");
   lcd.setCursor(7,1);
   lcd.print(suhuBawah);
   lcd.setCursor(11,1);
   lcd.print(" C");   
 }
