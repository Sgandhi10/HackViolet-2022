#include <ArduinoBLE.h>

const char* deviceServiceUuid = "19b10000-e8f2-537e-4f6c-d104768a1214";
const char* deviceServiceCharacteristicUuid = "19b10001-e8f2-537e-4f6c-d104768a1214";

int measuringTime = 10000;              // The time that must have a consistent reading in order to register a seziure in the system
int mDelayTime = 500;                   // The time between each reading for muscle sensor
bool start = false;                     // Indicates that the first reading above the seizure level has occured
unsigned long startTime;                // Indicates the start time of the first reading above seizure level
bool messageSent = false;               // Indicates that a message has been sent
int resetTime = 30000;                // The amount of time that must elapse before another message is sent out. Reset functionality for the code
int seizureLevel = 700;                 // The reading threshold in order for it to be considered a seizure
int bDelayTime = 100;                    // The time delay for the button readings
unsigned long checkSeizureTimeElapsed;  // The amount of time that has elapsed since the checkSeizure method has been called

BLEService warningService(deviceServiceUuid); 
BLEByteCharacteristic warningCharacteristic(deviceServiceCharacteristicUuid, BLERead | BLEWrite);

void setup() {
  // Sets up muscle sensor, panic button, and begins serial output
  pinMode(3, INPUT);
  pinMode(A0, INPUT);
  Serial.begin(9600);
  while (!Serial);
  if (!BLE.begin()) {
    Serial.println("- Starting BLE module failed!");
    while (1);
  }

  BLE.setLocalName("Nano 33");
  BLE.setAdvertisedService(warningService);
  warningService.addCharacteristic(warningCharacteristic);
  BLE.addService(warningService);
  warningCharacteristic.writeValue(-1);
  BLE.advertise();

  Serial.println("Nano 33 BLE (Peripheral Device)");
  Serial.println(" ");
}

void loop() {
  // put your main code here, to run repeatedly:
  BLEDevice central = BLE.central();
  Serial.println("- Discovering central device...");
  delay(500);

  if (central) {
    Serial.println("* Connected to central device!");
    Serial.print("* Device MAC address: ");
    Serial.println(central.address());
    Serial.println(" ");

    while (central.connected()) {
//      if (warningCharacteristic.written()) {
//         warning = warningCharacteristic.value();
//         writeGesture(warning);
//       }
         panicButton();
         if(millis() - checkSeizureTimeElapsed > mDelayTime){
           checkSeizure();
         }
         delay(bDelayTime);
    }
    
    Serial.println("* Disconnected to central device!");
  }
  // if the panic button is pressed send a notification
  
  
}

void panicButton(){
  if(1 - digitalRead(3)){
    sendNotification(2);
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
      sendNotification(1);
      messageSent = true;
    }
  }
  else if(start){
    start == false;
  }
  else if(messageSent && millis() - startTime > resetTime){
    messageSent == false;
    Serial.println("reset");
  }
}

bool sendNotification(int condition){
  //Sends notification
  if(condition == 1){
     Serial.println("Muscle Sensor");
     warningCharacteristic.writeValue((byte)condition);
     return true;
  }
  if(condition == 2){
    Serial.println("Panic Button");
    warningCharacteristic.writeValue((byte)condition);
    return true;
  }
}
