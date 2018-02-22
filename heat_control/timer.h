#include <TimeLib.h>
#include <WidgetRTC.h>

WidgetRTC rtc;

class MyTimer {
    int startHour;
    int startMinute;
    int stopHour;
    int stopMinute;
    byte weekdays = 0x00;

    // weekday(); // Day of the week (1-7), Sunday is day 1

    public:
        MyTimer() {

        }

        void init_from_timeinput(TimeInputParam t) {
          if (t.hasStartTime()) {
            startHour = t.getStartHour();
            startMinute = t.getStartMinute();
          }

          if (t.hasStopTime()) {
            stopHour = t.getStopHour();
            stopMinute = t.getStopMinute();
          }

          for (int day = 1; day < 8; day++) {
            if (t.isWeekdaySelected(day)) {
              bitSet(weekdays, day);
            }
          }
        }

        // void setStartTime(int startHour, int startMinute) {
        //   startHour = startHour;
        //   startMinute = startMinute;
        // }
        
        // void setStopTime(int stopHour, int stopMinute) {
        //   stopHour = stopHour;
        //   stopMinute = stopMinute;
        // }

        bool checkStartTime() {
          return hour() == startHour && minute() == startMinute && bitRead(weekdays, (weekday() - 1) % 7);
        }

        bool checkStopTime() {
          return hour() == stopHour && minute() == stopMinute && bitRead(weekdays, (weekday() - 1) % 7);
        }
};

