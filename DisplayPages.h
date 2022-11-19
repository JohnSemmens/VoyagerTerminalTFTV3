// DisplayPages.h

#ifndef _DISPLAYPAGES_h
#define _DISPLAYPAGES_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

// These are Mission commands. They are part of the pre-programmed mission.
enum MissionCommandType {
	ctGotoWaypoint,	// goto waypoint, stay within boundary distance of the rumb line (max CTE)
	ctLoiter,		// loiter at the waypoint, for duration, and within boundary distance
	ctLoiterUntil,	// loiter at the waypoint, until a time of day, and within boundary distance
	ctReturnToHome,	// return to the preset home location
	ctSteerWindCourse // Steer a wind course.
};

enum EncoderModeType {
	emPageMode,			// rotary encoder is in page mode
	emParameterIndex,	// rotary encoder is in Parameter Index Mode 
	emValue,				// Value Mode
	emValue2				// second Value Mode
};


enum DisplayModeType {
	dmStart,
	dmLPO,
	dmLNA,
	//dmLNAPOS,
	dmLAT,
	dmLWP,
	dmLMI,
	dmLWI,
	dmLVS,
	dmVER,
	dmTMG,
	dmEQG,
	dmParameters,
	dmConfig,
	dmResetMissionIndex,
	dmSteerCompassCourse,
	dmSteerWindAngle,
	dmHome,
	dmDisplay,
	dmLocalGPS,
	dmWingSail,
	dmLoRaTerminal
};

enum CommandStateType{
	csIdle,                  // idle state, feathered settings For sails.
	//csFullManual,            // The vessel is under full manual command via RC. i.e. steering and sail
	//csPartialManual ,        // The vessel Is under Partial manual command via RC. i.e. steering only, With auto-sail control
	csFollowMission,         // The vessel Is under automatic control following the mission list.
	csSteerMagneticCourse,   // The vessel Is steering a course relative To the compass
	csSteerWindCourse,       // The vessel Is steering a course relative To the wind.
	csReturnToHome,          // Return To the preset home location
	csSetHome,               // Set home location
	csResetMissionIndex,     // Reset the Mission Index, To force a restsart Of the mission.
	csLoiter                 // Loiter here.
};

enum DisplayDetailType {
	ddDescription,  // Vessel OLED Display Page Description
	ddMessage		// Vessel OLED Display Page Message Parameter
};

// this is the current state of the vessel. This would be generally controlled externally (manually) from either an RC link, or a Serial link.
enum VesselCommandStateType {
	vcsIdle,				// idle state, feathered settings for sails.
	vcsFullManual,			// The vessel is under full manual command via RC. i.e. steering and sail
	vcsPartialManual,		// The vessel is under partial manual command via RC. i.e. steering only, with auto-sail control
	vcsFollowMission,		// The vessel is under automatic control following the mission list.
	vcsSteerMagneticCourse,	// The vessel is steering a course relative to the compass.
	vcsSteerWindCourse,		// The vessel is steering a course relative to the wind.
	vcsReturnToHome,		// return to the preset home location
	vcsSetHome,				// set home location
	vcsResetMissionIndex,	// Reset the Mission Index, to force a restart of the mission.
	vcsLoiter				// Loiter here
};

void DisplayPage(int DisplayIndex);

void CursorUpdateLoop(void*);

String GetVoyagerDisplayDetails(DisplayDetailType DetailType, int ValueIndex);

#endif

