/*
 * This project builds a timer using an arduino uno or nano.
 * Details on how to assemble the circuit are given in the README
 * residing alongside this source file
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

void setup ()
{
	Serial.end ();

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

	countdownStatus = timer.countdown (lcd);

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
