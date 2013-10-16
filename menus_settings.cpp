/*
 * Project: Digital Wristwatch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#include <avr/pgmspace.h>
#include <string.h>
#include "typedefs.h"
#include "menus_settings.h"
#include "menus_timedate.h"
#include "menus_sleep.h"
#include "menus_sound.h"
#include "menus_display.h"
#include "resources.h"
#include "menu.h"
#include "menus_functions.h"

#define OPTION_COUNT	2
#define OPTION_EXIT		OPTION_COUNT - 1

static s_prev_menu prevMenuData;

static void mSelect(void);

void mSettingsOpen()
{
	setMenuInfo(OPTION_COUNT, PSTR("   < SETTINGS >"), MENU_TYPE_ICON, mSelect, upOption, downOption);

	setMenuOption_P(0, PSTR("Time & date"), menu_timedate, mTimeDateOpen);
	//shao setMenuOption_P(1, PSTR("Sleep"), menu_sleep, mSleepOpen);
	//shao setMenuOption_P(2, PSTR("Sound"), menu_sound, mSoundOpen);
	//shao setMenuOption_P(3, PSTR("Display"), menu_display, mDisplayOpen);
//	setMenuOption_P(4, PSTR("LEDs"), NULL, NULL);
//	setMenuOption_P(5, PSTR("UI"), NULL, NULL);
//	setMenuOption_P(6, PSTR("RC Settings"), NULL, NULL);
	setMenuOption_P(OPTION_EXIT, menuBack, menu_exit, back);
	
	setPrevMenuOpen(&prevMenuData, mSettingsOpen);

	beginAnimation2(NULL);
}

static void mSelect()
{
	setPrevMenuExit(&prevMenuData, OPTION_EXIT);
	doAction(true);
}
