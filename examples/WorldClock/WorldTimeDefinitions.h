/*
 * WorldTimeDefinitions.h
 * -- Part of the 'WorldClock' example for the 'Timezone' library
 *
 * Contains definitions of the timezones used in the example.
 *
 */

#ifndef __WORLDTIMEDEFINITIONS_H__
#define __WORLDTIMEDEFINITIONS_H__

#include <Timezone.h>

/*
 * Australia Eastern Time Zone
 * (Sydney, Melbourne)
 */
/* DST: UTC + 11 hours */
TimeChangeRule aEDT = {
	"AEDT",
	First,
	Sun,
	Oct,
	2,
	660
};

/* STD: UTC + 10 hours */
TimeChangeRule aEST = {
	"AEST",
	First,
	Sun,
	Apr,
	3,
	600
};

/* Timezone object: ausET */
Timezone ausET(aEDT, aEST);


/*
 * Central European Time
 * (Frankfurt, Paris)
 */
/* Central European Summer Time: UTC + 2 hours */
TimeChangeRule CEST = {
	"CEST",
	Last,
	Sun,
	Mar,
	2,
	120
};

/* Central European Standard Time: UTC + 1 hour */
TimeChangeRule CET = {
	"CET",
	Last,
	Sun,
	Oct,
	3,
	60
};

/* Timezone object: CE */
Timezone CE(CEST, CET);


/*
 * United Kingdom
 * (London, Belfast)
 */
/* British Summer Time: UTC + 1 hour */
TimeChangeRule BST = {
	"BST",
	Last,
	Sun,
	Mar,
	1,
	60
};

/* Standard Time: UTC */
TimeChangeRule GMT = {
	"GMT",
	Last,
	Sun,
	Oct,
	2,
	0
};

/* Timezone object: UK */
Timezone UK(BST, GMT);


/*
 * US Eastern Time Zone
 * (New York, Detroit)
 */
/* Eastern Daylight Time: UTC - 4 hours */
TimeChangeRule usEDT = {
	"EDT",
	Second,
	Sun,
	Mar,
	2,
	-240
};

/* Eastern Standard Time: UTC - 5 hours */
TimeChangeRule usEST = {
	"EST",
	First,
	Sun,
	Nov,
	2,
	-300
};

/* Timezone object: usET */
Timezone usET(usEDT, usEST);


/*
 * US Central Time Zone
 * (Chicago, Houston)
 */
/* Central Daylight Time: UTC - 5 hours */
TimeChangeRule usCDT = {
	"CDT",
	Second,
	dowSunday,
	Mar,
	2,
	-300
};

/* Central Standard Time: UTC - 6 hours */
TimeChangeRule usCST = {
	"CST",
	First,
	dowSunday,
	Nov,
	2,
	-360
};

/* Timezone object: usCT */
Timezone usCT(usCDT, usCST);


/*
 * US Mountain Time Zone
 * (Denver, Salt Lake City)
 */
/* Mountain Daylight Time: UTC - 6 hours */
TimeChangeRule usMDT = {
	"MDT",
	Second,
	dowSunday,
	Mar,
	2,
	-360
};

/* Mountain Standard Time: UTC - 7 hours */
TimeChangeRule usMST = {
	"MST",
	First,
	dowSunday,
	Nov,
	2,
	-420
};

/* Timezone object: usMT */
Timezone usMT(usMDT, usMST);


/*
 * Arizona uses the US Mountain Time Zone, but does not use DST
 */
/* Timezone object: usAZ */
Timezone usAZ(usMST, usMST);


/*
 * US Pacific Time Zone
 * (Las Vegas, Los Angeles)
 */
/* Pacific Daylight Time: UTC - 7 hours */
TimeChangeRule usPDT = {
	"PDT",
	Second,
	dowSunday,
	Mar,
	2,
	-420
};

/* Pacific Standard Time: UTC - 8 hours */
TimeChangeRule usPST = {
	"PST",
	First,
	dowSunday,
	Nov,
	2,
	-480
};

/* Timezone object: usPT */
Timezone usPT(usPDT, usPST);

#endif	/* __WORLDTIMEDEFINITIONS_H__ */
