/*
 * Project: Digital Wristwatch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "common.h"


#define USB_CONNECTED()	(!pinRead(A2))
#define CHARGING()		(!pinRead(A2))

#endif /* GLOBAL_H_ */