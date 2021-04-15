/*
 * This project builds a timer using an arduino uno or nano.
 * Details on how to assemble the circuit are given in the README
 * residing alongside this source file
 * 
 * An extra pushbutton can be hooked from ground to the reset pin to hard reset the board.
 *
 * Created on 22nd March 2021 By Koustubh Srivastava
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "timer.h"
#include "chars.h"

#define SPKRpin		6
#define STRTSTPpin	5
#define NEXTpin		2
#define PLUS1pin	3
#define MINUS1pin	4
#define BLINKINTERVAL	500	//time to wait after each blink of field heading

LiquidCrystal_I2C lcd (0x27, 16, 2);

Timer timer (STRTSTPpin);
uint8_t currentField = 1;	//1 : seconds | 2 : minutes | 3: hours
unsigned long timeLastBlinked;

bool isFieldHidden = false;
uint8_t countdownStatus = 0;	//0: successfully timed | non-zero: error, or paused

void alarm (uint8_t);
int countdown (void);

void setup ()
{
	//Serial.end ();
	Serial.begin (9600);
	lcd.init ();
	lcd.clear ();
	lcd.backlight ();

	lcd.createChar (0, clock);
	lcd.createChar (1, bell);

	pinMode (PLUS1pin, INPUT_PULLUP);
	pinMode (MINUS1pin, INPUT_PULLUP);
	pinMode (NEXTpin, INPUT_PULLUP);
	pinMode (STRTSTPpin, INPUT_PULLUP);
	pinMode (SPKRpin, OUTPUT);
}

void loop ()
{
	while (digitalRead (STRTSTPpin) == HIGH) {
		timer.displayTime (lcd);
		if (millis () - timeLastBlinked >= BLINKINTERVAL) {
			if (isFieldHidden) {
				timer.showFields (lcd);
				isFieldHidden = false;
			} else {
				timer.hideField (currentField, lcd);
				isFieldHidden = true;
			}
			timeLastBlinked = millis ();
		}

		if (digitalRead (NEXTpin) == LOW) {
			delay (200);
			currentField = (currentField < 3) ? currentField + 1 : 1;
		} else if (digitalRead (PLUS1pin) == LOW) {
			delay (200);	//to avoid bounces
			timer.increase (currentField);
		} else if (digitalRead (MINUS1pin) == LOW) {
			delay (200);
			timer.decrease (currentField);
		}
	}
	delay (200);
	lcd.setCursor (15, 0);
	lcd.write (0);

	countdownStatus = countdown ();

	lcd.setCursor (15, 0);
	lcd.print (" ");	//erase clock character
	delay (200);

	if (countdownStatus == 0) {
		alarm (SPKRpin);
	}
}

void alarm (uint8_t speakerPin)
{
	for (int i = 0; i < 3; i++) {
		lcd.setCursor (15, 0);
		lcd.write (1);
		digitalWrite (speakerPin, HIGH);
		delay (500);
		lcd.setCursor (15, 0);
		lcd.print (" ");	//erase bell character
		digitalWrite (speakerPin, LOW);
		delay (500);
	}
}


//This functions should ideally be declared as a method
//but has not been to allow for timer interrupts to be accessed
bool isLcdUpdated = false;

int countdown (void)
{
	if (timer.time.hours == 0 && timer.time.minutes == 0 && timer.time.seconds == 0) {
		return -1;
	}
	short int countdownStatus = -1;	//0: successfull
	TCCR1A = 0;

	TCCR1B |= (1 << CS12);	//sets prescaler to 1024
	TCCR1B &= ~(1 << CS11);
	TCCR1B |= (1 << CS10);

	TCNT1 = 0;
	TIMSK1 |= (1 << TOIE1);
	TCNT1 = START_TIMER_ON;

	while (digitalRead (STRTSTPpin) == HIGH) {
		if (!isLcdUpdated) {
			timer.displayTime (lcd);
			isLcdUpdated = true;
		}
		if (timer.time.hours <= 0 && timer.time.minutes <= 0 && timer.time.seconds <= 0) {
			countdownStatus = 0;
			break;
		}
	}
	TCCR1B = 0;	//sets prescaler to 1024
	TIMSK1 = 0;
	TCNT1 = 0;
	return countdownStatus;
}

ISR (TIMER1_OVF_vect)
{
	Serial.print (timer.time.hours);
	Serial.print ("  ");
	Serial.print (timer.time.minutes);
	Serial.print ("  ");
	Serial.println (timer.time.seconds);
	TCNT1 = START_TIMER_ON;

	if (timer.time.seconds == 0) {
		if (timer.time.minutes == 0) {
			if (timer.time.hours == 0) {
				return;
			}
			timer.decrease (3);
			timer.time.minutes = 59;
		}
		timer.decrease (2);
		timer.time.seconds = 59;
	}
	
	timer.decrease (1);
	isLcdUpdated = false;
	return;
}
