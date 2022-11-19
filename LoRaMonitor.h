// LoRaMonitor.h

#ifndef _LoRaMonitor_h
#define _LoRaMonitor_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

//define some colour values
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

void LoRaMonitorAdd(char* CLI_Msg);
void LoRaMonitorAdd(String CLI_Msg);

#endif

