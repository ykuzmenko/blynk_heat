#include <TimeLib.h>
#include <WidgetRTC.h>

WidgetRTC rtc;

class MyTimer {
		int startHour;
		int startMinute;
		int stopHour;
		int stopMinute;

    public:
        MyTimer() {

        }

        void init_from_timeinput(TimeInputParam t) {
        	if (t.hasStartTime()) {
        		startHour = t.getStartHour();
        		startMinute = t.getStartMinute();
  				}
        }

        void setStartTime(int startHour, int startMinute) {
        	startHour = startHour;
        	startMinute = startMinute;
        }
        
        void setStopTime(int stopHour, int stopMinute) {
					stopHour = stopHour;
					stopMinute = stopMinute;
        }

        int getStartTimeHour(void) {
        	return(stopHour);
        }

        int getStartTimeMinute(void) {
        	return(stopHour);
        }

        bool checkStartTime() {
        	weekday();
        	return hour() == startHour && minute() == startMinute;
        }

        bool checkStopTime() {
        	weekday();
        	return hour() == stopHour && minute() == stopMinute;
        }
};

