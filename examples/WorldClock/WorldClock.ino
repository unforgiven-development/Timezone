/*----------------------------------------------------------------------*
 * Timezone library example sketch.                                     *
 * Self-adjusting clock for multiple time zones.                        *
 * Jack Christensen Mar 2012                                            *
 *                                                                      *
 * Sources for DST rule information:                                    *
 * http://www.timeanddate.com/worldclock/                               *
 * http://home.tiscali.nl/~t876506/TZworld.html                         *
 *                                                                      *
 * This work is licensed under the Creative Commons Attribution-        *
 * ShareAlike 3.0 Unported License. To view a copy of this license,     *
 * visit http://creativecommons.org/licenses/by-sa/3.0/ or send a       *
 * letter to Creative Commons, 171 Second Street, Suite 300,            *
 * San Francisco, California, 94105, USA.                               *
 *----------------------------------------------------------------------*/

#include "WorldTimeDefinitions.h"

#include <Time.h>			/* <http://www.arduino.cc/playground/Code/Time> */
#include <Timezone.h>		/* <https://github.com/unforgiven-development/Timezone> */

#define UART_BAUD_RATE 115200

/* undefine "USE_FUDGE_FACTOR" by commenting out the following line */
#define USE_FUDGE_FACTOR 1

/* Change the value below to define how frequently the times are printed. Unit is milliseconds. Default value: 10000 */
#define PRINT_EVERY_MILLISECONDS 10000

TimeChangeRule *tcr;		/* pointer to the time change rule, use to get the TZ abbrev */
time_t utc;

/* -----[ FUNCTION PROTOTYPES ]----- */
time_t compileTime();
void printTime(time_t t, char *tz, char *loc);
void sPrintI00(int val);
void sPrintDigits(int val)


/**
 * Standard Arduino "setup" function
 */
void setup() {
	Serial.begin(UART_BAUD_RATE);
	while(!Serial) {
		delay(50);	/* sit in wait loop until serial connects (on certain devices) */
	}

	setTime(usET.toUTC(compileTime()));
}

/**
 * Standard Arduino "loop" function
 */
void loop() {
	Serial.println();

	utc = now();

	printTime(ausET.toLocal(utc, &tcr), tcr->abbrev, "Sydney");
	printTime(CE.toLocal(utc, &tcr), tcr->abbrev, "Paris");
	printTime(UK.toLocal(utc, &tcr), tcr->abbrev, " London");
	printTime(utc, "UTC", " Universal Coordinated Time");
	printTime(usET.toLocal(utc, &tcr), tcr->abbrev, " New York");
	printTime(usCT.toLocal(utc, &tcr), tcr->abbrev, " Chicago");
	printTime(usMT.toLocal(utc, &tcr), tcr->abbrev, " Denver");
	printTime(usAZ.toLocal(utc, &tcr), tcr->abbrev, " Phoenix");
	printTime(usPT.toLocal(utc, &tcr), tcr->abbrev, " Los Angeles");

	delay(PRINT_EVERY_MILLISECONDS);
}

/**
 * Function to return the compile date and time as a time_t value
 *
 * @brief Generate time based on compilation time.
 *
 * @return The date and time of compilation, as a "time_t" value.
 *
 */
time_t compileTime() {
	/**
	 * Sets a "fudge factor" (in seconds), to allow for the difference between the actual time at which the sketch was
	 * compiled, and the time at which execution of the sketch started.
	 *
	 * Various factors can have an influence on the latency between compilation time and execution time, such as the
	 * time spent uploading the binary, whether the bootloader of the device has a delay before starting execution, etc.
	 *
	 * The default value of 25 (seconds), set below, is just an example. Note that your mileage may vary in finding an
	 * ideal value.
	 *
	 */
	const time_t FUDGE(25);

	char *compDate = __DATE__, *compTime = __TIME__, *months = "JanFebMarAprMayJunJulAugSepOctNovDec";
	char chMon[4], *m;
	int d, y;
	tmElements_t tm;
	time_t t;

	strncpy(chMon, compDate, 3);
	chMon[3] = '\0';
	m = strstr(months, chMon);
	tm.Month = ((m - months) / 3 + 1);

	tm.Day = atoi(compDate + 4);
	tm.Year = atoi(compDate + 7) - 1970;
	tm.Hour = atoi(compTime);
	tm.Minute = atoi(compTime + 3);
	tm.Second = atoi(compTime + 6);

	t = makeTime(tm);

#ifdef USE_FUDGE_FACTOR
	/* returns the compilation time, in addition to the "fudge factor" */
	return t + FUDGE;
#else
	return t;
#endif	/* USE_FUDGE_FACTOR */
}

/**
 * Print the current time, given the timezone specified.
 */
void printTime(time_t t, char *tz, char *loc) {
	sPrintI00(hour(t));
	sPrintDigits(minute(t));
	sPrintDigits(second(t));
	Serial.print(' ');
	Serial.print(dayShortStr(weekday(t)));
	Serial.print(' ');
	sPrintI00(day(t));
	Serial.print(' ');
	Serial.print(monthShortStr(month(t)));
	Serial.print(' ');
	Serial.print(year(t));
	Serial.print(' ');
	Serial.print(tz);
	Serial.print(' ');
	Serial.print(loc);
	Serial.println();
}


//Print an integer in "00" format (with leading zero).
//Input value assumed to be between 0 and 99.
void sPrintI00(int val) {

	/* check that the input value is between 0 and 99 */
	if ((val >= 0) && (val <= 99)) {
		/* if the input value is valid, print the output to the serial console */

		/* if the value specified is less than 10, print a "0" before the value to ensure the result is always 2 digits */
		if (val < 10) {
			Serial.print('0');
		}
		Serial.print(val, DEC);

		return;
	} else {
		/* if the input value is NOT valid, print an error to the serial console */

		Serial.println(F("\n\n\tERROR:"));
		Serial.println(F("\t\tIN FUNCTION: void sPrintI00(int val)"));
		Serial.print(F("\t\tPARAMETER(int val): "));
		Serial.println(val, DEC);
		Serial.println(F("\t\tERROR DESCRIPTION: parameter \"int val\" is out of bounds.\n\n"));

		return;
	}
}

//Print an integer in ":00" format (with leading zero).
//Input value assumed to be between 0 and 99.
void sPrintDigits(int val) {

	/* check that the input value is between 0 and 99 */
	if ((val >= 0) && (val <= 99)) {
		/* if the input value is valid, print the output to the serial console */

		Serial.print(':');
		/* if the value specified is less than 10, print a "0" before the value to ensure the result is always 2 digits */
		if (val < 10) {
			Serial.print('0');
		}
		Serial.print(val, DEC);

		return;
	} else {
		/* if the input value is NOT valid, print an error to the serial console */

		Serial.println(F("\n\n\tERROR:"));
		Serial.println(F("\t\tIN FUNCTION: void sPrintDigits(int val)"));
		Serial.print(F("\t\tPARAMETER(int val): "));
		Serial.println(val, DEC);
		Serial.println(F("\t\tERROR DESCRIPTION: parameter \"int val\" is out of bounds.\n\n"));

		return;
	}
}
