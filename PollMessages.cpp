// 
// 
// 

#include "PollMessages.h"

extern volatile unsigned int DisplayIndex2;

void PollVoyager(int DisplayIndex)
{
	// poll the Vessel with the message realted to the currently displayed page or Display index

	switch (DisplayIndex)
	{
	case dmStart:
		// no poll
		break;

	case dmLPO:
		Serial1.println(F("lpo"));
		break;

	case dmLNA:
	case dmLocalGPS:
		Serial1.println("lna");
		break;

	case dmSteerCompassCourse:
	case dmLAT:
		Serial1.println("lat");
		break;

	case dmLWP:
		Serial1.println("lwp");
		break;

	case dmLMI:
	case dmResetMissionIndex:
		Serial1.println("lmi");
		break;

	case dmLWI:
		Serial1.println("lwi");
		break;

	case dmLVS:
		Serial1.println("lvs");
		break;

	case dmVER:
		Serial1.println("ver");
		break;

	case dmTMG:
		Serial1.println("tmg");
		break;

	case dmEQG:
		Serial1.println("eqg");
		break;

	case dmSteerWindAngle:
		Serial1.println("lpf");
		break;

	case dmHome:
		Serial1.println("hlg");
		break;

	case dmWingSail:
		Serial1.println("lws");
		break;

	default:;
	}
}

