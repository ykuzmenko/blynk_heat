#include <string>
#include <iostream>

typedef struct
 {
     int startHour;
     int startMinute;
     int stopHour;
     int stopMinute;
     uint8_t weekdays = 0x00;
 }  timer_struct_type;

timer_struct_type timers[2];

void checkTimers() {

}

void updateTimer() {
	timer_struct_type timer;	
}

int main(void) {
	timer_struct_type timer;

	timer.startHour = 12;	
	timer.startMinute = 30;	

	timer.stopHour = 13;	
	timer.stopHour = 30;

	timers[0] = timer;

	printf("Start hour: %d\n", timers[0].startHour);

	if (timers[0].weekdays << 1) {
		printf("Monday is selected");
	}

	return(0);
}