int measuringTime = 10000;              // The time that must have a consistent reading in order to register a seziure in the system
int mDelayTime = 500;                   // The time between each reading for muscle sensor
bool start = false;                     // Indicates that the first reading above the seizure level has occured
unsigned long startTime;                // Indicates the start time of the first reading above seizure level
bool messageSent = false;               // Indicates that a message has been sent
int resetTime = 1000000;                // The amount of time that must elapse before another message is sent out. Reset functionality for the code
int seizureLevel = 700;                 // The reading threshold in order for it to be considered a seizure
int bDelayTime = 100;                    // The time delay for the button readings
unsigned long checkSeizureTimeElapsed;  // The amount of time that has elapsed since the checkSeizure method has been called

void setup() {
  // Sets up muscle sensor, panic button, and begins serial output
  pinMode(3, INPUT);
  pinMode(A0, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  // if the panic button is pressed send a notification
  panicButton();
  if(millis() - checkSeizureTimeElapsed > mDelayTime){
    checkSeizure();
  }
  delay(bDelayTime);
}

void panicButton(){
  if(1 - digitalRead(3)){
    sendNotification(1);
  }
}

void checkSeizure(){
  //Checks to see if the criterias have been met for a seizure
  int muscleSensorVal = analogRead(A0);
  if(muscleSensorVal > seizureLevel){
    if(start == false){
      start = true;
      startTime = millis();
    }
    else if(millis() - startTime > measuringTime && messageSent == false){
      sendNotification(0);
      messageSent = true;
    }
  }
  else if(start){
    start == false;
  }
  else if(messageSent && millis() - startTime > resetTime){
    messageSent == false;
  }
}

bool sendNotification(int condition){
  //Sends notification
  if(condition == 0){
     Serial.println("Muscle Sensor");
     return true;
  }
  if(condition == 1){
    Serial.println("Panic Button");
    return true;
  }
}
