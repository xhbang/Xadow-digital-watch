/*
 * Project: Digital Wristwatch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#include <avr/io.h>
#include <string.h>
#include "common.h"
#include "devices_ds1337.h"
#include "devices_ds1337_defs.h"
#include <Wire.h>
#include "drivers_ds1337.h"

DS1337 RTC = DS1337();

void ds1337_init()
{
    RTC.start(); // ensure RTC oscillator is running, if not already
    
	// Set registers

	byte data;

	// Control
	// Enable oscillator
	data = RTC.getRegister(DS1337ADDR_CTRL);
	bits_clr(data, _BV(DS1337BIT_EOSC)|_BV(DS1337BIT_RS1)|_BV(DS1337BIT_RS2));
	RTC.setRegister(DS1337ADDR_CTRL, data);

	// Status
	// Turn off 32KHz output
	// Unset alarm 1 & 2 interrupts
	data = RTC.getRegister(DS1337ADDR_STATUS);
	bits_clr(data, _BV(DS1337BIT_A2F)|_BV(DS1337BIT_A1F));
	RTC.setRegister(DS1337ADDR_STATUS, data);

	// Set 1Hz output
	ds1337_sqw(DS1337_SQW_ON);
}

// Switch between 1Hz output and alarm interrupt output
void ds1337_sqw(bool enable)
{
	byte data;
	data = RTC.getRegister(DS1337ADDR_CTRL);
	if(enable) // 1Hz output
	{
		bit_clr(data, DS1337BIT_INTCN);
		bit_clr(data, DS1337BIT_RS2);
		bit_clr(data, DS1337BIT_RS1);
	}
	else // Alarm output
	{
		bit_set(data, DS1337BIT_INTCN);
	}		
	RTC.setRegister(DS1337ADDR_CTRL, data);
}

// Get time
void ds1337_get(s_time* timeData)
{
	//read(DS1337ADDR_SECS, (byte*)timeData, sizeof(s_time));
    RTC.readTime();
    timeData->secs = RTC.getSeconds();
    timeData->mins = RTC.getMinutes();
    timeData->hours = RTC.getHours();
    timeData->date = RTC.getDays();
    timeData->day = RTC.getDayOfWeek();
    timeData->month = RTC.getMonths();
    timeData->year = RTC.getYears();

	// Month and day start at 1, but we want to start at 0
	if(timeData->day > 0)
		timeData->day--;

	// Can't subtract 1 if month is BCD 10, have to do it manually
	if(timeData->month > 0)
	{
		timeData->month--;
	}		
}

// Save time
void ds1337_save(s_time* timeData)
{
	// Month is stored starting at 0 in program, but RTC starts at 1
	//byte month = timeData->month + 1;
	//if(month == 0b00001010) // invalid BCD, added 1 to 9 = 10 = bad
		//month = 0b00010000; // add 6 to fix, but that increases program size, so just set manually

	//write(DS1337ADDR_SECS,	timeData->secs);
	//write(DS1337ADDR_MINS,	timeData->mins);
	//write(DS1337ADDR_HRS,	timeData->hours);
	//write(DS1337ADDR_DAY,	timeData->day + 1); // Day is also stored starting at 0 in program, but RTC starts at 1
	//write(DS1337ADDR_DATE,	timeData->date);
	//write(DS1337ADDR_MONTH,	month);
	//write(DS1337ADDR_YEAR,	timeData->year);
    RTC.setSeconds(timeData->secs);
    RTC.setMinutes(timeData->mins);
    RTC.setHours(timeData->hours);
    RTC.setDays(timeData->date);
    RTC.setDayOfWeek(timeData->day+1);
    RTC.setMonths(timeData->month+1);
    RTC.setYears(timeData->year);

}

// Get temperature
void ds1337_temp(char* tempInt, byte* tempFrac)
{
	//shao read(DS1337ADDR_TEMPH, (byte*)tempInt, 1);
	//shao read(DS1337ADDR_TEMPL, tempFrac, 1);
}

// User alarm uses alarm 2
void ds1337_setUserAlarmWake(s_alarm* alarm)
{
	// Clear flag
	ds1337_userAlarmState();

	byte data;
	data = RTC.getRegister(DS1337ADDR_CTRL);
	if(alarm != NULL)
	{
		RTC.setRegister(DS1337ADDR_ALARM1_SECS,	0);
		RTC.setRegister(DS1337ADDR_ALARM1_MINS,	alarm->min);
		RTC.setRegister(DS1337ADDR_ALARM1_HRS,	alarm->hour);
		RTC.setRegister(DS1337ADDR_ALARM1_DAYDATE,0b10000000 | alarm->days);
		bit_set(data, DS1337BIT_A2IE);
	}
	else // Alarm disabled
		bit_clr(data, DS1337BIT_A2IE);
	RTC.setRegister(DS1337ADDR_CTRL, data);
}

// System wakeup uses alarm 1
// NOTE: Alarm 2 doesn't have seconds
void ds1337_setSystemAlarmWake(s_alarm* alarm)
{
	// Clear flag
	ds1337_systemAlarmState();

	byte data;
	data = RTC.getRegister(DS1337ADDR_CTRL);
	if(alarm != NULL)
	{
		RTC.setRegister(DS1337ADDR_ALARM2_MINS,	alarm->min);
		RTC.setRegister(DS1337ADDR_ALARM2_HRS,	0b10000000 | alarm->hour);
		RTC.setRegister(DS1337ADDR_ALARM2_DAYDATE,0b10000000 | alarm->days);
		bit_set(data, DS1337BIT_A1IE);
	}
	else // Alarm disabled
		bit_clr(data, DS1337BIT_A1IE);
	RTC.setRegister(DS1337ADDR_CTRL, data);
}

// Get user alarm state
bool ds1337_userAlarmState()
{
	byte data;
	data = RTC.getRegister(DS1337ADDR_STATUS);
	bool state = data & _BV(DS1337BIT_A1F);
	if(state) // Clear state if its set
	{
		bit_clr(data, DS1337BIT_A1F);
		RTC.setRegister(DS1337ADDR_STATUS, data);
	}

	return state;
}

// Get systel alarm state
bool ds1337_systemAlarmState()
{
    return false;
	/*shao byte data;
	data = RTC.getRegister(DS1337ADDR_STATUS);
	bool state = data & _BV(DS1337BIT_A1F);
	if(state) // Clear state if its set
	{
		bit_clr(data, DS1337BIT_A1F);
		RTC.setRegister(DS1337ADDR_STATUS, data);
	}

	return state;*/
}


