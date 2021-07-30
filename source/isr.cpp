#include <Arduino.h>
#include "Timer.h"
extern Timer timer;

ISR (TIMER1_OVF_vect)
{
	TCNT1 = START_TIMER_ON;

	if (timer.time.seconds == 0) {
		if (timer.time.minutes == 0) {
			if (timer.time.hours == 0) {
				return;
			}
			timer.decrease (3);
			timer.time.minutes = 59;
		}
		timer.decrease (2);	//2: second field, minutes
		timer.time.seconds = 59;
	}
	timer.decrease (1);
	return;
}
