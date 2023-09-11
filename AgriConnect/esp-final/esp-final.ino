#include <WiFi.h>
#include <ThingSpeak.h>
// #include <string>
// using namespace std;
#define WIFI_SSID "abcdef"
#define WIFI_PASSWORD "12345678" 

WiFiClient client;
unsigned long myChannelNumber = 2099426;
const char* myWriteAPIKey =  "Z28MPS63QY2MA31A"; 


// =============================================
// UART CODE
#define RXp2 16
#define TXp2 17

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600,SERIAL_8N1,RXp2,TXp2);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  ThingSpeak.begin(client);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("WiFi connected");
}

void loop() {
  float humidity = 60.38;
  float temperature = 30.25;
  float latitude = 15.458490;
  float longitude = 75.007469;
  float heatndex = 75.00;
  float altitude =0.00;
  int day,month,year,wfsensor;
  // String wfsensor;
  delay(1000);
  Serial.println("Message Recieved: ");
  Serial.println("ESP STARTED READING");
  String stri = Serial2.readString();
  delay(2000);
  Serial.print(stri);
  delay(1000);
  Serial.println("ESP STOPed READING");
  // String temp,humidity,heatindex,altitude,latitude,longitude;
  int k=0,c=0;char lat[6],longt[6],alt[6],dd[3],mm[3],yyyy[5],water[4],humi[4],tempr[5],heatIndex[5],f=0;
  while(1){
    if(stri[k]=='e'&&stri[k+1]==':'&&c==0){
      lat[0]=stri[k+2];
      lat[1]=stri[k+3];
      lat[2]=stri[k+4];
      lat[3]=stri[k+5];
      lat[4]=stri[k+6];
      c++;k++;continue;
    }
    if(stri[k]=='e'&&stri[k+1]==':'&&c==1){
      longt[0]=stri[k+2];
      longt[1]=stri[k+3];
      longt[2]=stri[k+4];
      longt[3]=stri[k+5];
      longt[4]=stri[k+6];
      c++;k++;continue;
    }
    if(stri[k]=='e'&&stri[k+1]==':'&&c==2){
      alt[0]=stri[k+2];
      alt[1]=stri[k+3];
      alt[2]=stri[k+4];
      alt[3]=stri[k+5];
      alt[4]=stri[k+6];
      c++;k++;continue;
    }
    if(stri[k]=='d'&&stri[k+1]==':'){
      dd[0]=stri[k+2];
      dd[1]=stri[k+3];
    }
    if(stri[k]=='m'&&stri[k+1]==':'){
      mm[0]=stri[k+2];
      // mm[1]=stri[k+3];
    }
    if(stri[k]=='y'&&stri[k+1]==':'){
      yyyy[0]=stri[k+2];
      yyyy[1]=stri[k+3];
      yyyy[2]=stri[k+4];
      yyyy[3]=stri[k+5];
      // mm[1]=stri[k+3];
    }
    if(stri[k]=='l'&&stri[k+1]==':'){
      water[0]=stri[k+2];
      water[1]=stri[k+3];
      water[2]=stri[k+4];

      // mm[1]=stri[k+3];
    }
    if(stri[k]=='H'&&stri[k+1]==':'){
      humi[0]=stri[k+2];
      humi[1]=stri[k+3];
      humi[2]=stri[k+4];

      // mm[1]=stri[k+3];
    }
    if(stri[k]=='T'&&stri[k+1]==':'){
      tempr[0]=stri[k+2];
      tempr[1]=stri[k+3];
      tempr[2]=stri[k+4];

      // mm[1]=stri[k+3];
    }
    if(stri[k]=='I'&&stri[k+1]==':'){
      heatIndex[0]=stri[k+2];
      heatIndex[1]=stri[k+3];
      heatIndex[2]=stri[k+4];
      break;
      // mm[1]=stri[k+3];
    }
    if(stri[k]=='E')
    break;
    k++;
  }
  latitude=atof(&lat[0]);
  int dat,mon,yr;
  float  longi,alti,wati,tempi,heati,humidi;
  longitude=atof(&longt[0]);
  altitude=atof(&alt[0]);
  temperature=atof(&tempr[0]);
  heatndex=atof(&heatIndex[0]);
  humidity=atof(&humi[0]);
  wati=atoi(&water[0]);
  dat=atoi(&dd[0]);
  mon=atoi(&mm[0]);
  yr=atoi(&yyyy[0]);
  Serial.print("hello");
  Serial.print(heatndex);
    
//  int water_value =  water.toInt();
//  float lati=lat.toFloat();
//  float longi=longt.toFloat();
//  float alti=alt.toFloat();
//  int ddd=dd.toInt();
//   int mmm=mm.toInt();
//   int yy=yyyy.toInt();

  delay(2000);

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor");
    return;
  }
  longitude = 75.007469;
  latitude = 15.458490;

  Serial.print("Temperature:");
  Serial.print(temperature);
  Serial.print(" °C, Humidity:");
  Serial.print(humidity);
  Serial.println(" %");
ThingSpeak.setField(1,temperature);
ThingSpeak.setField(2,humidity);
ThingSpeak.setField(3,latitude);
ThingSpeak.setField(4,longitude);
ThingSpeak.setField(5,heatndex);


ThingSpeak.writeFields(myChannelNumber,myWriteAPIKey);

  delay(15000); 
}