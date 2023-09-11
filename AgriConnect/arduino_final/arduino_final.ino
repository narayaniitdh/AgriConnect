#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <math.h>
// =================GPS==============2,3=========
static const int RXPin = 2, TXPin = 3;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);
double gpsLatLonAlt[3]={0.000000,0.000000,0.000000}; // Latitude Longitude Altitude
int date[3]= {0,0,0}; // dd mm yyyy
// =================DHTSen=============6=========
#define DHTPIN 6
#define DHTTYPE DHT11
DHT dht = DHT(DHTPIN,DHTTYPE);
float temp_hum_heatindex[3]={0.00,0.00,0.00};
// ==============================================
// Water/RainFall Sensor=========================
const int waterpin = 12;
// =================================================

// All function Declarations=====================
void chillGPSisHere(double item[],int date[]);
void temp_humidity(float arr[]);
int waterfall();
// ==============================================

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
dht.begin();
ss.begin(GPSBaud);
pinMode(waterpin,OUTPUT);
}

void loop() {
  Serial.println("START");
  // ================Temperature-Humidity Heat Index========================
  temp_humidity(temp_hum_heatindex);
  Serial.print("H:");
  Serial.print(temp_hum_heatindex[1]);
  Serial.print(" % ");
  Serial.print("T:");
  Serial.print(temp_hum_heatindex[0]);
  Serial.print(" HI:");
  Serial.print(temp_hum_heatindex[2]);
  Serial.print(" \xC2\xB0");
  Serial.print("C | ");
  // ===========================================
  // ========================================================
  while(gpsLatLonAlt[0]==0.000000 && gpsLatLonAlt[1]==0.000000 && gpsLatLonAlt[2]==0.000000){
  chillGPSisHere(gpsLatLonAlt,date);
  }
  Serial.print(" Latitude:");
  Serial.print(gpsLatLonAlt[0]/1000000.00);
  Serial.print(" Longitude:");
  Serial.print(gpsLatLonAlt[1]/1000000.00);
  Serial.print(" Altitude:");
  Serial.println(gpsLatLonAlt[2]/1000000.00);
  Serial.print("dd:");
  Serial.print(date[0]);
  Serial.print(" mm:");

  Serial.print(date[1]);
  Serial.print(" yyyy:");

  Serial.println(date[2]);
  Serial.print(" ");
  // =========================================================
  if (waterfall(waterpin)==1){
    Serial.print(":Raining ");
  }else{
    Serial.print(":Dry ");
  }
// ===============================================================
Serial.println("ENDED");
  gpsLatLonAlt[0]=0.00;gpsLatLonAlt[1]=0.00;gpsLatLonAlt[2]=0.00;
}


void temp_humidity(float arr[]){
    delay(2000);
  float h = dht.readHumidity();
  // Read the temperature as Celsius:
  float t = dht.readTemperature();
  // Read the temperature as Fahrenheit:
  float f = dht.readTemperature(true);
arr[0] = t;arr[1]=h;arr[2]=f;
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.print(":Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (default):
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius:
  float hic = dht.computeHeatIndex(t, h, false);


  // Serial.print("Humidity: ");
  // Serial.print(h);
  // Serial.print(" % ");
  // Serial.print("Temperature: ");
  // Serial.print(t);
  // Serial.print(" \xC2\xB0");
  // Serial.print("C | ");
  // Serial.print(f);
  // Serial.print(" \xC2\xB0");
  // Serial.print("F ");
  // Serial.print("Heat index: ");
  // Serial.print(hic);
  // Serial.print(" \xC2\xB0");
  // Serial.print("C | ");
  // Serial.print(hif);
  // Serial.print(" \xC2\xB0");
  // Serial.println("F");
  return;
}


void chillGPSisHere(double item[], int date[]){
  // delay(1000);
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0){
    gps.encode(ss.read());
    if (gps.location.isUpdated()){
      double latitu=((double)gps.location.lat())*1000000.00   ;
      double longitu=((double)gps.location.lng())*1000000.00    ;
      double altitu=((double)gps.altitude.meters())*1000000.00    ;
      // latitu = roundf(100000*latitu)/100000;
      // longitu = roundf(100000*longitu)/100000;
      // altitu = roundf(1000*altitu)/1000;
      // Serial.println(latitu);
      item[0]=latitu;item[1]=longitu;item[2]=altitu;
      // Latitude in degrees (double)
      // Serial.print("Latitude= "); 
      // Serial.print(gps.location.lat(), 6);  
      
      // // Longitude in degrees (double)
      // Serial.print(" Longitude= "); 
      // Serial.println(gps.location.lng(), 6); 

      // Raw date in DDMMYY format (u32)
      // Serial.print("Raw date DDMMYY = ");
      // Serial.println(gps.date.value()); 

      // Year (2000+) (u16)
      // Serial.print("Year = "); 
      date[2] = (int)gps.date.year(); 
      // Month (1-12) (u8)
      // Serial.print("Month = "); 
      date[1] = (int)gps.date.month(); 
      // Day (1-31) (u8)
      // Serial.print("Day = "); 
      date[0] = (int)gps.date.day(); 

      // Raw time in HHMMSSCC format (u32)
      // Serial.print("Raw time in HHMMSSCC = "); 
      // Serial.println(gps.time.value()); 

      // Hour (0-23) (u8)
      // Serial.print("Hour = "); 
      // Serial.println(gps.time.hour()); 
      // Minute (0-59) (u8)
      // Serial.print("Minute = "); 
      // Serial.println(gps.time.minute()); 
      // Second (0-59) (u8)
      // Serial.print("Second = "); 
      // Serial.println(gps.time.second()); 
      // 100ths of a second (0-99) (u8)
      // Serial.print("Centisecond = "); 
      // Serial.println(gps.time.centisecond()); 


      Serial.print("Altitude in meters:"); 
      Serial.print(gps.altitude.meters()); 
      Serial.print(" ");


      // // Number of satellites in use (u32)
      // Serial.print("Number os satellites in use = "); 
      // Serial.println(gps.satellites.value()); 

      // // Horizontal Dim. of Precision (100ths-i32)
      // Serial.print("HDOP = "); 
      // Serial.println(gps.hdop.value()); 
      // delay(1000);
  }
  }
}


int waterfall(int waterpin){
  int sensorValue = analogRead(A0);
  Serial.print("waterfall:");
  Serial.print(sensorValue);  
  Serial.print(" ");
    if (sensorValue<900)
    {
      // Serial.println("it is raining");
      digitalWrite(waterpin,HIGH);
      return 1;
    }
    else
    {
       digitalWrite(waterpin,LOW);
       return 0;
    }
    delay(1000);        // delay in between reads for stability
  }