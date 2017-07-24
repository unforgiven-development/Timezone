/*----------------------------------------------------------------------*
 * Timezone library example sketch.                                     *
 * Write TimeChangeRules to EEPROM.                                     *
 * Jack Christensen Mar 2012                                            *
 *                                                                      *
 * This work is licensed under the Creative Commons Attribution-        *
 * ShareAlike 3.0 Unported License. To view a copy of this license,     *
 * visit http://creativecommons.org/licenses/by-sa/3.0/ or send a       *
 * letter to Creative Commons, 171 Second Street, Suite 300,            *
 * San Francisco, California, 94105, USA.                               *
 *----------------------------------------------------------------------*/

#include <Time.h>			/* <http://www.arduino.cc/playground/Code/Time> */
#include <Timezone.h>		/* <https://github.com/JChristensen/Timezone> */

#define EEPROM_RULES_ADDRESS 0x0100

/* US Eastern Time Zone (New York, Detroit) */
TimeChangeRule usEdt = { "EDT", Second, Sun, Mar, 2, -240 };		/* UTC - 4 hours */
TimeChangeRule usEst = { "EST", First, Sun, Nov, 2, -300 };			/* UTC - 5 hours */
Timezone usEastern(usEdt, usEst);

/* define the pin which contains the LED */
const int ledPin = 13;


void setup(void) {
	pinMode(ledPin, OUTPUT);

#ifdef __AVR__
	/* write rules to defined EEPROM address */
	usEastern.writeRules(EEPROM_RULES_ADDRESS);
#else
	/* can't write rules on non-AVR devices, blink an "SOS" loop */
	while (true) {
		/* 3 "short" blinks */
		digitalWrite(ledPin, HIGH);
		delay(250);
		digitalWrite(ledPin, LOW);
		delay(250);
		digitalWrite(ledPin, HIGH);
		delay(250);
		digitalWrite(ledPin, LOW);
		delay(250);
		digitalWrite(ledPin, HIGH);
		delay(250);
		digitalWrite(ledPin, LOW);
		delay(250);

		/* 3 "long" blinks */
		digitalWrite(ledPin, HIGH);
		delay(1000);
		digitalWrite(ledPin, LOW);
		delay(1000);
		digitalWrite(ledPin, HIGH);
		delay(1000);
		digitalWrite(ledPin, LOW);
		delay(1000);
		digitalWrite(ledPin, HIGH);
		delay(1000);
		digitalWrite(ledPin, LOW);
		delay(1000);

		/* 3 "short" blinks */
		digitalWrite(ledPin, HIGH);
		delay(250);
		digitalWrite(ledPin, LOW);
		delay(250);
		digitalWrite(ledPin, HIGH);
		delay(250);
		digitalWrite(ledPin, LOW);
		delay(250);
		digitalWrite(ledPin, HIGH);
		delay(250);
		digitalWrite(ledPin, LOW);
		delay(250);
	}
#endif	/* __AVR__ */
}

void loop(void) {
	/*
	 * as the write operation is performed in the "setup()" function, we have nothing to do in "loop()", so we will just
	 * indicate that the write operation is complete by rapidly blinking the onboard LED
	 */
	digitalWrite(13, HIGH);
	delay(100);
	digitalWrite(13, LOW);
	delay(100);
}
