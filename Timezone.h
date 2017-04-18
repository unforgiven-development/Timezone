/* ------------------------------------------------------------------------------------------------------------------ *
 * Timezone.h                                                                                                         *
 * -- Part of the Arduino 'Timezone' library                                                                          *
 *                                                                                                                    *
 * Copyright (c) 2017 Gerad Munsch <gmunsch@unforgivendevelopment.com>                                                *
 * Copyright (c) 2012 Jack Christensen                                                                                *
 *                                                                                                                    *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. To view a copy of    *
 * this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send a letter to:                            *
 *     Creative Commons                                                                                               *
 *     171 Second Street                                                                                              *
 *     Suite 300                                                                                                      *
 *     San Francisco, CA 94105                                                                                        *
 *     United States of America                                                                                       *
 *                                                                                                                    *
 * ------------------------------------------------------------------------------------------------------------------ */

#ifndef __TIMEZONE_H__
#define __TIMEZONE_H__

#if ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#include <TimeLib.h>		/* <http://www.arduino.cc/playground/Code/Time> */


/* convenient constants for dstRules */
enum week_t {
	Last,
	First,
	Second,
	Third,
	Fourth
};


enum dow_t {
	Sun = 1,
	Mon,
	Tue,
	Wed,
	Thu,
	Fri,
	Sat
};


enum month_t {
	Jan = 1,
	Feb,
	Mar,
	Apr,
	May,
	Jun,
	Jul,
	Aug,
	Sep,
	Oct,
	Nov,
	Dec
};



/* structure to describe rules for when daylight/summer time begins, or when standard time begins. */
struct TimeChangeRule {
	char abbrev[6];		/* XXX: five chars max */
	uint8_t week;		/* WEEK of the MONTH: First, Second, Third, Fourth, or Last (week of the month) */
	uint8_t dow;		/* DAY of WEEK: 1 = Sun, 2 = Mon, (...), 7 = Sat */
	uint8_t month;		/* MONTH: 1 = Jan, 2 = Feb, (...), 12 = Dec */
	uint8_t hour;		/* 0 - 23 */
	int offset;			/* offset from UTC in minutes */
};



class Timezone {
public:
	Timezone(TimeChangeRule dstStart, TimeChangeRule stdStart);
#ifdef __AVR__
	Timezone(int address);
#endif	/* __AVR__ */

	time_t toLocal(time_t utc);
	time_t toLocal(time_t utc, TimeChangeRule **tcr);
	time_t toUTC(time_t local);

	bool utcIsDST(time_t utc);
	bool locIsDST(time_t local);

#ifdef __AVR__
	void readRules(int address);
	void writeRules(int address);
#endif	/* __AVR__ */

private:
	void calcTimeChanges(int yr);
	time_t toTime_t(TimeChangeRule r, int yr);

	TimeChangeRule _dst;	/* rule for start of dst or summer time for any year */
	TimeChangeRule _std;	/* rule for start of standard time for any year */

	time_t _dstUTC;			/* DST start for given/current year, given in UTC */
	time_t _stdUTC;			/* STD time start for given/current year, given in UTC */

	time_t _dstLoc;			/* DST start for given/current year, given in local time */
	time_t _stdLoc;			/* STD time start for given/current year, given in local time */
};

#endif	/* __TIMEZONE_H__ */
