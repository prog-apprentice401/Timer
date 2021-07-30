#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "utils.h"

void showFields (LiquidCrystal_I2C lcd)
{
	lcd.setCursor (0, 1);
	lcd.print (" Hr | Mn | Se");
}

void hideField (uint8_t fieldToHide, LiquidCrystal_I2C lcd)
{
	switch (fieldToHide) {
		case 1:
			lcd.setCursor (11, 1);
			break;
		case 2:
			lcd.setCursor (6, 1);
			break;
		case 3:
			lcd.setCursor (1, 1);
			break;
	}
	lcd.print ("  ");
}

void displayTime (Time time, LiquidCrystal_I2C lcd)
{
	lcd.setCursor (1, 0);	//prints time to lcd
	lcd.print ((time.hours < 10) ? "0" : "");
	lcd.print (time.hours);
	lcd.print (" :");
	lcd.setCursor (6, 0);
	lcd.print ((time.minutes < 10) ? "0" : "");
	lcd.print (time.minutes);
	lcd.print (" :");
	lcd.setCursor (11, 0);
	lcd.print ((time.seconds < 10) ? "0" : "");
	lcd.print (time.seconds);
}

void resetInterruptTimer1 (void)
{
	TCCR1B = 0;
	TIMSK1 = 0;
	TCNT1 = 0;
}
