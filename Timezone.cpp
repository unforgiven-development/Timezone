/* ------------------------------------------------------------------------------------------------------------------ *
 * Timezone.cpp                                                                                                       *
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

#ifdef __AVR__
  #include <avr/eeprom.h>
#endif	/* __AVR__ */

#include "Timezone.h"


/**
 * Create a Timezone object from the given time change rules.
 *
 * @brief Create a timezone object using specified rules
 *
 * @param[in] dstStart TimeChangeRule object defining the start of DST
 * @param[in] stdStart TimeChangeRule object defining the end of DST
 *
 */
Timezone::Timezone(TimeChangeRule dstStart, TimeChangeRule stdStart) {
	_dst = dstStart;
	_std = stdStart;
}


#ifdef __AVR__
/**
 * Create a Timezone object from the time change rules stored in EEPROM at the given address. This only works on devices
 * with EEPROM (for now, it is limited to AVR devices).
 *
 * @brief Create a timezone object using rules stored in EEPROM
 *
 * @param[in] address EEPROM address where the timezone rules are located
 *
 */
Timezone::Timezone(int address) {
	readRules(address);
}
#endif	/* __AVR__ */


/*----------------------------------------------------------------------*
 * Convert the given UTC time to local time, standard or                *
 * daylight time, as appropriate.                                       *
 *----------------------------------------------------------------------*/
time_t Timezone::toLocal(time_t utc) {
	/* recalculate the time change points if needed */
	if (year(utc) != year(_dstUTC)) {
		calcTimeChanges(year(utc));
	}

	if (utcIsDST(utc)) {
		return utc + _dst.offset * SECS_PER_MIN;
	} else {
		return utc + _std.offset * SECS_PER_MIN;
	}
}


/*----------------------------------------------------------------------*
 * Convert the given UTC time to local time, standard or                *
 * daylight time, as appropriate, and return a pointer to the time      *
 * change rule used to do the conversion. The caller must take care     *
 * not to alter this rule.                                              *
 *----------------------------------------------------------------------*/
time_t Timezone::toLocal(time_t utc, TimeChangeRule **tcr) {
	/* recalculate the time change points if needed */
	if (year(utc) != year(_dstUTC)) {
		calcTimeChanges(year(utc));
	}

	if (utcIsDST(utc)) {
		*tcr = &_dst;
		return utc + _dst.offset * SECS_PER_MIN;
	} else {
		*tcr = &_std;
		return utc + _std.offset * SECS_PER_MIN;
	}
}


/*----------------------------------------------------------------------*
 * Convert the given local time to UTC time.                            *
 *                                                                      *
 * WARNING:                                                             *
 * This function is provided for completeness, but should seldom be     *
 * needed and should be used sparingly and carefully.                   *
 *                                                                      *
 * Ambiguous situations occur after the Standard-to-DST and the         *
 * DST-to-Standard time transitions. When changing to DST, there is     *
 * one hour of local time that does not exist, since the clock moves    *
 * forward one hour. Similarly, when changing to standard time, there   *
 * is one hour of local times that occur twice since the clock moves    *
 * back one hour.                                                       *
 *                                                                      *
 * This function does not test whether it is passed an erroneous time   *
 * value during the Local -> DST transition that does not exist.        *
 * If passed such a time, an incorrect UTC time value will be returned. *
 *                                                                      *
 * If passed a local time value during the DST -> Local transition      *
 * that occurs twice, it will be treated as the earlier time, i.e.      *
 * the time that occurs before the transistion.                         *
 *                                                                      *
 * Calling this function with local times during a transition interval  *
 * should be avoided!                                                   *
 *----------------------------------------------------------------------*/
time_t Timezone::toUTC(time_t local) {
	/* recalculate the time change points if needed */
	if (year(local) != year(_dstLoc)) {
		calcTimeChanges(year(local));
	}

	if (locIsDST(local)) {
		return local - _dst.offset * SECS_PER_MIN;
	} else {
		return local - _std.offset * SECS_PER_MIN;
	}
}


/*----------------------------------------------------------------------*
 * Determine whether the given UTC time_t is within the DST interval    *
 * or the Standard time interval.                                       *
 *----------------------------------------------------------------------*/
bool Timezone::utcIsDST(time_t utc) {
	/* recalculate the time change points if needed */
	if (year(utc) != year(_dstUTC)) {
		calcTimeChanges(year(utc));
	}

	if (_stdUTC == _dstUTC) {		/* (daylight savings time not observed in this timezone) */
		return false;
	} else if (_stdUTC > _dstUTC) {	/* (Northern Hemisphere) */
		return (utc >= _dstUTC && utc < _stdUTC);
	} else {						/* (Southern Hemisphere) */
		return !(utc >= _stdUTC && utc < _dstUTC);
	}
}


/*----------------------------------------------------------------------*
 * Determine whether the given Local time_t is within the DST interval  *
 * or the Standard time interval.                                       *
 *----------------------------------------------------------------------*/
bool Timezone::locIsDST(time_t local) {
	/* recalculate the time change points if needed */
	if (year(local) != year(_dstLoc)) {
		calcTimeChanges(year(local));
	}

	if (_stdUTC == _dstUTC) {		/* (daylight savings time not observed in this timezone) */
		return false;
	} else if (_stdLoc > _dstLoc) {	/* (Northern Hemisphere) */
		return (local >= _dstLoc && local < _stdLoc);
	} else {						/* (Southern Hemisphere) */
		return !(local >= _stdLoc && local < _dstLoc);
	}
}


/*----------------------------------------------------------------------*
 * Calculate the DST and standard time change points for the given      *
 * given year as local and UTC time_t values.                           *
 *----------------------------------------------------------------------*/
void Timezone::calcTimeChanges(int yr) {
	_dstLoc = toTime_t(_dst, yr);
	_stdLoc = toTime_t(_std, yr);

	_dstUTC = _dstLoc - _std.offset * SECS_PER_MIN;
	_stdUTC = _stdLoc - _dst.offset * SECS_PER_MIN;
}


/*----------------------------------------------------------------------*
 * Convert the given DST change rule to a time_t value                  *
 * for the given year.                                                  *
 *----------------------------------------------------------------------*/
time_t Timezone::toTime_t(TimeChangeRule r, int yr) {
	tmElements_t tm;
	time_t t;
	uint8_t m, w;				/* temp copies of r.month and r.week */

	m = r.month;
	w = r.week;

	if (w == 0) {				/* Last week = 0 */
		if (++m > 12) {			/* for "Last", go to the next month */
			m = 1;
			yr++;
		}
		w = 1;					/* and treat as first week of next month, subtract 7 days later */
	}

	tm.Hour = r.hour;
	tm.Minute = 0;
	tm.Second = 0;
	tm.Day = 1;
	tm.Month = m;
	tm.Year = yr - 1970;

	t = makeTime(tm);			/* first day of the month, or first day of next month for "Last" rules */

	t += (7 * (w - 1) + (r.dow - weekday(t) + 7) % 7) * SECS_PER_DAY;

	if (r.week == 0) {
		t -= 7 * SECS_PER_DAY;	/* back up a week if this is a "Last" rule */
	}

	return t;
}


#ifdef __AVR__
/**
 * Read the timezone rules (DST and standard time) from EEPROM, starting at the given address.
 * XXX: Only valid on devices which contain EEPROM (AVR only, for now)
 *
 * @brief Read the timezone rules from EEPROM
 *
 * @param[in] address EEPROM address where the timezone rules begin.
 *
 */
void Timezone::readRules(int address) {
	eeprom_read_block((void *) &_dst, (void *) address, sizeof(_dst));
	address += sizeof(_dst);
	eeprom_read_block((void *) &_std, (void *) address, sizeof(_std));
}


/**
 * Write the timezone rules (DST and standard time) to EEPROM, starting at the given address.
 * XXX: Only valid on devices which contain EEPROM (AVR only, for now)
 *
 * @brief Write the timezone rules to EEPROM
 *
 * @param[in] address EEPROM address where the timezone rules are to begin.
 *
 */
void Timezone::writeRules(int address) {
	eeprom_write_block((void *) &_dst, (void *) address, sizeof(_dst));
	address += sizeof(_dst);
	eeprom_write_block((void *) &_std, (void *) address, sizeof(_std));
}
#endif	/* __AVR__ */
