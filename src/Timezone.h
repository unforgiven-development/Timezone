/**
 * \file Timezone.h
 * Provides definitions for the Timezone library, including the class implementation, the various functions for the use
 * of timezone rules, support for DST/summer-time, as well as the ability to store timezone and time-change rules in
 * \b EEPROM (provided that the device supports the "standard" Arduino \b EEPROM library).
 *
 * \brief Provides definitions for the Timezone library
 *
 * \author		Gerad Munsch <gmunsch@unforgivendevelopment.com>
 * \author		Jack Christensen
 * \date		2012-2017
 * \copyright	This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. To view a
 *				copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send a letter to:
 *					Creative Commons
 *					171 Second Street
 *					Suite 300
 *					San Francisco, CA 94105
 *					United States of America
 *
 */


/**
 * \page deps Dependencies
 * A description of the resources needed to utilize this library.
 *
 * \tableofcontents
 *
 * \section libdeps Libraries
 * For use of this library
 */



#ifndef _TIMEZONE_H__
#define _TIMEZONE_H__


/**
 * Check if we're running on \b AVR -- as it has \b EEPROM functionality available to store board-specific data, and the
 * \c avr/eeprom.h file must be \c #included
 */
#ifdef __AVR__
#include <avr/eeprom.h>
#define PLATFORM_SUPPORTS_EEPROM 1
#else
#define PLATFORM_SUPPORTS_EEPROM 0
#endif


#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <TimeLib.h>		/* <http://www.arduino.cc/playground/Code/Time> */






/**
 * An enumeration which provides helpful (ie: "human-readable") values for the
 * "week-of-month" definitions for use with DST rules.
 *
 * \brief Provides "week-of-month" values
 */
enum week_t {
	Last,		/*!< The last week of the month */
	First,		/*!< The first week of the month */
	Second,		/*!< The second week of the month */
	Third,		/*!< The third week of the month */
	Fourth		/*!< The fourth week of the month */
};


/**
 * An enumeration to provide "friendly" values for the days of the week.
 *
 * \brief Provides values for the days of the week.
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
 * An enumeration to provide "friendly" values for the months of the year.
 *
 * \brief Provides values for the months of the year.
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
 * Provides a data structure to hold the rules describing when daylight savings time (or "summer time") begins, as well
 * as when it ends (ie: "standard time").
 *
 * \brief Holds the necessary info to define a time change rule
 */
struct TimeChangeRule {
	char    abbrev[6];	/*!< Abbreviation for the rule \note five chars max */
	uint8_t week;		/*!< WEEK of the MONTH: First, Second, Third, Fourth, or Last (week of the month) */
	uint8_t dow;		/*!< DAY of WEEK: 1 = Sun, 2 = Mon, (...), 7 = Sat */
	uint8_t month;		/*!< MONTH: 1 = Jan, 2 = Feb, (...), 12 = Dec */
	uint8_t hour;		/*!< HOUR: 0 - 23 */
	int     offset;		/*!< offset from UTC in minutes */
};


/**
 * \class Timezone
 * Provides an easy method to manage timezones, as well as daylight savings time (or "summer time") on Arduino devices.
 * The "standard" Arduino time libraries are rather basic, and do not provide the very convenient, and useful featureset
 * of timezone and DST management. With many new devices implementing an RTC in hardware, and many "legacy" projects are
 * implementing RTC functionality with serial-based RTC devices (I²C and/or SPI), such as the DS1307, DS3231, etc..
 *
 * \brief Provides timezone and DST/summer-time support for the Arduino TimeLib
 */
class Timezone {
public:
	/**
	 * \fn Timezone(TimeChangeRule dstStart, TimeChangeRule stdStart)
	 * Constructor for a Timezone instance, given the rules which define the start and end of daylight savings time.
	 *
	 * \brief Constructor for Timezone instance
	 *
	 * \param[in]	dstStart	The TimeChangeRule (struct) defining the start of daylight savings time (DST)
	 * \param[in]	stdStart	The TimeChangeRule (struct) defining the start of standard time (ie: the end of DST)
	 */
	Timezone(TimeChangeRule dstStart, TimeChangeRule stdStart);

#if (PLATFORM_SUPPORTS_EEPROM == 1)
	/**
	 * \overload Timezone(int address)
	 * Create a Timezone object from the time change rules stored in EEPROM at the given address. This only works on
	 * devices/platforms which support the standard EEPROM library (for now, it is limited to AVR devices).
	 *
	 * \brief Create a timezone object using rules stored in EEPROM
	 *
	 * \param[in]	address	EEPROM address where the timezone rules are located
	 *
	 */
	Timezone(uint16_t address);
#endif


	time_t toLocal(time_t utc);


	time_t toLocal(time_t utc, TimeChangeRule **tcr);


	time_t toUTC(time_t local);



	bool utcIsDST(time_t utc);


	bool locIsDST(time_t local);

#if (PLATFORM_SUPPORTS_EEPROM == 1)
	/**
	 * Read the timezone rules (DST and standard time) from EEPROM, starting at the given address.
	 * \note Only valid on devices which contain EEPROM (AVR only, for now)
	 *
	 * \brief Read the timezone rules from EEPROM
	 *
	 * \param[in] address EEPROM address where the timezone rules begin.
	 *
	 */
	void readRules(uint16_t address);

	/**
	 * Write the timezone rules (DST and standard time) to EEPROM, starting at the given address.
	 * \note Only valid on devices which contain EEPROM (AVR only, for now)
	 *
	 * @brief Write the timezone rules to EEPROM
	 *
	 * @param[in] address EEPROM address where the timezone rules are to begin.
	 *
	 */
	void writeRules(uint16_t address);
#endif

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



#endif	/* _TIMEZONE_H__ */
