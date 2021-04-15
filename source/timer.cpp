/*
 * This is the source file for all time-related functions used in the project
 *
 * Created on 22nd March 2021 By Koustubh Srivastava
*/

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "timer.h"

//fucntion: constructor, assigns correct pin numbers to private variables
//accepts : required pin numbers
//returns : void
Timer::Timer (uint8_t startStopPin)
{
	_startStopPin = startStopPin;
	
	pinMode (_startStopPin, INPUT);
	
	time.hours = 0;
	time.minutes = 0;
	time.seconds = 0;
}

//function: prints current time to the LCD
//accepts : Time struct [Defined in timer.h]
//returns : void
void Timer::displayTime (LiquidCrystal_I2C lcd)
{
	lcd.setCursor (1, 0);
	lcd.print ((time.hours < 10) ? "0" : "");
	lcd.print (time.hours);
	lcd.setCursor (4, 0);
	lcd.print ((time.minutes < 10) ? ": 0" : ": ");
	lcd.print (time.minutes);
	lcd.setCursor (9, 0);
	lcd.print ((time.seconds < 10) ? ": 0" : ": ");
	lcd.print (time.seconds);
}

//function: hides the currentField, counter-part of showFields ()
//accepts : void, uses global variable currentField
//returns : void
void Timer::hideField (uint8_t currentField, LiquidCrystal_I2C lcd)
{
	switch (currentField) {	//set cursor to starting of current field
	case 1 :
		lcd.setCursor (11, 1);
		break;
	case 2 :
		lcd.setCursor (6, 1);
		break;
	case 3 :
		lcd.setCursor (1, 1);
		break;
	default:
		return;
	}
	lcd.print ("  ");	//erase current field
}

//function: shows all field, counter-part to hideField ()
//accepts : void
//returns : void
void Timer::showFields (LiquidCrystal_I2C lcd) {
	lcd.setCursor (0, 1);
	lcd.print (" Hr | Mn | Se");	//print all fields
}

//function: public method, increases tgiven time field by 1 unit
//accepts : field to increase as `Field` enum
//returns : void
void Timer::increase (uint8_t currentField)
{
	switch (currentField) {
		case 1 :
			increaseSeconds ();
			break;
		case 2 :
			increaseMinutes ();
			break;
		case 3 :
			increaseHours ();
			break;
		default :
			break;
	}
}

//function: public method to decrease given time field by 1
//accepts : field to increase as `Field` enum
//returns : void
void Timer::decrease (uint8_t currentField)
{
	switch (currentField) {
		case 1 :
			decreaseSeconds ();
			break;
		case 2 :
			decreaseMinutes ();
			break;
		case 3 :
			decreaseHours ();
			break;
		default :
			return;
	}
}

//function: private method to increase seconds
//accepts : void
//returns : void
void Timer::increaseSeconds (void)
{
	if (time.seconds >= 59) {
		time.seconds = 0;
	} else {
		time.seconds++;
	}
}

//function: private method to increase minutes
//accepts : void
//returns : void
void Timer::increaseMinutes (void)
{
	if (time.minutes >= 59) {
		time.minutes = 0;
	} else {
		time.minutes++;
	}
}

//function: private method to increase hours
//accepts : void
//returns : void
void Timer::increaseHours (void)
{
	if (time.hours >= 99) {
		time.hours = 0;
	} else {
		time.hours++;
	}
}

//function: private method to decrease seconds
//accepts : void
//returns : void
void Timer::decreaseSeconds ()
{
	if (time.seconds <= 0) {
		time.seconds = 59;
	} else {
		time.seconds--;
	}
}

//function: private method to decrease seconds
//accepts : void
//returns : void
void Timer::decreaseMinutes ()
{
	if (time.minutes <= 0) {
		time.minutes = 59;
	} else {
		time.minutes--;
	}
}

//function: private method to decrease hours
//accepts : void
//returns : void
void Timer::decreaseHours ()
{
	if (time.hours <= 0) {
		time.hours = 99;
	} else {
		time.hours--;
	}
}
