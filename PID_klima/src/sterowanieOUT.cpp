/*
 * sterowanieOUT.cpp
 *
 *  Created on: 17 sty 2026
 *      Author: ZoMbiE
 *
 */

#include "sterowanieOUT.h"
#include "Arduino.h"
#include "main.h"

// pwm dla grzałek
const int out_pin = 4;
const int outPWM_pin = 5;

void initOUT()
{
	// init dla out i outPWM
	pinMode(out_pin,OUTPUT);
	digitalWrite(out_pin, 0);
	analogWrite(outPWM_pin, 0);
}

void updateOUT()
{
	digitalWrite(out_pin, glTablica.data.out);
	analogWrite(outPWM_pin, glTablica.data.outPWM);
}
