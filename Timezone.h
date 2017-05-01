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


/**
 * \enum week_t Provides helpful values for week-of-month definitions for use with DST rules.
 */
enum week_t {
	Last,	/*!< The last week of the month */
	First,	/*!< The first week of the month */
	Second,	/*!< The second week of the month */
	Third,	/*!< The third week of the month */
	Fourth	/*!< The fourth week of the month */
};


/**
 * \enum dow_t Provides values for the days of the week.
 */
enum dow_t {
	Sun = 1,	/*!< Sunday */
	Mon,		/*!< Monday */
	Tue,		/*!< Tuesday */
	Wed,		/*!< Wednesday */
	Thu,		/*!< Thursday */
	Fri,		/*!< Friday */
	Sat			/*!< Saturday */
};


/**
 * \enum month_t Provides values for the months of the year.
 */
enum month_t {
	Jan = 1,	/*!< January */
	Feb,		/*!< February */
	Mar,		/*!< March */
	Apr,		/*!< April */
	May,		/*!< May */
	Jun,		/*!< June */
	Jul,		/*!< July */
	Aug,		/*!< August */
	Sep,		/*!< September */
	Oct,		/*!< October */
	Nov,		/*!< November */
	Dec			/*!< December */
};


/**
 * \struct TimeChangeRule A structure to hold the rules describing when daylight savings time (or "summer time") begins,
 *                        as well as when it ends.
 */
struct TimeChangeRule {
	char abbrev[6];		/* XXX: five chars max */
	uint8_t week;		/* WEEK of the MONTH: First, Second, Third, Fourth, or Last (week of the month) */
	uint8_t dow;		/* DAY of WEEK: 1 = Sun, 2 = Mon, (...), 7 = Sat */
	uint8_t month;		/* MONTH: 1 = Jan, 2 = Feb, (...), 12 = Dec */
	uint8_t hour;		/* 0 - 23 */
	int offset;			/* offset from UTC in minutes */
};


/**
 * \class Timezone Provides an easy method to manage timezones, as well as daylight savings time (or "summer time") on
 *                 Arduino devices. The "standard" Arduino time libraries are rather basic, and do not provide the very
 *                 convenient, and useful featureset of timezone and DST management. With many new devices implementing
 *                 an RTC in hardware, and many "legacy" projects are implementing RTC functionality with serial-based
 *                 RTC devices (I²C and/or SPI), such as the DS1307, DS3231, etc..
 */
class Timezone {
public:

	/**
	 * Constructor for a Timezone instance, given the rules which define the start and end of daylight savings time.
	 *
	 * \brief Constructor for Timezone instance
	 *
	 * \param [in] dstStart	The TimeChangeRule (data structure) defining the start of daylight savings time (DST)
	 * \param [in] stdStart	The TimeChangeRule (data structure) defining the start of standard time (ie: the end of DST)
	 */
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
