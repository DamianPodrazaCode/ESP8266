/*
 * ds18b20.cpp
 *
 *  Created on: 17 sty 2026
 *      Author: ZoMbiE
 */

#include "main.h"

#include "ds18b20.h"

#include <OneWire.h>
#include <DallasTemperature.h>

// ds18b20 ------------------------------------------------------
OneWire ds18x20[] = {14, 12}; // nazwy pinów do których są podpięte czujniki
const int oneWireCount = sizeof(ds18x20) / sizeof(OneWire);
DallasTemperature sensor[oneWireCount];
const uint8_t resolution = 12;
// ds18b20 ------------------------------------------------------

// ---------------------------------------------------------------
void initTemperatureSensor()
{
	// pobranie adresów i ustalenie rozdzielczości DS18b20
	DeviceAddress deviceAddress;
	for (int i = 0; i < oneWireCount; i++)
	{
		sensor[i].setOneWire(&ds18x20[i]);
		sensor[i].begin();
		if (sensor[i].getAddress(deviceAddress, 0))
			sensor[i].setResolution(deviceAddress, resolution);
	}
}

// ---------------------------------------------------------------
// odczyt z DS18b20 x 5, odczytywać nie częciej niż co 750ms
void updateNextTemperatureSensor()
{
	static int step = 0;
	sensor[step].setWaitForConversion(false);
	sensor[step].requestTemperatures();
	sensor[step].setWaitForConversion(true);
	glTablica.data.ds[step] = sensor[step].getTempCByIndex(0);
	step++;
	if (step >= oneWireCount)
		step = 0;
}

// ---------------------------------------------------------------
