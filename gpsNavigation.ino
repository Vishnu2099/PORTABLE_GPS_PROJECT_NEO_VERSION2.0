

#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include "DHT.h"
#define DHTPIN 11

#define DHTTYPE DHT11


static const int RXPin = 7, TXPin = 6;
static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps;

SoftwareSerial ss(RXPin, TXPin);

int pPin = A5;
int pValue;


DHT dht(DHTPIN, DHTTYPE);


void setup() {
  Serial.begin(9600);
  ss.begin(GPSBaud);

  pinMode(pPin,INPUT);

  dht.begin();
}

void loop() {
  pValue = analogRead(pPin);
  if (pValue >= 0 && pValue <= 150) {
    displayLoc();
  }
  if (pValue >= 151 && pValue <= 250) {
    displayTime();
  }
  if (pValue >= 251 && pValue <= 350) {
    displaySpeedal();
    }
  if (pValue >= 351 && pValue <= 450) {
        displaySateliteinfo();
    }
  if (pValue >= 451 && pValue <= 650) {
        displayTH();
    }
  if (pValue >=651 && pValue <= 700){
      Navigation();
  }

}

void displayLoc() { 
    while (ss.available() > 0) {
    gps.encode(ss.read());
  }
  if (gps.location.isValid()) {
      Serial.print("AGE:");
      Serial.print(gps.location.age());
      Serial.print("ms");
      Serial.println();
      Serial.print("LAT:");
      Serial.print(gps.location.lat(), 6);
      Serial.println(" ");
      Serial.print("LON:");
      Serial.print(gps.location.lng(), 6);
      Serial.println(" ");
      delay(500);
  } else {
      Serial.print(F("LOCATION:NOT SYNCHRONISED"));
      Serial.println();
      delay(2000);
    }
} 
void displayTime(){
      while (ss.available() > 0) {
    gps.encode(ss.read());
  }
      Serial.print("AGE:");
      Serial.print(gps.time.age());
      Serial.print("ms");
      Serial.println();
      Serial.print("TIME:");
      if (gps.time.hour() < 10) Serial.print(F("0"));
      Serial.print(gps.time.hour());
      Serial.print(F(":"));
      if (gps.time.minute() < 10) Serial.print(F("0"));
      Serial.print(gps.time.minute());
      Serial.print(F(":"));
      if (gps.time.second() < 10) Serial.print(F("0"));
      Serial.print(gps.time.second());
      Serial.print(F(":"));
      if (gps.time.centisecond() < 10) Serial.print(F("0"));
      Serial.print(gps.time.centisecond());
      Serial.println(" ");
      Serial.print(F("Date:"));
      Serial.print(gps.date.month());
      Serial.print(F("/"));
      Serial.print(gps.date.day());
      Serial.print(F("/"));
      Serial.print(gps.date.year());
      Serial.println(" ");
      delay(2000);
}
void displayTH() {
  delay(2000);
  

 
  float h = dht.readHumidity();
  
  float t = dht.readTemperature();

  float f = dht.readTemperature(true);

  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.print("NO DATA");
    Serial.println(" ");
    delay(500);
    return;
  }
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print("%");
  Serial.println(" ");
  Serial.print(F("Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.println(" ");
  Serial.print(F("Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C |"));
  Serial.print(hif);
  Serial.println(F("°F"));
}

void displaySpeedal(){

    Serial.print(F("ms SPEED:="));
    Serial.print(gps.speed.value());
    Serial.print(F(" Knots="));
    Serial.print(gps.speed.knots());
    Serial.print(F(" MPH="));
    Serial.print(gps.speed.mph());
    Serial.print(F(" m/s="));
    Serial.print(gps.speed.mps());
    Serial.print(F(" km/h="));
    Serial.println(gps.speed.kmph());
    Serial.print(F("ms ALTIT:="));
    Serial.print(gps.altitude.value());
    Serial.print(F(" Meters="));
    Serial.print(gps.altitude.meters());
    Serial.print(F(" Miles="));
    Serial.print(gps.altitude.miles());
    Serial.print(F(" KM="));
    Serial.print(gps.altitude.kilometers());
    Serial.print(F(" Feet="));
    Serial.println(gps.altitude.feet());
    delay(2000);
  
}

void displaySateliteinfo(){
    Serial.print(F("SAT CONNECTED:="));
    Serial.print(gps.satellites.value());
    Serial.println();
    Serial.print(F("Precision(hdop)="));
    Serial.print(gps.hdop.hdop());
    Serial.println();
    Serial.print(F("Age="));
    Serial.print(gps.hdop.age());
    Serial.println();
    delay(1000);
    
}
void Navigation(){
  double lats[]{9.983812927697816,10.51607095624761};
  double lon[]{76.29919052234263,76.21360940196047};
  String areas[]{"Ernakulam","Thrissur"};
  
  Serial.println("------------------------------------------------------------");   
  Serial.println("WELCOME TO NAVIGATION BETA");
  Serial.println("NAVIGATION LOCK ENABLED TO PREVENT ACCIDENTAL SWITCHING");
  Serial.println("PRESET DESTINATIONS-->");
  for(String i:areas){
    Serial.println(i);
  }

  Serial.println("------------------------------------------------------------"); 

  Serial.println("Enter your choice");
  Serial.println("type custom for custom search");
  Serial.println("press c to unlock nav mode");

  Serial.println("------------------------------------------------------------"); 

  while (Serial.available() == 0) {}
  String choice = Serial.readString();
  choice.trim();

  if(choice=="c"){
    Serial.println("Unlocked you can change to other");
    delay(3000);
    Serial.println("------------------------------------------------------------"); 
  }

  else{
  int count{0};
  String check;
  for(auto i:areas){
    check=i;
    if(i==choice){/*index finding*/
      break;
    }
    else{
      count+=1;
    }
  }
  while(true){
    if(check!= choice){
    Serial.println("---INVALID CHOICE---");
    break;
    }
    else{
      double distance =
        TinyGPSPlus::distanceBetween(
            gps.location.lat(),
            gps.location.lng(),
            lats[count], 
            lon[count]);
        double courseto =
        TinyGPSPlus::courseTo(
            gps.location.lat(),
            gps.location.lng(),
            lats[count], 
            lon[count]);
      Serial.println("------------------------------------------------------------");     
      Serial.print(F("Distance="));
      Serial.print(distance/1000, 6);
      Serial.print(F(" km Course-to="));
      Serial.print(courseto, 6);
      Serial.print(F(" degrees ["));
      Serial.print(TinyGPSPlus::cardinal(courseto));
      Serial.println(F("]"));
      Serial.println("------------------------------------------------------------");   
      Serial.println("press s to stop");
      String cancel = Serial.readString();
      cancel.trim();
      if(cancel=="s"){
        break;
      }
      else{
      }
      delay(2000);
      
    }
    
  }
    
  }

  

}
