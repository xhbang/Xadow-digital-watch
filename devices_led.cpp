/*
 * Project: Digital Wristwatch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

// LED control

#include <avr/io.h>
#include <avr/power.h>
#include <util/delay.h>
#include "common.h"
#include "devices_led.h"
//#include "millis/millis.h"
#include "watchconfig.h"
#include "pwrmgr.h"

#define RED_OCR		OCR0A
#define RED_COM		COM0A1
#define RED_PIN		17
#define GREEN_OCR	OCR0B
#define GREEN_COM	COM0B1
#define GREEN_PIN	11

typedef struct {
	byte flashLen;
	millis8_t startTime;
}led_s;

static led_s ledRed;
static led_s ledGreen;

static void flash(led_s*, byte, byte, volatile byte*, byte, byte);
static bool update(led_s*, byte, byte);

void led_init()
{
	//shao TCCR0A = _BV(WGM01)|_BV(WGM00);
	//shao TCCR0B = _BV(CS01)|_BV(CS00);
	//shao power_timer0_disable();

	pinMode(RED_PIN, OUTPUT);
	pinMode(GREEN_PIN, OUTPUT);
	pinWrite(RED_PIN, HIGH);
	pinWrite(GREEN_PIN, HIGH);
}

static void flash(led_s* led, byte len, byte brightness, volatile byte* ocr, byte com, byte pin)
{
	led->flashLen = len;
	led->startTime = millis();

    if (brightness == 255) 
    {
        pinWrite(pin, LOW);
    } else if (brightness == 0) {
        pinWrite(pin, HIGH);
    }
    /*shao if(brightness == 255 || brightness == 0)
    {
        TCCR0A &= ~com;
        brightness == 255 ? (PORTD |= pin) : (PORTD &= ~pin);
    }
    else
    {
        power_timer0_enable();
        TCCR0A |= com;
        *ocr = brightness;
    }*/
}

void led_flash(byte led, byte len, byte brightness)
{
	switch(led)
	{
		case LED_RED:
			flash(&ledRed, len, brightness, &RED_OCR, _BV(RED_COM), (RED_PIN));
			break;
		case LED_GREEN:
			flash(&ledGreen, len, brightness, &GREEN_OCR, _BV(GREEN_COM), (GREEN_PIN));
			break;
		default:
			break;
	}

	pwrmgr_setState(PWR_ACTIVE_LED, PWR_STATE_IDLE);
}
/*
bool led_flashing()
{
	return ledRed.flashLen || ledGreen.flashLen;
}
*/
static bool update(led_s* led, byte com, byte pin)
{
	if(led->flashLen && (millis8_t)(millis() - led->startTime) >= led->flashLen)
	{
		//shao PORTD &= pin;
		//shao TCCR0A &= com;
        pinWrite(pin, HIGH);
		led->flashLen = 0;
	}

	return led->flashLen;
}

void led_update()
{
	bool red = update(&ledRed, (byte)~_BV(RED_COM), (RED_PIN));
	bool green = update(&ledGreen, ~_BV(GREEN_COM), (GREEN_PIN));

	if(!red && !green)
	{
		//power_timer0_disable();
		pwrmgr_setState(PWR_ACTIVE_LED, PWR_STATE_NONE);
	}
}