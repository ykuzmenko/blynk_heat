#define BLYNK_PRINT Serial

#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>
#include "blynk_secrets.h"

#define W5100_CS  10
#define SDCARD_CS 4


#define DEBUG true

#include "my_motor.h"
#include "timer.h"

BlynkTimer timer;
MyTimer my_timer;

const int timerCount = 2;
MyTimer* my_timers = new MyTimer[timerCount];

WidgetTerminal terminal(V10);

MyMotor my_motor = MyMotor(7, 6, 5, 9, 8);

void setup () {
  pinMode(SDCARD_CS, OUTPUT);
  digitalWrite(SDCARD_CS, HIGH); // Deselect the SD card

  Serial.begin(9600);

  timer.setInterval(300L, motor_process);

  Blynk.begin(auth, blynk_server, 8442);

  setSyncInterval(10 * 60);
  timer.setInterval(60000L, check_my_timers);
}


BLYNK_WRITE(V1) {
  int buttonState = param.asInt();

  if (buttonState == 0) {
    my_motor.close();
  }

  if (buttonState == 1) {
    my_motor.open();
  }  
}

BLYNK_WRITE(V2) {
  TimeInputParam t(param);

  MyTimer mt = MyTimer();
  mt.init_from_timeinput(t);

  my_timers[0] = mt;
}


BLYNK_WRITE(V3) {
  TimeInputParam t(param);

  MyTimer mt = MyTimer();
  mt.init_from_timeinput(t);

  my_timers[1] = mt;

}

BLYNK_CONNECTED() {
  Blynk.syncVirtual(V1, V2, V3, V10);
  rtc.begin();
}

void motor_process() {
  my_motor.processMotion();
}

void check_my_timers() {
 for (int i = 0; i < 2; i++) {
   if (my_timers[i].checkStartTime()) {
      terminal.print("Opened at: ");
      terminal.print(hour());
      terminal.print(":");
      terminal.print(minute());

      my_motor.open();
   }
   if (my_timers[i].checkStopTime()) {
      terminal.print("Closed at: ");
      terminal.print(hour());
      terminal.print(":");
      terminal.print(minute());

      my_motor.close();
   }
 }
 terminal.flush();
}

void loop(){
  Blynk.run();
  timer.run();
}

