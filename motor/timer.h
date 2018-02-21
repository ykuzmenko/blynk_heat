#include <TimeLib.h>
#include <WidgetRTC.h>

WidgetRTC rtc;

typedef struct
 {
     int startHour;
     int startMinute;
     int stopHour;
     int stopMinute;
     byte weekdays;
 }  timer_struct_type;

timer_struct_type timers[2];


timer_struct_type timers[2];

void checkTimers() {

}

void updateTimer(TimeInputParam time_input) {
	timer_struct_type timer;	
}