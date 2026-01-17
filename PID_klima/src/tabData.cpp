/*
 * tabData.cpp
 *
 *  Created on: 17 sty 2026
 *      Author: ZoMbiE
 */

#include "tabData.h"

tabData::tabData()
{
	data.bStart = 1;
	data.bStop = 1;
	data.out = 1; 
	data.outPWM = 50;

	// sprintf((char*) data.ssid, "destyl"); flashIO.h
	// sprintf((char*) data.pass, "destyl1234"); flashIO.h
	// sprintf((char *)data.statusLan, "status lan ok");
}

tabData::~tabData()
{
}
