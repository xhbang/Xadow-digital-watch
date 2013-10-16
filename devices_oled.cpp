/*
 * Project: Digital Wristwatch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

// OLED control

#include <avr/io.h>
#include <util/delay.h>
#include "common.h"
#include "devices_oled.h"
#include "devices_oled_defs.h"
//#include "drivers_spi.h"
#include "watchconfig.h"
#include <Wire.h>

/*shao #define CSN	B2
#define DC	B0
#define RST	D7

#define spiSelect()		(pinWrite(CSN, LOW))
#define spiDeselect()	(pinWrite(CSN, HIGH))
#define cmdMode()		(pinWrite(DC, LOW))
#define dataMode()		(pinWrite(DC, HIGH))
#define rstHigh()		(pinWrite(RST, HIGH))
#define rstLow()		(pinWrite(RST, LOW))

static inline bool oled_select(void)
{
	spi_enable();
	spiSelect();
	return true;
}

static inline bool oled_deselect(void)
{
	spiDeselect();
	spi_disable();
	return false;
}

#define MODE_CMD	(cmdMode())
#define MODE_DATA	(dataMode())
#define CHIPSELECT(mode) mode; \
	for(bool cs = oled_select(); cs; cs = oled_deselect()) 
*/
byte oledBuffer[FRAME_BUFFER_SIZE];

static void resetPosition(void);
static void sendCommand(unsigned char command);
static void sendData(unsigned char Data);

void oled_init()
{
	/*shao pinMode(CSN, OUTPUT);
	pinMode(DC, OUTPUT);

	rstHigh();
	pinMode(RST, OUTPUT);

	delay(50);
	rstLow();
	delay(50);
	rstHigh();
	delay(50);

//	sendCmd(OLED_OFF);

	resetPosition();

	sendCmd2(OLED_SETMUX, 63);
	sendCmd2(OLED_DISPLAY_OFFSET, 0);
	sendCmd(OLED_DISPLAY_START | 0);
	oled_set180(watchConfig.display180);
	sendCmd2(OLED_COMCFG, 0x12);
	oled_setBrightness(watchConfig.brightness * 85);
	sendCmd(OLED_DISPLAY_SHOWRAM);
	oled_setInvert(watchConfig.invert);
	sendCmd2(OLED_CLOCK_FREQ, 0x80);
	sendCmd2(OLED_CHARGEPUMP, OLED_CHARGEPUMP_EN);

	sendCmd(OLED_ON);

//	sendCmd2(OLED_PRECHARGE_PERIOD, 0b11110001);//0xF1); // pre-charge period
//	sendCmd2(OLED_VCOM_LEVEL, 0x20); // 0x40 // V-COM deselect level
	sendCmd2(OLED_MEMMODE, OLED_MEM_HORIZ);

	delay(20);
	oled_flush();
	delay(10);*/

    sendCommand(SeeedOLED_Display_Off_Cmd); 	//display off
    delay(5); 
    sendCommand(SeeedOLED_Display_On_Cmd); 	//display on
    delay(5); 
    sendCommand(SeeedOLED_Normal_Display_Cmd);  //Set Normal Display (default)
}


static void sendData(byte Data)
{
	Wire.beginTransmission(SeeedOLED_Address); // begin I2C transmission
#if defined(ARDUINO) && ARDUINO >= 100
     Wire.write(SeeedOLED_Data_Mode);            // data mode
     Wire.write(Data);
#else
     Wire.send(SeeedOLED_Data_Mode);            // data mode
     Wire.send(Data);
#endif
     Wire.endTransmission();                    // stop I2C transmission
}

static void sendCommand(byte command)
{
	Wire.beginTransmission(SeeedOLED_Address); // begin I2C communication
#if defined(ARDUINO) && ARDUINO >= 100
  Wire.write(SeeedOLED_Command_Mode);	     // Set OLED Command mode
  Wire.write(command);
#else
  Wire.send(SeeedOLED_Command_Mode);	     // Set OLED Command mode
  Wire.send(command);
#endif
  Wire.endTransmission();    		     // End I2C communication	
}

void setHorizontalMode()
{
    sendCommand(0x20); 			//set addressing mode
    sendCommand(0x00); 			//set horizontal addressing mode
}

void setPageMode()
{
    sendCommand(0x20); 			//set addressing mode
    sendCommand(0x02); 			//set page addressing mode
}

void setTextXY(unsigned char Row, unsigned char Column)
{
    sendCommand(0xB0 + Row); 			//set page address
    sendCommand(0x00 + (8*Column & 0x0F)); 	//set column lower address
    sendCommand(0x10 + ((8*Column>>4)&0x0F)); 	//set column higher address
}


static void resetPosition()
{
	/*shao CHIPSELECT(MODE_CMD)
	{
		spi_transfer_nr(0x22);
		spi_transfer_nr(0x00);
		spi_transfer_nr(0x07);

		spi_transfer_nr(0x21);
		spi_transfer_nr(0x00);
		spi_transfer_nr(0x7F);
	}*/
    setPageMode();
    setTextXY(0,0); 
    setHorizontalMode();
    delay(5);
}

void oled_flush()
{
	resetPosition();

	/*shao CHIPSELECT(MODE_DATA)
	{
		debugPin_spi(HIGH);

		// Here we load a byte from the global variable buffer into a fast local variable.
		// This is done while we're waiting for the SPI transfer to complete.
		// Delay cycles is also used to replace loop_until_bit_is_set().
		// In total, we go from ~3.84ms transfer time of the basic loop to ~2.31ms using local variables and delay cycles. (4MHz SPI, 1KB frame buffer)
		// As an added bonus we clear the frame buffer with the extra clock cycles which means we don't have to wait for memset() to clear it later on.
		// The minimum possible transfer time for 4MHz SPI and 1KB frame buffer is 2.048ms. 12.5% overhead (1 cycle to read SPSR, 1 cycle to set SPDR)... not bad :3

		for(uint i=0;i<FRAME_BUFFER_SIZE;i++)
		{
			byte next = oledBuffer[i]; // Load next byte
			oledBuffer[i] = 0x00; // Clear buffer byte

			// At max speed SPI takes 16 cycles to send a byte
			// 8 cycles were used to increment the loop counter, compare the counter, loading next byte and clearing buffer byte
			// 8 cycles are left
			//shao __builtin_avr_delay_cycles(8);

			SPSR; // Need to read register to clear SPIF

			SPDR = next; // Send byte
		}

		// After sending the last byte the for loop exits, so we need to wait for the transfer to finish here
		//shao __builtin_avr_delay_cycles(14);
		SPSR;
		//loop_until_bit_is_set(SPSR, SPIF);

		// Original method
		//for(uint i=0;i<FRAME_BUFFER_SIZE;i++)
		//	spi_transfer_nr(oledBuffer[i]);

		debugPin_spi(LOW);
	}*/

    /*shao if (watchConfig.display180) 
    {
        for (uint i = (FRAME_BUFFER_SIZE-1); i >=0; i--) 
        {
            byte next = oledBuffer[i]; // Load next byte
            oledBuffer[i] = 0x00; // Clear buffer byte

            sendData(next);
        }
    }else
    {*/
        for (uint i = 0; i < FRAME_BUFFER_SIZE; i++) 
        {
            //Serial.print(oledBuffer[i]);
            byte next = oledBuffer[i]; // Load next byte
            oledBuffer[i] = 0x00; // Clear buffer byte

            sendData(next);
        }
    //shao }
}

void oled_power(bool on)
{
    if (on) {
        sendCommand(SeeedOLED_Display_On_Cmd); 	//display on
        delay(5); 
        sendCommand(SeeedOLED_Normal_Display_Cmd);  //Set Normal Display (default)
    }else
    {
        sendCommand(SeeedOLED_Display_Off_Cmd); 	//display off
        delay(5); 
    }
}

void oled_setBrightness(byte brightness)
{
	sendCommand(SeeedOLED_Set_Brightness_Cmd);
    sendCommand(brightness);
}

void oled_setInvert(bool invert)
{
    if (invert) {
        sendCommand(SeeedOLED_Inverse_Display_Cmd);
    }else
    {
        sendCommand(SeeedOLED_Normal_Display_Cmd);
    }
}

void oled_set180(bool rotate)
{
	/*shao if(rotate)
	{
		sendCmd(OLED_SEG_REMAP);
		sendCmd(OLED_SCANDIR_REMAP);
	}
	else
	{
		sendCmd(OLED_SEG_NML);
		sendCmd(OLED_SCANDIR_NML);
	}*/
}

