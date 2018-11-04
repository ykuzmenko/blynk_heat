#define BLYNK_PRINT Serial

#include <SPI.h>
//#include <Ethernet.h>
//#include <BlynkSimpleEthernet.h>
#include "blynk_secrets.h"

#define MACADDRESS 0x00,0x01,0x02,0x03,0x04,0x05
//#define MYIPADDR 192,168,0,6
//#define MYIPMASK 255,255,255,0
//#define MYDNS 192,168,0,1
//#define MYGW 192,168,0,1
//#define UARTBAUD 115200



#include <UIPEthernet.h>
#include <BlynkSimpleUIPEthernet.h>

//#define W5100_CS  10
//#define SDCARD_CS 4


#define DEBUG true

#include "my_motor.h"
#include "timer.h"

BlynkTimer timer;
MyTimer my_timer;

const int timerCount = 2;
MyTimer* my_timers = new MyTimer[timerCount];

WidgetTerminal terminal(V10);


// MyMotor my_motor(7, 6, 5, 9, 8);
// MyMotor(int open_sensor_pin, int closed_sensor_pin, int enable_pin, int input1_pin, int input2_pin ) {
MyMotor my_motor(PB3, PB4, PB5, PB6, PB7);
// MyMotor my_motor(PB8, PB9, PB5, PB6, PB7);

void setup () {

  Serial.begin(9600);

  timer.setInterval(300L, motor_process);

  Blynk.begin(auth, blynk_server, 8442);

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
}

void motor_process() {
  my_motor.processMotion();
}

void check_my_timers() {
//   log_to_terminal("check timers");

   for (int i = 0; i < timerCount; i++) {
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

