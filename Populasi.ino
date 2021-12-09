#include <WiFi.h>
#include <FirebaseESP32.h>
#include <EEPROM.h>

#define WIFI_SSID "MENTARI 07"
#define WIFI_PASSWORD "mentari0707"
//#define WIFI_SSID "electricshock"
//#define WIFI_PASSWORD "choicechoice"


#define FIREBASE_HOST "https://waletqu-f32d7.firebaseio.com/"
#define FIREBASE_AUTH "CMcLN8kWGMomhzCraXYYTOLahf0kBUACT7323pBY"


FirebaseData firebaseData;

#define EEPROM_SIZE 128

const int A = 34;
const int B = 23;
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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  pinMode(A, INPUT_PULLUP);
  pinMode(B, INPUT_PULLUP);
//
//EEPROM.write(rom,0);
//EEPROM.write(rom2,0);
//EEPROM.write(rom3,0);
//
//EEPROM.commit();
  
  masuk = EEPROM.read(rom);
  keluar = EEPROM.read(rom2);
  total = EEPROM.read(rom3);

  
}

void loop() {
  // put your main code here, to run repeatedly:

  if(WiFi.status() != WL_CONNECTED){

   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  }

  baca_A = digitalRead(A);
  baca_B = digitalRead(B);

 // online_cek();

  readSensor();

  Serial.print("masuk = ");
  Serial.print(masuk);
  Serial.print("   ");
  Serial.print("keluar = ");
  Serial.print(keluar);
  Serial.print("   ");
  Serial.print("total = ");
  Serial.println(total);

  if (millis() - lastPublish >= interval*60000){

   int timeapp;
   int resett = 0;

   if(Firebase.getInt(firebaseData, "/Populasi/Reset/reset"))
    {
      //succes
      resett = firebaseData.intData();
    }else{

      Serial.print("Error in get on data, ");
      Serial.print(firebaseData.errorReason());
    }

   
    if (resett == 1){

      EEPROM.write(rom,0);
      EEPROM.write(rom2,0);
      EEPROM.write(rom3,0);
      
      EEPROM.commit();

      Firebase.setInt(firebaseData, "/Populasi/Reset/reset", 0);
    }

    masuk = EEPROM.read(rom);
    keluar = EEPROM.read(rom2);
    total = EEPROM.read(rom3);
    
   
     if(Firebase.getInt(firebaseData, "/Populasi/Time/Timeapp"))
    {
      //succes
      timeapp = firebaseData.intData();
    }else{

      Serial.print("Error in get on data, ");
      Serial.print(firebaseData.errorReason());
    }

  Firebase.setInt(firebaseData, "/Populasi/Timedev/", timeapp);
  
  Firebase.setInt(firebaseData, "/Populasi/Masuk", masuk);
  Firebase.setInt(firebaseData, "/Populasi/Keluar", keluar);
  Firebase.setInt(firebaseData, "/Populasi/Total", total);

  lastPublish = millis();
  }
}

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
