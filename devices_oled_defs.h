/*
 * Project: Digital Wristwatch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#ifndef OLED_DEFS_H_
#define OLED_DEFS_H_

// Commands
#define OLED_OFF				0xAE // Power off
#define OLED_ON					0xAF // Power on
#define OLED_MEMMODE			0x20 // Memory addressing mode
#define OLED_NONINVERT			0xA6 // Noninvert
#define OLED_INVERT				0xA7 // Invert
#define OLED_SEG_NML			0xA0 // Seg remap left to right
#define OLED_SEG_REMAP			0xA1 // Seg remap right to left
#define OLED_SETCONTRAST		0x81 // Set contrast
#define OLED_SETMUX				0xA8 // MUX
#define OLED_SCANDIR_NML		0xC0 // Scan direction top to bottom
#define OLED_SCANDIR_REMAP		0xC8 // Scan direction bottom to top
#define OLED_DISPLAY_OFFSET		0xD3
#define OLED_COMCFG				0xDA
#define OLED_DISPLAY_SHOWRAM	0xA4 // Show RAM contents
#define OLED_DISPLAY_HIDERAM	0xA5 // Blank screen, but still on
#define OLED_CHARGEPUMP			0x8D
#define OLED_CLOCK_FREQ			0xD5
#define OLED_DISPLAY_START		0x40
#define OLED_PRECHARGE_PERIOD	0xD9
#define OLED_VCOM_LEVEL			0xDB

// Data
#define OLED_MEM_HORIZ		0x00
#define OLED_MEM_VERT		0x01
#define OLED_MEM_PAGE		0x02
#define OLED_CHARGEPUMP_EN	0x14
#define OLED_CHARGEPUMP_DIS	0x10



//shao
#define SeeedOLED_Max_X 		127	//128 Pixels
#define SeeedOLED_Max_Y 		63	//64  Pixels

#define PAGE_MODE			01
#define HORIZONTAL_MODE			02


#define SeeedOLED_Address		0x3c
#define SeeedOLED_Command_Mode		0x80
#define SeeedOLED_Data_Mode		0x40
#define SeeedOLED_Display_Off_Cmd	0xAE
#define SeeedOLED_Display_On_Cmd	0xAF
#define SeeedOLED_Normal_Display_Cmd	0xA6
#define SeeedOLED_Inverse_Display_Cmd	0xA7
#define SeeedOLED_Activate_Scroll_Cmd	0x2F
#define SeeedOLED_Dectivate_Scroll_Cmd	0x2E
#define SeeedOLED_Set_Brightness_Cmd	0x81

#define Scroll_Left			0x00
#define Scroll_Right			0x01

#define Scroll_2Frames			0x7
#define Scroll_3Frames			0x4
#define Scroll_4Frames			0x5
#define Scroll_5Frames			0x0
#define Scroll_25Frames			0x6
#define Scroll_64Frames			0x1
#define Scroll_128Frames		0x2
#define Scroll_256Frames		0x3



#endif /* OLED_DEFS_H_ */