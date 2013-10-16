/*
 * Project: Digital Wristwatch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#ifndef DS1337_H_
#define DS1337_H_

#include "typedefs.h"

#define DS1337_SQW_ON	true
#define DS1337_SQW_OFF	false

void ds1337_init(void);
void ds1337_sqw(bool);
void ds1337_get(s_time*);
void ds1337_save(s_time*);
void ds1337_temp(char*, byte*);
void ds1337_setUserAlarmWake(s_alarm*);
void ds1337_setSystemAlarmWake(s_alarm*);
bool ds1337_userAlarmState(void);
bool ds1337_systemAlarmState(void);

#endif /* DS1337_H_ */