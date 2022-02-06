#include "Firebase_Arduino_WiFiNINA.h"
#include <SPI.h>
#include <WiFiNINA.h> //Include this instead of WiFi101.h as needed
#include <WiFiUdp.h>
#include <RTCZero.h>

RTCZero rtc;

#include "arduino_secrets.h"

//#define DATABASE_URL 
//#define DATABASE_SECRET 
//#define WIFI_SSID 
//#define WIFI_PASSWORD 

int measuringTime = 5000;              // The time that must have a consistent reading in order to register a seziure in the system
int mDelayTime = 500;                   // The time between each reading for muscle sensor
bool start = false;                     // Indicates that the first reading above the seizure level has occured
unsigned long startTime;                // Indicates the start time of the first reading above seizure level
bool messageSent = false;               // Indicates that a message has been sent
int resetTime = 10000;                // The amount of time that must elapse before another message is sent out. Reset functionality for the code
int seizureLevel = 500;                 // The reading threshold in order for it to be considered a seizure
int bDelayTime = 150;                    // The time delay for the button readings
unsigned long checkSeizureTimeElapsed;  // The amount of time that has elapsed since the checkSeizure method has been called

int keyIndex = 0;                           // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

const int GMT = 19;

//Define Firebase data object
FirebaseData fbdo;

void setup() {
  // Sets up muscle sensor, panic button, and begins serial output
  pinMode(3, INPUT);
  pinMode(A0, INPUT);
  Serial.begin(115200);

  Serial.print("Connecting to Wi-Fi");
  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED)
  {
    status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print(".");
    delay(100);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  rtc.begin();
  unsigned long epoch;
  delay(10000);
  int numberOfTries = 0, maxTries = 6;
  do {
    epoch = WiFi.getTime();
    numberOfTries++;
  }
  while ((epoch == 0) && (numberOfTries < maxTries));
  if (numberOfTries == maxTries) {
    Serial.print("NTP unreachable!!");
  }
  else {
    Serial.print("Epoch received: ");
    Serial.println(epoch);
    rtc.setEpoch(epoch);
    Serial.println();
  }
  Serial.println(timeAndDate());
  delay(1000);

  //Provide the autntication data
  Firebase.begin(DATABASE_URL, DATABASE_SECRET, WIFI_SSID, WIFI_PASSWORD);
  Firebase.reconnectWiFi(true);
}

void loop() {
  // put your main code here, to run repeatedly:
 panicButton();
 if(millis() - checkSeizureTimeElapsed > mDelayTime){
   checkSeizureTimeElapsed = millis();
   checkSeizure();
 }
 delay(bDelayTime);
}

void panicButton(){
  if(1 - digitalRead(3)){
    sendNotification(2);
  }
}

void checkSeizure(){
  //Checks to see if the criterias have been met for a seizure
  int muscleSensorVal = analogRead(A0);
  int elapsedTime = millis() - startTime;
//  Serial.println(muscleSensorVal);
  if(muscleSensorVal > seizureLevel && messageSent == false){
    if(start == false){
      start = true;
      startTime = millis();
    }
    else if(elapsedTime > measuringTime){
      sendNotification(1);
      messageSent = true;
    }
  }
  else if(start == true){
    start = false;
  }
  if(messageSent == true && elapsedTime > resetTime){
    start = false;
    messageSent = false;
    startTime = millis();
  }
}

bool sendNotification(int condition){
  //Sends notification
  if(condition == 1){
     Serial.println("Muscle Sensor");
     String jsonData = "{\"" + timeAndDate() + "\":\"" + 1 + "\"}";
     Firebase.pushJSON(fbdo, "/", jsonData);
     return true;
  }
  if(condition == 2){
    Serial.println("Panic Button");
    String jsonData = "{\"" + timeAndDate() + "\":\"" + 2 + "\"}";
    Firebase.pushJSON(fbdo, "/", jsonData);
    return true;
  }
}
String print2Digits(int number) {
  String tmp = "";
  if (number < 10) {
    tmp = tmp + "0";
  }
  tmp = tmp + (String)number;
  return tmp;
}
String timeAndDate(){
   return print2Digits(2000 + rtc.getYear()) + "-" + print2Digits(rtc.getMonth()) + "-" + print2Digits((rtc.getDay()-1) + (rtc.getHours()+GMT)/24) + " " + print2Digits((rtc.getHours() + GMT)%24) + ":" + print2Digits(rtc.getMinutes()) + ":" + print2Digits(rtc.getSeconds());
}
