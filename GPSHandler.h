// GPSHandler.h

#ifndef _GPSHandler_h
#define _GPSHandler_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

void GPSReadLoop(void*);



#endif

