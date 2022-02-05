#include <ArduinoBLE.h>
#include <FirebaseArduino.h>
 
// Set these to run example. 
#define FIREBASE_HOST "example.firebaseio.com" 
#define FIREBASE_AUTH "token_or_secret" 
#define WIFI_SSID "SSID" 
#define WIFI_PASSWORD "PASSWORD" 

int measuringTime = 4000;              // The time that must have a consistent reading in order to register a seziure in the system
int mDelayTime = 500;                   // The time between each reading for muscle sensor
bool start = false;                     // Indicates that the first reading above the seizure level has occured
unsigned long startTime;                // Indicates the start time of the first reading above seizure level
bool messageSent = false;               // Indicates that a message has been sent
int resetTime = 30000;                // The amount of time that must elapse before another message is sent out. Reset functionality for the code
int seizureLevel = 400;                 // The reading threshold in order for it to be considered a seizure
int bDelayTime = 150;                    // The time delay for the button readings
unsigned long checkSeizureTimeElapsed;  // The amount of time that has elapsed since the checkSeizure method has been called

void setup() {
  // Sets up muscle sensor, panic button, and begins serial output
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); 
  Serial.print("connecting"); 
  while (WiFi.status() != WL_CONNECTED) { 
    Serial.print("."); 
    delay(500); 
  } 
  Serial.println(WiFi.localIP()); 
   
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
  pinMode(3, INPUT);
  pinMode(A0, INPUT);
  Serial.begin(9600);
}

void loop() { 
  // set value 
  Firebase.setFloat("number", 42.0); 
  // handle error 
  if (Firebase.failed()) { 
      Serial.print("setting /number failed:"); 
      Serial.println(Firebase.error());   
      return; 
  } 
  delay(1000); 
   
  // update value 
  Firebase.setFloat("number", 43.0); 
  // handle error 
  if (Firebase.failed()) { 
      Serial.print("setting /number failed:"); 
      Serial.println(Firebase.error());   
      return; 
  } 
  delay(1000); 
 
  // get value  
  Serial.print("number: "); 
  Serial.println(Firebase.getFloat("number")); 
  delay(1000); 
 
  // remove value 
  Firebase.remove("number"); 
  delay(1000); 
 
  // set string value 
  Firebase.setString("message", "hello world"); 
  // handle error 
  if (Firebase.failed()) { 
      Serial.print("setting /message failed:"); 
      Serial.println(Firebase.error());   
      return; 
  } 
  delay(1000); 
   
  // set bool value 
  Firebase.setBool("truth", false); 
  // handle error 
  if (Firebase.failed()) { 
      Serial.print("setting /truth failed:"); 
      Serial.println(Firebase.error());   
      return; 
  } 
  delay(1000); 
 
  // append a new value to /logs 
  String name = Firebase.pushInt("logs", n++); 
  // handle error 
  if (Firebase.failed()) { 
      Serial.print("pushing /logs failed:"); 
      Serial.println(Firebase.error());   
      return; 
  } 
  Serial.print("pushed: /logs/"); 
  Serial.println(name); 
  delay(1000); 
}
