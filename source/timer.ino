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
#include "utils.h"
#include "timer.h"
#include "chars.h"

#define ENABLE_ALARMpin	7
#define SPKRpin		6
#define STRTSTPpin	4
#define NEXTpin		2
#define PLUS1pin	3
#define MINUS1pin	5
#define BLINKINTERVAL	500	//time to wait after each blink of field heading

LiquidCrystal_I2C lcd (0x27, 16, 2);
Timer timer (STRTSTPpin);

Field currentField = Seconds;
unsigned long timeLastBlinked;
bool isFieldHidden = false;
uint8_t countdownStatus = 0;	//0: successfully timed | non-zero: error, or paused

void alarm (uint8_t);
void toggleFields (bool, uint8_t);

void setup ()
{
	resetInterruptTimer1 ();
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
	pinMode (ENABLE_ALARMpin, INPUT_PULLUP);
}

void loop ()
{
	while (digitalRead (STRTSTPpin) == HIGH) {
		displayTime (timer.time, lcd);
		if (millis () - timeLastBlinked >= BLINKINTERVAL) {
			toggleFields (isFieldHidden, currentField);
			isFieldHidden = !isFieldHidden;
			timeLastBlinked = millis ();
		}

		if (digitalRead (NEXTpin) == LOW) {
			delay (200);	//to avoid bounces
			currentField = (currentField < Hours) ? currentField + 1 : Seconds;
		} else if (digitalRead (PLUS1pin) == LOW) {
			delay (200);	//to avoid bounces
			timer.increase (currentField);
		} else if (digitalRead (MINUS1pin) == LOW) {
			delay (200);	//to avoid bounces
			timer.decrease (currentField);
		}
	}
	delay (200);
	lcd.setCursor (15, 0);
	lcd.write (0);  //print clock character

	showFields (lcd);

	countdownStatus = timer.countdown (lcd);

	lcd.setCursor (15, 0);
	lcd.print (" ");	//erase clock character

	if (countdownStatus == 0 && digitalRead (ENABLE_ALARMpin) == LOW) {
		alarm (SPKRpin);
	}
  delay (200);  //to debounce the PAUSE click
}

void toggleFields (bool isFieldHidden, uint8_t currentField)
{
	if (isFieldHidden) {
		showFields (lcd);
	} else {
		hideField (currentField, lcd);
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
