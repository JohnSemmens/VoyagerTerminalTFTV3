// PollMessages.h

#ifndef _PollMessages_h
#define _PollMessages_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "DisplayPages.h"

void PollVoyager(int DisplayIndex);

//void SendWakeupPrefix();

#endif

