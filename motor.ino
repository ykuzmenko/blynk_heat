#define BLYNK_PRINT Serial


#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>
#include "blynk_secrets.h"

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).


#define W5100_CS  10
#define SDCARD_CS 4


long interval = 6000;
long checkInterval = 300;
long lastCheckMillis = 0;
long prevousMillis = 0;

int openSensorValue = 1;
int closedSensorValue = 1;
int enable = 1;

int firstRun = 1;

enum moveDirections {
  closing,
  opening,
  stopped
};

enum positionStates {
  opened,
  closed,
  state_undefined
};

enum moveDirections moveDir = stopped;
enum positionStates posState;

int INPUT1_PIN = 9;
int INPUT2_PIN = 8;

int ENABLE_PIN = 5;
int CLOSED_SENSOR_PIN = 6;
int OPEN_SENSOR_PIN = 7;

void setup () {
  pinMode(INPUT1_PIN, OUTPUT);
  pinMode(INPUT2_PIN,OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(CLOSED_SENSOR_PIN, INPUT);
  pinMode(OPEN_SENSOR_PIN,INPUT);

  pinMode(SDCARD_CS, OUTPUT);
  digitalWrite(SDCARD_CS, HIGH); // Deselect the SD card

  Serial.begin(9600);

  Blynk.begin(auth, blynk_server, 8442);
}

void motorEnable(){
  digitalWrite(ENABLE_PIN, HIGH);
}

void motorDisable() {
  digitalWrite(ENABLE_PIN, LOW);
  moveDir = stopped;
}

void moveClose() {
  motorEnable();
  moveDir = closing;
  digitalWrite(INPUT2_PIN, HIGH);
  digitalWrite(INPUT1_PIN, LOW); 
}

void moveOpen() {
  motorEnable();
  moveDir = opening;
  digitalWrite(INPUT1_PIN, HIGH); 
  digitalWrite(INPUT2_PIN, LOW);  

}

positionStates getPosition() {
  if (moveDir == opening || moveDir == closing) {
    return state_undefined; 
  }

  if (moveDir == stopped) {
    // Serial.println("Check closed:");
    moveClose();
    closedSensorValue = digitalRead(CLOSED_SENSOR_PIN);
    // Serial.print("closedSensorValue");
    // Serial.println(closedSensorValue);
    openSensorValue = digitalRead(OPEN_SENSOR_PIN);
    // Serial.print("openSensorValue");
    // Serial.println(openSensorValue );
    if (closedSensorValue == HIGH) {
      Serial.println("Closed state sensor fired");
      return closed;
    }
    if (openSensorValue == HIGH) {
      Serial.println("Open state sensor fired");
      return opened;
    }
    
  
    // Serial.println("Check opened:");
    moveOpen();
//    moveClose();
    closedSensorValue = digitalRead(CLOSED_SENSOR_PIN);
    // Serial.print("closedSensorValue");
    // Serial.println(closedSensorValue);
    openSensorValue = digitalRead(OPEN_SENSOR_PIN);
    // Serial.print("openSensorValue");
    // Serial.println(openSensorValue );
    if (openSensorValue == HIGH) {
      Serial.println("Open state sensor fired");
      return opened;
    }
    if (closedSensorValue == HIGH) {
      Serial.println("Closed state sensor fired");
      return closed;
    }
    // Serial.println("Nothing fired");
    return state_undefined;
  }
}

BLYNK_WRITE(V1) {
  int pinData = param.asInt();
  Serial.println("v1 received");

  // if (pinData == 1 && getPosition() == opened) {
  if (pinData == 1) {
    Serial.println("Closing");
    moveClose();
  }

  // if (pinData == 0 && getPosition() == closed) {
  if (pinData == 0) {
    Serial.println("Opening");
    moveOpen();
  }  
}

void loop(){
  unsigned long currentMillis = millis();
  
  Blynk.run();
  
  if (currentMillis - lastCheckMillis > checkInterval) {
    lastCheckMillis = currentMillis;
    Serial.print("Move dir: ");
    Serial.println(moveDir);
    switch (moveDir) {
      case closing:
        closedSensorValue = digitalRead(CLOSED_SENSOR_PIN);
        if (closedSensorValue == HIGH) {
          motorDisable();
        }
        break;

      case opening:
        openSensorValue = digitalRead(OPEN_SENSOR_PIN);
        if (openSensorValue == HIGH) {
          motorDisable();          
        }
        break;
     
      case stopped:
        if (firstRun) {
          firstRun = 0;
          switch(getPosition()){
            case state_undefined :
              // Serial.println("Undefined");
              moveClose();
              break;
            case opened :
              // Serial.println("opened");
              moveClose();
              break;
            case closed :
              // Serial.println("closed");
              moveOpen();
              break;
          }
        break;
      }
    }
  }
}

