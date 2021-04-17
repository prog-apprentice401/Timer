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

//function: public method, increases given time field by 1 unit
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

//function: public method to countdown the set timer
int Timer::countdown (LiquidCrystal_I2C lcd)
{
	if (time.hours == 0 && time.minutes == 0 && time.seconds == 0) {
		return -1;
	}
	short int countdownStatus = -1;	//0: successfull
	TCCR1A = 0;	//resets all timer 1 control bits

	TCCR1B |= (1 << CS12);	//sets prescaler to 1024
	TCCR1B &= ~(1 << CS11);
	TCCR1B |= (1 << CS10);

	TCNT1 = 0;	//sets timer1 to 0
	TIMSK1 |= (1 << TOIE1);	//enables Timer Overflow Interrupt
	TCNT1 = START_TIMER_ON;

	while (digitalRead (_startStopPin) != LOW) {
		lcd.setCursor (1, 0);	//prints time to lcd
		lcd.print ((time.hours < 10) ? "0" : "");
		lcd.print (time.hours);
		lcd.setCursor (6, 0);
		lcd.print ((time.minutes < 10) ? "0" : "");
		lcd.print (time.minutes);
		lcd.setCursor (11, 0);
		lcd.print ((time.seconds < 10) ? "0" : "");
		lcd.print (time.seconds);

		if (time.hours <= 0 && time.minutes <= 0 && time.seconds <= 0) {
			countdownStatus = 0;
			break;
		}
	}
	TCCR1B = 0;	//sets prescaler to 1024
	TIMSK1 = 0;
	TCNT1 = 0;
	return countdownStatus;
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
