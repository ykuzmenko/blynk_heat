#define BLYNK_PRINT Serial

#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>
#include "blynk_secrets.h"

#include "timer.h"

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

int INPUT1_PIN = 9;
int INPUT2_PIN = 8;

int ENABLE_PIN = 5;
int CLOSED_SENSOR_PIN = 6;
int OPEN_SENSOR_PIN = 7;

#include "motor.h"

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

  setSyncInterval(10 * 60);
  timer.setInterval(10000L, checkTimers);
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

BLYNK_WRITE(V2) {
  TimeInputParam t(param);
  updateTimer(t);
}

BLYNK_CONNECTED() {
  Blynk.syncVirtual(V1);
  rtc.begin();
}

void loop(){
  Blynk.run();
  timer.run();
}

