/*
 * tabData.h
 *
 *  Created on: 17 sty 2026
 *      Author: ZoMbiE
 */

#include <Arduino.h>

#ifndef TABDATA_H_
#define TABDATA_H_

struct tablica
{
	// uint8_t bStart;
	// uint8_t bStop;
	float ds[2];
	uint8_t outPWM; 
	uint8_t out;
	uint8_t timeDelay; // debug - maksymalny czas trwania pętli zadań
	uint8_t ssid[32];
	uint8_t pass[32];
	uint8_t statusLan[32];
};

class tabData
{
public:
	tablica data;

	tabData();
	virtual ~tabData();
};

#endif /* TABDATA_H_ */
