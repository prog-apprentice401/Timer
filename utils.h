#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "timer.h"

#ifndef __UTILS__H__
	#define __UTILS_H__
	void showFields (LiquidCrystal_I2C);
	void hideField (uint8_t, LiquidCrystal_I2C);

	void displayTime (Time, LiquidCrystal_I2C);
	void resetInterruptTimer1 (void);
#endif
