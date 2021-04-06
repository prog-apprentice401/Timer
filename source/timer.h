/*
 * This is the header file for functions defined in <timer.cpp>
 *
 * Created on 22nd March 2021 By Koustubh Srivastava
*/
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#ifndef __TIMER_H__
	#define __TIMER_H__

	#define TIMEINTERVAL 1000
	struct Time {
		uint8_t hours;
		uint8_t minutes;
		uint8_t seconds;
	};

	class Timer {
		public:
			Time time;
			Timer (uint8_t);
			void displayTime (LiquidCrystal_I2C lcd);
			void hideField (uint8_t, LiquidCrystal_I2C);
			void showFields (LiquidCrystal_I2C);
			void increase (uint8_t);
			void decrease (uint8_t);
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