




#include <Wire.h>
#include <avr/pgmspace.h>
#include "drivers_ds1337.h"
//#include "programStrings.h"


// NOTE: To keep the math from getting even more lengthy/annoying than it already is, the following constraints are imposed:
//   1) All times are in 24-hour format (military time)
//   2) DayOfWeek field is not used internally or checked for validity. Alarm functions may optionally set alarms repeating on DayOfWeek, but this feature has not been tested yet.
//   3) This library's buffer stores all times in raw BCD format, just as it is sent from the RTC.
//      It is not converted to/from 'real' (binary) values until needed via get...() and set...() functions.
//      In other words, don't go hacking around and reading from the rtc_bcd[] buffer directly, unless you want the raw BCD results.


// Cumulative number of days elapsed at the start of each month, assuming a normal (non-leap) year.
unsigned int monthdays[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

DS1337::DS1337()
{
	Wire.begin();
}



unsigned char DS1337::getRegister(unsigned char registerNumber)
{
	Wire.beginTransmission(DS1337_CTRL_ID);
	Wire.write(registerNumber);
	Wire.endTransmission();

	Wire.requestFrom(DS1337_CTRL_ID, 1);

	return Wire.read();
}

void DS1337::setRegister(unsigned char registerNumber, unsigned char value)
{
	Wire.beginTransmission(DS1337_CTRL_ID);
	Wire.write(registerNumber); // set register pointer

	Wire.write(value);

	Wire.endTransmission();
}

void DS1337::readTime(void)
{
// use the Wire lib to connect to tho rtc
// reset the register pointer to zero
	Wire.beginTransmission(DS1337_CTRL_ID);
	Wire.write(0x00);
	Wire.endTransmission();

// request the 7 bytes of data    (secs, min, hr, dow, date. mth, yr)
	Wire.requestFrom(DS1337_CTRL_ID, 7);
	for(int i=0; i<7; i++)
	{
	// store data in raw bcd format
		if (Wire.available())
			rtc_bcd[i]=Wire.read();
	}
}

unsigned char DS1337::getSeconds()
{
    return bcd2bin(rtc_bcd[DS1337_SEC]);
}

unsigned char DS1337::getMinutes()
{
    return bcd2bin(rtc_bcd[DS1337_MIN]);
}
unsigned char DS1337::getHours()
{
    return bcd2bin(rtc_bcd[DS1337_HR]);
}
unsigned char DS1337::getDays()
{
    return bcd2bin(rtc_bcd[DS1337_DATE]);
}
unsigned char DS1337::getDayOfWeek()
{
    return bcd2bin(rtc_bcd[DS1337_DOW]);
}
unsigned char DS1337::getMonths()
{
    return bcd2bin(rtc_bcd[DS1337_MTH]);
}
unsigned int DS1337::getYears()
{
    return 2000 + bcd2bin(rtc_bcd[DS1337_YR]);
}


void DS1337::setSeconds(unsigned char v)
{
    rtc_bcd[DS1337_SEC] = bin2bcd(v);

}
void DS1337::setMinutes(unsigned char v)
{
    rtc_bcd[DS1337_MIN] = bin2bcd(v);

}
void DS1337::setHours(unsigned char v)
{
    rtc_bcd[DS1337_HR] = bin2bcd(v);

}
void DS1337::setDays(unsigned char v)
{
    rtc_bcd[DS1337_DATE] = bin2bcd(v);

}
void DS1337::setDayOfWeek(unsigned char v)
{
    rtc_bcd[DS1337_DOW] = bin2bcd(v);

}
void DS1337::setMonths(unsigned char v)
{
    rtc_bcd[DS1337_MTH] = bin2bcd(v);

}
void DS1337::setYears(unsigned int v)
{
    if (v>1999)
    {
        v -= 2000;
    }
    rtc_bcd[DS1337_YR] = bin2bcd(v);

}

byte DS1337::bcd2bin(byte v)
{
   return (v&0x0F) + ((v>>4)*10);
}

byte DS1337::bin2bcd(byte v)
{
   return ((v / 10)<<4) + (v % 10);
}

void DS1337::stop(void)
{
	setRegister(DS1337_SP, getRegister(DS1337_SP) | DS1337_SP_EOSC);
}

void DS1337::start(void)
{
	setRegister(DS1337_SP, getRegister(DS1337_SP) & !DS1337_SP_EOSC);
}
