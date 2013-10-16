/*
 * Project: Digital Wristwatch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <Arduino.h>
#include "config.h"
#include "util.h"
#include "typedefs.h"

#ifndef ARDUINO
#define ARDUINO
#endif


#define pinWrite            digitalWrite
#define pinRead             digitalRead

typedef unsigned int millis_t;

typedef unsigned char millis8_t;

#endif /* COMMON_H_ */