#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;
 
// sensor diletakkan di pin 2
#define ONE_WIRE_BUS 5
 // setup sensor
OneWire oneWire(ONE_WIRE_BUS);
 // berikan nama variabel,masukkan ke pustaka Dallas
DallasTemperature sensorSuhu(&oneWire);
 
float suhuSekarang;
 
const int pinRelay[4] = {1, 2, 3, 4}; //ganti nomor pin sesuai degan yang digunakan di masing - masing channel relay
const int pinTombol[4] = {5, 6, 7, 8}; //ganti nomor pin sesuai degan yang digunakan di masing - masing tombol

int modeTombol_aktif = false; //mode tombol aktif low. ubah jadi true jika mode aktif high
int modeRelay_aktif = false; // mode relay aktif low. ubah jadi true jika mode aktfi high

int i;
int j;
int k;

int tekan;
int untekan;

int bacaTombol1;
int bacaTombol2;
int bacaTombol3;
int bacaTombol4;

int on;
int off;

void setup() {
  Serial.begin(115200);  //Initialize serial
  sensorSuhu.begin(); 

  uint32_t currentFrequency; 

  if (! ina219.begin()) { 
    Serial.println("Failed to find INA219 chip");
 while (1) { delay(10); }
 }

 Serial.println("Measuring voltage and current with INA219 ...");

 if (modeTombol_aktif == false){
  for (int a = 0; a < 5; a++){
    pinMode(pinTombol[a], INPUT_PULLUP);
  }
  tekan = LOW;
  untekan = HIGH;
 }
 else{
  for (int a = 0; a < 5; a++){
    pinMode(pinTombol[a], INPUT);
  }
  tekan = HIGH;
  untekan = LOW;
 }

//Array untuk mode output pin relay
 for (i = 0; i < 5; i++){
  pinMode(pinRelay[i], OUTPUT);
 }
 
}

void tombol_relay(){
    
    if (modeRelay_aktif == false){
      on = LOW;
      off = HIGH;
    }
    else{
      on = HIGH;
      off = LOW;
    }
    
    if (bacaTombol1 == tekan){
      digitalWrite(pinRelay[0], on);
    }

    if (bacaTombol2 == tekan){
      digitalWrite(pinRelay[1], on);
    }

    if (bacaTombol3 == tekan){
      digitalWrite(pinRelay[2], on);
    }
    
    if (bacaTombol4 == tekan){
      digitalWrite(pinRelay[3], on);
    }
}

void autoRelay_suhu(){

  if (suhuSekarang <= -20){
    
      digitalWrite(pinRelay[0], off);
  }
  
  if (suhuSekarang >= 10){
    digitalWrite(pinRelay[0], on);
  }
}

void loop() {

 float shuntvoltage = 0;
 float busvoltage = 0;
 float current_mA = 0;
 float loadvoltage = 0;
 float power_mW = 0;

 int bacaTombol1 = digitalRead(pinTombol[0]);
 int bacaTombol2 = digitalRead(pinTombol[1]);
 int bacaTombol3 = digitalRead(pinTombol[2]);
 int bacaTombol4 = digitalRead(pinTombol[3]);
 
 shuntvoltage = ina219.getShuntVoltage_mV();
 busvoltage = ina219.getBusVoltage_V();
 current_mA = (ina219.getCurrent_mA()/1000);
 power_mW = ina219.getPower_mW();
 loadvoltage = busvoltage + (shuntvoltage / 1000);
 
  suhuSekarang = ambilSuhu();

  tombol_relay();
  autoRelay_suhu();
  
  Serial.println(suhuSekarang);     
  delay(2000);
 // Get a new temperature reading
  Serial.print("Temperature: ");
  Serial.print(suhuSekarang);
  Serial.println(" ºC");
  
 Serial.print("Bus Voltage: "); 
 Serial.print(busvoltage); 
 Serial.println("V");
 Serial.print("Current: "); 
 Serial.print(current_mA); 
 Serial.println("mA");
 delay(2000);
}

float ambilSuhu()
{
   sensorSuhu.requestTemperatures();
   float suhu = sensorSuhu.getTempCByIndex(0);
   return suhu;   
}
