/*
 * tabData.h
 *
 *  Created on: 12 gru 2020
 *      Author: ZoMbiE
 */

#include <Arduino.h>

#ifndef TABDATA_H_
#define TABDATA_H_

struct tablica
{
	float ds[2];
	uint8_t out;
	uint8_t ssid[32];
	uint8_t pass[32];
	uint8_t statusLan[32];
	uint8_t timeDelay;
};

class tabData
{
public:
	tablica data;

	tabData();
	virtual ~tabData();
};

#endif /* TABDATA_H_ */
