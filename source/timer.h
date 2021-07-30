/*
 * This is the header file for functions defined in <timer.cpp>
 *
 * Created on 22nd March 2021 By Koustubh Srivastava
*/
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#ifndef __TIMER_H__
	#define __TIMER_H__

	#define START_TIMER_ON	(65535 - 15625)
	struct Time {
		uint8_t hours;
		uint8_t minutes;
		uint8_t seconds;
	};

	typedef enum field {
		Seconds = 1,
		Minutes = 2,
		Hours = 3,
	} Field;

	class Timer {
		public:
			Time time;
			Timer (uint8_t);
			void increase (Field);
			void decrease (Field);
			int countdown (LiquidCrystal_I2C);
		private:
			uint8_t _startStopPin;

			void increaseSeconds (void);
			void increaseMinutes (void);
			void increaseHours (void);

			void decreaseSeconds (void);
			void decreaseMinutes (void);
			void decreaseHours (void);
	};
#endif //__TIMER_H__
