// 
// 
// 

#include "GPSHandler.h"
#include "TinyGPS++.h"
#include "Location.h"

Location Currentloc;
extern Location myloc;
extern double mySOG;
extern double myCOG;

extern bool GPSEnabled;
//extern float myloc_lat;
//extern float myloc_lng;

extern TinyGPSPlus gps;
extern int Range;
extern int Bearing;

#define GPSEnablePin	0

extern float Currentloc_lat;
extern float Currentloc_lng;

void GPSReadLoop(void*)
{
	if (GPSEnabled) {
		// This sketch displays information every time a new sentence is correctly encoded.
		while (Serial2.available() > 0)
		{
			if (gps.encode(Serial2.read()))
			{
				if (gps.location.isValid())
				{
					myloc.lat = gps.location.lat() * 10000000UL;
					myloc.lng = gps.location.lng() * 10000000UL;
					mySOG = gps.speed.mps();
					myCOG = gps.course.deg();

					Currentloc.lat = Currentloc_lat * 10000000UL;
					Currentloc.lng = Currentloc_lng * 10000000UL;

					Range = get_distance(myloc,Currentloc);
					Bearing = get_bearingf(myloc,Currentloc);
				}
				else
				{
					myloc.lat = 0;
					myloc.lng = 0;
					Range = 0;
					Bearing = 0;
				}
			}
		}


	}

}
