
//DHT22 Setup
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

//SD Card Setup
#include <SPI.h>
#include <SD.h>
File sdcard_file;
int CS_pin = 10;

//DS18B20 Temperature sensor
#include <OneWire.h>
#include<DallasTemperature.h>
#define ONE_WIRE_BUS 3
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

//MPL3115A2 Altimeter Setup
#include <Wire.h>
#include <Adafruit_MPL3115A2.h>
Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();
int scale = 3;
boolean micro_is_5V = true;

//LED
int Blue = 7;
int Red = 6;
int Green = 5;

void setup() {
pinMode(23, INPUT); //detects if a switch is turned on to stop the sdcard writing and to save the data
sensors.begin();
pinMode(Blue, OUTPUT);
pinMode(Red, OUTPUT);
pinMode(Green, OUTPUT);
  
digitalWrite(Green, HIGH);


  dht.begin();

  pinMode(CS_pin, OUTPUT);//declaring CS pin as output pin
  if (SD.begin()){
  
    
    digitalWrite(Green, LOW);
    digitalWrite(Green, HIGH);
    digitalWrite(Green, LOW);
    digitalWrite(Green, HIGH);
    digitalWrite(Green, LOW);
    digitalWrite(Green, HIGH);
  } else
  {
    digitalWrite(Green, LOW);
    digitalWrite(Red, HIGH);
    delay(5000);
    return;
  }
  sdcard_file = SD.open("data.csv", FILE_WRITE);
  if (sdcard_file) { //If the file is found
    
    digitalWrite(Red, LOW);
    digitalWrite(Blue, LOW);
    digitalWrite(Green, HIGH);
    
  }
  else {
    
     digitalWrite(Green, LOW);
    digitalWrite(Red, HIGH);
    
    delay(1000);
    digitalWrite(Red, LOW);
    digitalWrite(Blue, HIGH);
    delay(1000);
     digitalWrite(Green, LOW);
    digitalWrite(Red, HIGH);
    
    delay(1000);
    digitalWrite(Red, LOW);
    digitalWrite(Blue, HIGH);
    delay(1000);
    digitalWrite(Red, HIGH);
    digitalWrite(Blue, LOW);
    
  }
  
sdcard_file.print(",");
sdcard_file.print("DS18B20");
sdcard_file.print(",");
sdcard_file.print("DHT22 Temp");
sdcard_file.print(",");
sdcard_file.print("Hum");
sdcard_file.print(",");
sdcard_file.print("H.InX");
sdcard_file.print(",");
sdcard_file.print("Int. Temp");
sdcard_file.print(",");
sdcard_file.print("Pres. Pasc.");
sdcard_file.print(",");
sdcard_file.print("alt.");
sdcard_file.print(",");
sdcard_file.print("rawX");
sdcard_file.print(",");
sdcard_file.print("rawY");
sdcard_file.print(",");
sdcard_file.print("rawZ");
sdcard_file.print(",");
sdcard_file.print("scaledX");
sdcard_file.print(",");
sdcard_file.print("scaledY");
sdcard_file.print(",");
sdcard_file.print("scaledZ");


}

void loop() {
 
 delay(222);
 digitalWrite(Green, LOW);
 digitalWrite(Blue, HIGH);
 
 if(! baro.begin()) {
  
  
 }
float pascals = baro.getPressure();



float altm = baro.getAltitude();



float tempC = baro.getTemperature();



 //Accelerometer
  int rawX = analogRead(A0);
  int rawY = analogRead(A1);
  int rawZ = analogRead(A2);
 
  float scaledX, scaledY, scaledZ; 
  if (micro_is_5V) //scaling data if voltage is 5V (it is)
  {
    scaledX = mapf(rawX, 0, 675, -scale, scale); 
    scaledY = mapf(rawY, 0, 675, -scale, scale);
    scaledZ = mapf(rawZ, 0, 675, -scale, scale);
  }
  else //scaling data if voltage is 3.3V
  {
    scaledX = mapf(rawX, 0, 1023, -scale, scale);
    scaledY = mapf(rawY, 0, 1023, -scale, scale);
    scaledZ = mapf(rawZ, 0, 1023, -scale, scale);
  }
  


//DS18B20 Temp
sensors.requestTemperatures();




 //DS18B20 Humidity Sensor
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early for DHT
  if (isnan(h) || isnan(t) || isnan(f)) {
    
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
  
  sdcard_file.println(sensors.getTempCByIndex(0));
  sdcard_file.print(",");
  sdcard_file.print(t);
   sdcard_file.print(",");
  sdcard_file.print(h);
   sdcard_file.print(",");
  sdcard_file.print(hic);
   sdcard_file.print(",");
  sdcard_file.print(tempC);
   sdcard_file.print(",");
  sdcard_file.print(pascals);
   sdcard_file.print(",");
  sdcard_file.print(altm);
   sdcard_file.print(",");
  sdcard_file.print(rawX);
   sdcard_file.print(",");
  sdcard_file.print(rawY);
   sdcard_file.print(",");
  sdcard_file.print(rawZ);
  sdcard_file.print(",");
  sdcard_file.print(scaledX);
   sdcard_file.print(",");
  sdcard_file.print(scaledY);
  sdcard_file.print(",");
  sdcard_file.print(scaledZ);
  sdcard_file.print(",");
  
delay(223);

 sdcard_file.close();
 
}
float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;

}
