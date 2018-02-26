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

 MyMotor my_motor(7, 6, 5, 9, 8);

// byte arduino_mac[] = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
// IPAddress arduino_ip ( 192,   168,   2,  2);
// IPAddress dns_ip     (  8,   8,   8,   8);
// IPAddress gateway_ip ( 192,   168,   2,   1);
// IPAddress subnet_mask(255, 255, 255,   0);


void setup () {
  pinMode(SDCARD_CS, OUTPUT);
  digitalWrite(SDCARD_CS, HIGH); // Deselect the SD card

  Serial.begin(9600);

  timer.setInterval(300L, motor_process);

  Blynk.begin(auth, blynk_server, 8442);
//  Blynk.begin(auth, blynk_server, 8442, arduino_ip, dns_ip, gateway_ip, subnet_mask, arduino_mac);

  my_motor.setup();

  setSyncInterval(10 * 60);
  timer.setInterval(60000L, check_my_timers);
}


BLYNK_WRITE(V1) {
  int buttonState = param.asInt();

  if (buttonState == 0) {
    log_to_terminal("manually close");
    my_motor.close();
  }

  if (buttonState == 1) {
    log_to_terminal("manually open");
    my_motor.open();
  }  
}

BLYNK_WRITE(V2) {
  TimeInputParam t(param);

  BLYNK_LOG("Timer1 set");

  MyTimer mt = MyTimer();
  mt.init_from_timeinput(t);

  log_to_terminal("Timer1 set");
  terminal.flush();

  my_timers[0] = mt;
}


BLYNK_WRITE(V3) {
  TimeInputParam t(param);

  MyTimer mt = MyTimer();
  mt.init_from_timeinput(t);

  log_to_terminal("Timer2 set");

  my_timers[1] = mt;

}

BLYNK_CONNECTED() {
  Blynk.syncVirtual(V1, V2, V3);
  rtc.begin();
  log_to_terminal("Connected");
  terminal.flush();
}

void motor_process() {
  my_motor.processMotion();
}

void check_my_timers() {
  // log_to_terminal("check timers");

   for (int i = 0; i < 2; i++) {
     if (my_timers[i].checkStartTime()) {
        log_to_terminal("open by Timer");

        Blynk.virtualWrite(1, 1);
        my_motor.open();

     }
     if (my_timers[i].checkStopTime()) {
        log_to_terminal("close by Timer");

        Blynk.virtualWrite(1, 0);
        my_motor.close();
     }
   }
 terminal.flush();
}

void log_to_terminal(String line) {
    terminal.print(hour());
  terminal.print(":");
  terminal.print(minute());
  terminal.print(" ");
  terminal.println(line);
  terminal.flush();
}

void loop(){
  Blynk.run();
  timer.run();
}

