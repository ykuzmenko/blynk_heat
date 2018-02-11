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

BlynkTimer timer;

void setup () {
  pinMode(INPUT1_PIN, OUTPUT);
  pinMode(INPUT2_PIN,OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(CLOSED_SENSOR_PIN, INPUT);
  pinMode(OPEN_SENSOR_PIN,INPUT);

  pinMode(SDCARD_CS, OUTPUT);
  digitalWrite(SDCARD_CS, HIGH); // Deselect the SD card

  Serial.begin(9600);

  timer.setInterval(300L, processMotion);

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

int sensorTriggered(int sensorPin) {
  return digitalRead(sensorPin) == HIGH;
}

positionStates getPosition() {
  // Sensors can trigger only when motor is on

  if (moveDir == opening || moveDir == closing) {
    return state_undefined; 
  }

  if (moveDir == stopped) {
    moveClose();
    if (sensorTriggered(CLOSED_SENSOR_PIN)) {
      return closed;
    }
    if (sensorTriggered(OPEN_SENSOR_PIN)) {
      return opened;
    }
    
    moveOpen();
    if (sensorTriggered(OPEN_SENSOR_PIN)) {
      return opened;
    }
    if (sensorTriggered(CLOSED_SENSOR_PIN)) {
      return closed;
    }
    return state_undefined;
  }
}

BLYNK_WRITE(V1) {
  int buttonState = param.asInt();

  if (buttonState == 0) {
    moveClose();
  }

  if (buttonState == 1) {
    moveOpen();
  }  
}

BLYNK_CONNECTED() {
  Blynk.syncVirtual(V1);
}

void processMotion() {
    switch (moveDir) {
      case closing:
        if (sensorTriggered(CLOSED_SENSOR_PIN)) {
          motorDisable();
        }
        break;

      case opening:
        if (sensorTriggered(OPEN_SENSOR_PIN)) {
          motorDisable();          
        }
        break;
     
      case stopped:
        break;
    }  
}


void loop(){
  Blynk.run();
  timer.run();
}

