// 
// 
// 

#include "DisplayPages.h"
#include "Location.h"
#include "LoRaMonitor.h"

#include "LCDWIKI_GUI.h" //Core graphics library
#include "LCDWIKI_KBV.h" //Hardware-specific library
#include "TinyGPS++.h"

extern TinyGPSPlus gps;

extern LCDWIKI_KBV  lcd;

extern char Version[];
extern char VersionDate[];

// ********** Start Telemetry values *****************************************************************

//message: LPO
// LPO,V,I,WV,WI
extern float V_Ch1Voltage;
extern float V_Ch1Current;
extern float V_Ch2Voltage;
extern float V_Ch2Current;
extern float V_Ch3Voltage;
extern float V_Ch3Current;

//message: LNA
extern int CTE;
extern long DTW;
extern int BTW;
extern int CDA;
extern float Currentloc_lat;
extern float Currentloc_lng;
extern int COG;
extern float SOG_mps;
extern int HDG;

//message: LAT
//extern int HDG;
extern int pitch;
extern int roll;
extern int ROLL_Avg;
extern float VMG;

//message: LWP
extern float prev_WP_lat;
extern float prev_WP_lng;
extern float next_WP_lat;
extern float next_WP_lng;
extern int MaxCTE;

// message: LMI
extern int mission_index;
extern int mission_size;
extern int MI_cmd;
extern int MI_duration;
extern int MI_SteerAWA;
extern int MI_TrimTabAngle;

// message:  LWI
extern int AWA;
extern int TWD;
extern float TWS;
extern int WingSail_Angle;
extern int WingSail_TrimTabAngle;

// message: LVS
extern int CommandState;
extern char CommandStateDescription[20];

// message: VER
extern char VoyagerOS_Version[20];
extern char VoyagerOS_Date[20];

// message:TMG
extern int year;
extern byte month;
extern byte dayOfMonth;
extern byte hour;
extern byte minute;
extern byte second;

//message: EQG
extern char SDCardStatus[20];
extern char WingSailAngleSensorStatus[20];

// message: prg
extern char ParameterValueString[20];
extern int ParameterValueInt;
extern char ParameterDescription[25];

//message: HOM
extern float Home_lat;
extern float Home_lng;
extern char Home_set[10];
extern long DTH;
extern long BTH;

// ********** End Telemetry values *****************************************************************
extern char VoyagerSail_Version[20];
extern char VoyagerSail_Version_Date[20];

//message: POW
extern float W_Ch1Voltage;
extern float W_Ch1Current;
extern float W_Ch2Voltage;
extern float W_Ch2Current;
extern float W_Ch3Voltage;
extern float W_Ch3Current;


extern int Wing_Servo_us_Send;
extern int Wing_Servo_us_Response;

//extern bool ViaRelay;
//extern char NextDestination;
//extern String WakeupPrefix;

extern unsigned int MaxPageNumber;
extern volatile unsigned int DisplayIndex;
extern volatile unsigned int DisplayIndex2;
extern volatile int ValueIndex;
extern volatile int ValueIndex2;
extern int InitValueIndex2;

extern EncoderModeType EncoderMode;
extern DisplayModeType DisplayMode;
extern unsigned int MaxVoyagerOLEDScreens;

extern float busvoltage;
extern float current_mA;

//define some colour values
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

extern bool GPSEnabled;
extern Location myloc;
extern double mySOG;
extern double myCOG;
extern int Range;
extern int Bearing;


String DisplayLine1;

void SetupPage(int DisplayIndex)
{
	//lcd.Fill_Screen(BLACK);
	lcd.Set_Text_colour(WHITE);
	lcd.Set_Text_Back_colour(BLACK);
	lcd.Set_Text_Size(2);
	lcd.Set_Text_Mode(0); // no overlapp
	lcd.setCursor(0, 0);
	lcd.print("P");
	lcd.print(DisplayIndex);
	lcd.print("/");
	lcd.print(MaxPageNumber);

	lcd.setCursor(6, 0);
}

void DisplayPage(int DisplayIndex)
{
	// display the relevant LCD page as per the current value of DisplayIndex (or DisplayModeType)

	char MsgString[16];

	switch (DisplayIndex)
	{
	case dmStart:
		SetupPage(DisplayIndex);
		lcd.println();
		lcd.Set_Text_Size(3);
		lcd.print(Version);// , 3, BLUE, BLACK, 0);
		lcd.println();
		lcd.println();
		lcd.print(VersionDate); // , 3, BLUE, BLACK, 0);

		lcd.Set_Text_Size(2);

		lcd.setCursor(0, 7);
		lcd.print("Battery ");
		lcd.print(dtostrf(busvoltage, 3, 2, MsgString) );
		lcd.print("Vdc ");
		lcd.print(dtostrf(current_mA, 3, 0, MsgString ));
		lcd.print("ma");

		lcd.setCursor(0, 14);
		lcd.print("SERIAL_RX_BUFFER_SIZE ");
		lcd.print(SERIAL_RX_BUFFER_SIZE);

		break;

	case dmLPO:  // LPO
		SetupPage(DisplayIndex);

		lcd.print(F(" Voyager Power LPO"));

		lcd.setCursor(0, 1);
		lcd.print(F("Ch1 Sol "));
		lcd.print(dtostrf(V_Ch1Voltage, 3, 2, MsgString));
		lcd.print("V ");
		lcd.print(dtostrf(V_Ch2Current, 3, 0, MsgString));
		lcd.print("mA");

		lcd.setCursor(0, 2);
		lcd.print(F("Ch2 Chg "));
		lcd.print(dtostrf(V_Ch2Voltage, 3, 2, MsgString));
		lcd.print("V ");
		lcd.print(dtostrf(V_Ch2Current, 3, 0, MsgString));
		lcd.print("mA");

		lcd.setCursor(0, 3);
		lcd.print(F("Ch3 Dis "));
		lcd.print(dtostrf(V_Ch3Voltage, 3, 2, MsgString));
		lcd.print("V ");
		lcd.print(dtostrf(V_Ch3Current, 3, 0, MsgString));
		lcd.print("mA");

		break;

	case dmLNA: // LNA
		SetupPage(DisplayIndex);

		lcd.print(F(" Voyager Navigation LNA "));

		lcd.setCursor(0, 1);
		DisplayLine1 = "HDG:" + String(HDG) + "  " + " COG:" + String(COG) + "  ";
		lcd.print(DisplayLine1);

		lcd.setCursor(0, 2);
		DisplayLine1 = "BTW:" + String(BTW) + "  " + " DTW:" + String(DTW) + "  ";
		lcd.print(DisplayLine1);

		lcd.setCursor(0, 3);
		DisplayLine1 = "CTE:" + String(CTE) + "  " + " SOG:" + String(SOG_mps) + " m/s  " + String(SOG_mps * 1.94) + " knots  ";
		lcd.print(DisplayLine1);

		lcd.setCursor(0, 4);
		DisplayLine1 =  String(dtostrf(Currentloc_lat, 8, 5, MsgString)) +  "  " + String(dtostrf(Currentloc_lng, 8, 5, MsgString)) + "  ";
		lcd.print(DisplayLine1);

		break;


	case dmLAT: //LAT
		SetupPage(DisplayIndex);
		lcd.print(F("  Voyager Attitude LAT"));

		lcd.setCursor(0, 1);
		DisplayLine1 = "Hdg:" + String(HDG) + "  " + " Pitch:" + String(pitch) + "  ";
		lcd.print(DisplayLine1);

		lcd.setCursor(0, 2);
		DisplayLine1 = "Roll:" + String(roll) + "   " + " Roll_Avg:" + String(ROLL_Avg) + "  ";
		lcd.print(DisplayLine1);

		lcd.setCursor(0, 3);
		DisplayLine1 = "VMG:" + String(VMG) + "  ";
		lcd.print(DisplayLine1);
		break;

	case dmLWP: // LWP
		SetupPage(DisplayIndex);
		lcd.print(F("  Voyager Waypoint LWP Prev/Next"));

		lcd.setCursor(0, 1);
		lcd.print(dtostrf(prev_WP_lat, 8, 5, MsgString));
		lcd.print(" ");
		lcd.print(dtostrf(prev_WP_lng, 8, 5, MsgString));

		lcd.setCursor(0, 2);
		lcd.print(dtostrf(next_WP_lat, 8, 5, MsgString));
		lcd.print(" ");
		lcd.print(dtostrf(next_WP_lng, 8, 5, MsgString));

		lcd.setCursor(0, 4);
		lcd.print(F("MaxCTE:"));
		lcd.print(MaxCTE);
		lcd.print("  ");
		break;

	case dmLMI: // LMI
		SetupPage(DisplayIndex);
		lcd.print(F("  Voyager Mission LMI"));

		lcd.setCursor(0, 1);
		lcd.print("MI:");
		lcd.print(mission_index);
		lcd.print("/");
		lcd.print(mission_size);

		lcd.print(" - ");
		lcd.print(MI_cmd);
		lcd.print(" ");

		switch (MI_cmd)
		{
		case MissionCommandType::ctGotoWaypoint:
			lcd.print("WP");
			break;

		case MissionCommandType::ctLoiter:
			lcd.print("Lt");
			break;

		case MissionCommandType::ctLoiterUntil:
			lcd.print("LU");
			break;

		case MissionCommandType::ctReturnToHome:
			lcd.print("Hm");
			break;

		case MissionCommandType::ctSteerWindCourse:
			lcd.print("SW");
			break;

		default:
			lcd.print("-");
		}

		lcd.setCursor(0, 2);
		lcd.print(F("Duration:"));
		lcd.print(MI_duration);
		lcd.print("  ");

		lcd.setCursor(0, 3);
		lcd.print(F("SteerAWA:"));
		lcd.print(MI_SteerAWA);
		lcd.print(F(" Tab:"));
		lcd.print(MI_TrimTabAngle);
		lcd.print("  ");
		break;

	case dmLWI: //LWI
		SetupPage(DisplayIndex);
		lcd.print(F("  Voyager Wind LWI"));

		lcd.setCursor(0, 1);
		DisplayLine1 = "AWA:" + String(AWA) + " TWD:" + String(TWD) + "  ";
		lcd.print(DisplayLine1);

		lcd.setCursor(0, 2);
		DisplayLine1 = "TWS:" + String(TWS) + "  ";
		lcd.print(DisplayLine1);

		lcd.setCursor(0, 4);
		DisplayLine1 = "Sail:" + String(WingSail_Angle) + " Tab:" + String(WingSail_TrimTabAngle) + "  ";
		lcd.print(DisplayLine1);
		break;

	case dmLVS: //LVS
		SetupPage(DisplayIndex);
		lcd.print(F("  Voyager Vessel State LVS"));
		lcd.setCursor(0, 1);
		lcd.print(F("CommandState:"));
		lcd.print(CommandState);

		lcd.setCursor(0, 2);
		lcd.print(CommandStateDescription);

		lcd.setCursor(0, 6);
		lcd.print(F("Press SEND to Loiter"));
		break;

	case dmVER: //VER
		SetupPage(DisplayIndex);
		lcd.print(F("  Voyager VER"));
		lcd.setCursor(0, 2);
		lcd.print(VoyagerOS_Version);
		lcd.setCursor(0, 3);
		lcd.print(VoyagerOS_Date);
		break;

	case dmTMG: // TMG
		SetupPage(DisplayIndex);
		lcd.print(F("  Voyager Time TMG"));

		lcd.setCursor(0, 2);

		DisplayLine1 = String(dayOfMonth) + "/" + String(month) + "/" + String(year) + "  "
			+ String(hour) + ":" + String(minute) + ":" + String(second);
		lcd.print(DisplayLine1);
		break;

	case dmEQG: // eqg
		SetupPage(DisplayIndex);
		lcd.print(F("  Voyager Equipment EQG"));
		lcd.setCursor(0, 2);
		lcd.print(SDCardStatus);
		lcd.setCursor(0, 4);
		lcd.print(WingSailAngleSensorStatus);
		break;

	case dmParameters: // Parameters
		SetupPage(DisplayIndex);
		lcd.print(F("  Voyager Parameters"));
		lcd.setCursor(0, 1);

		lcd.print(F("Param: "));
		lcd.print(DisplayIndex2);
		lcd.print(F("  "));

		lcd.setCursor(0, 2);
		lcd.print(ParameterDescription);
		lcd.print(F("     "));

		lcd.setCursor(0, 3);
		lcd.print(F("Val: "));
		lcd.print(ParameterValueString);
		lcd.print(F("  "));

		lcd.setCursor(14, 3);
		lcd.print(ParameterValueInt + ValueIndex);
		lcd.print("  ");

		lcd.setCursor(0, 6);
		lcd.print(F("  Press Send to apply value."));
		lcd.setCursor(0, 7);
		lcd.print(F("  Press Get to retreive value."));
		switch (EncoderMode)
		{
		case EncoderModeType::emPageMode:
			lcd.setCursor(2, 0); 
			break;

		case EncoderModeType::emParameterIndex:
			lcd.setCursor(7, 1);  // set the cursor on the parameter index to indicate that the rotary encoder will cause it to change.
			break;

		case EncoderModeType::emValue:
			lcd.setCursor(14, 3); // set the cursor on the parameter value to indicate that the rotary encoder will cause it to change.
			break;

		default:;
		}
		break;


	case dmConfig:
		SetupPage(DisplayIndex);
		lcd.print(F("  Configuration     "));
		lcd.setCursor(0, 2);
		lcd.print(F("  Save Current Configuration (set/up)"));

		lcd.setCursor(0, 5);
		lcd.print(F("  Set Default Configuration  (get/down)"));

		break;


	case dmResetMissionIndex:
		SetupPage(DisplayIndex);
		lcd.print(F("  Voyager Reset Mission"));

		lcd.setCursor(0, 1);
		lcd.print(F("Index "));
		lcd.print("MI:");
		lcd.print(mission_index);
		lcd.print("/");
		lcd.print(mission_size);

		lcd.setCursor(0, 6);
		lcd.print(F("   Press SEND to reset"));
		break;


	case dmSteerCompassCourse:
		SetupPage(DisplayIndex);
		lcd.print(F("  Voyager Steer Compass"));

		lcd.setCursor(0, 1);
		lcd.print(F("Hdg: "));

		lcd.setCursor(5, 1);
		DisplayLine1 = String(HDG) + "  ";
		lcd.print(DisplayLine1);

		lcd.setCursor(0, 2);
		lcd.print(F("Set Course: "));

		lcd.setCursor(13, 2);   // set the cursor again
		DisplayLine1 = String(wrap_360(ValueIndex * 5)) +  "  ";
		lcd.print(DisplayLine1);

		lcd.setCursor(0, 6);
		lcd.print(F("  Press Send"));

		switch (EncoderMode)
		{
		case EncoderModeType::emPageMode:
			lcd.setCursor(2, 0);
			break;

		case EncoderModeType::emValue:
			lcd.setCursor(15, 2); // set the cursor on the parameter value to indicate that the rotary encoder will cause it to change.
			break;

		default:;
		}
		break;

	case dmSteerWindAngle:
		SetupPage(DisplayIndex);
		lcd.print(F("  Voyager Steer Wind"));

		// limit the combined Value to  0 to 30
		if ((InitValueIndex2 + ValueIndex2) < 0)  ValueIndex2 =  0 - InitValueIndex2;
	    if ((InitValueIndex2 + ValueIndex2) > 30) ValueIndex2 = 30 - InitValueIndex2;

		lcd.setCursor(0, 1);
		lcd.print(F("AWA:"));
		lcd.print(AWA);
		lcd.print(" ");

		lcd.setCursor(10, 1);
		lcd.print(F("Set: "));
		lcd.print((int)wrap_180(ValueIndex));
		lcd.print("  ");

		// display P/S or blank
		lcd.setCursor(19, 1);
		if (wrap_180(ValueIndex) > 0)
			lcd.print("S");

		if (wrap_180(ValueIndex) < 0)
			lcd.print("P");

		if (wrap_180(ValueIndex) == 0)
			lcd.print(" ");

		lcd.setCursor(0, 2);
		lcd.print(F("Tab:"));
		lcd.print(WingSail_TrimTabAngle);
		lcd.print(" ");
		lcd.setCursor(10, 2);
		lcd.print(F("Set: "));
		lcd.print(InitValueIndex2+ValueIndex2);
		lcd.print(" ");

		lcd.setCursor(0, 3);
		lcd.print(F("SOG:"));
		lcd.print(SOG_mps);
		lcd.print(" ");
		lcd.print(F("VMG:"));
		lcd.print(VMG);
		lcd.print(" ");

		lcd.setCursor(0, 6);
		lcd.print(F("  Press Send"));

		switch (EncoderMode)
		{
		case EncoderModeType::emPageMode:
			lcd.setCursor(2, 0);
			break;

		case EncoderModeType::emValue:
			lcd.setCursor(18, 1); // set the cursor on the parameter value to indicate that the rotary encoder will cause it to change.
			break;

		case EncoderModeType::emValue2:
			lcd.setCursor(18, 2); // set the cursor on the parameter value to indicate that the rotary encoder will cause it to change.
			break;

		default:;
		}
		break;

	case dmHome: // Set Home
		SetupPage(DisplayIndex);
		lcd.print(F("  Voyager Home"));

		lcd.setCursor(0, 2);
		lcd.print(dtostrf(Home_lat, 8, 5, MsgString));
		lcd.print(" ");

		lcd.setCursor(10, 2);
		lcd.print(dtostrf(Home_lng, 8, 5, MsgString));
		lcd.print(" ");

		lcd.setCursor(0, 3);
		lcd.print(F("BTH:"));
		lcd.print(BTH);

		lcd.setCursor(10, 3);
		lcd.print(F("DTH:"));
		lcd.print(DTH);

		lcd.setCursor(4, 4);
		lcd.print(Home_set);
		lcd.print("  ");

		lcd.setCursor(0, 9);
		lcd.print(F("Press SEND to set Home at "));
		lcd.setCursor(0, 10);
		lcd.print(F("current location"));
		break;

	case dmDisplay: // 
		SetupPage(DisplayIndex);
		lcd.print(F(" Voyager OLED Display"));
		lcd.setCursor(0, 1);

		if (ValueIndex < 0) ValueIndex = 0;
		if (ValueIndex > MaxVoyagerOLEDScreens) ValueIndex = MaxVoyagerOLEDScreens;

		lcd.setCursor(0, 3);
		lcd.print(F(" Display Page: "));

		lcd.setCursor(14, 3);
		DisplayLine1 = String(ValueIndex) + " " + GetVoyagerDisplayDetails(DisplayDetailType::ddDescription, ValueIndex) + "       ";
		lcd.print(DisplayLine1);

		lcd.setCursor(16, 4);
		lcd.print("(");
		lcd.print(GetVoyagerDisplayDetails(DisplayDetailType::ddMessage, ValueIndex));
		lcd.print(")");

		lcd.setCursor(0, 8);
		lcd.print(F("  Press Send to apply value"));

		switch (EncoderMode)
		{
		case EncoderModeType::emPageMode:
			lcd.setCursor(1, 0);
			break;

		case EncoderModeType::emValue:
			lcd.setCursor(16, 3); // set the cursor on the parameter value to indicate that the rotary encoder will cause it to change.
			break;

		default:;
		}
		break;

	case dmWingSail:
			SetupPage(DisplayIndex);
			lcd.print(F(" Wingsail Power"));
			lcd.setCursor(0, 1);
			lcd.print(F("Battery "));
			lcd.print(dtostrf(W_Ch1Voltage, 3, 2, MsgString));
			lcd.print("V ");
			lcd.print(dtostrf(W_Ch1Current, 4, 1, MsgString));
			lcd.print("mA      ");

			lcd.setCursor(0, 2);
			lcd.print("Last response: ");
			lcd.print(Wing_Servo_us_Response);
			lcd.print("s      ");

			lcd.setCursor(0, 3);

		break;

	case dmLocalGPS:
		SetupPage(DisplayIndex);
		lcd.print(F("  Voyager Range and Bearing"));

		GPSEnabled = true;

		lcd.setCursor(0, 1);
		DisplayLine1 = "Voyager Location:";
		lcd.print(DisplayLine1);

		lcd.setCursor(0, 2);
		DisplayLine1 = String(dtostrf(Currentloc_lat, 8, 5, MsgString)) + "  " + String(dtostrf(Currentloc_lng, 8, 5, MsgString)) + "  ";
		lcd.print(DisplayLine1);

		lcd.setCursor(0, 4);
		DisplayLine1 = "This Location:";
		lcd.print(DisplayLine1);
		lcd.setCursor(0, 5);
		DisplayLine1 = String(dtostrf(float(myloc.lat)/10000000UL, 8, 5, MsgString)) + "  " + String(dtostrf(float(myloc.lng)/10000000UL, 8, 5, MsgString)) + "  ";
		lcd.print(DisplayLine1);


		lcd.setCursor(0, 7);
		DisplayLine1 = "Range and Bearing:";
		lcd.print(DisplayLine1);
		lcd.setCursor(0, 8);
		DisplayLine1 = String(Range) + "m " + String(Bearing) + " deg T" + "                  ";
		lcd.print(DisplayLine1);

		lcd.setCursor(0, 10);
		lcd.print("SOG ");
		lcd.print(mySOG);
		lcd.print("m/s COG ");
		lcd.print(myCOG);
		lcd.print(" deg T   ");

		lcd.setCursor(0, 11);
		DisplayLine1 = "Satellites: ";
		lcd.print(DisplayLine1);
		DisplayLine1 = String(gps.satellites.value());
		lcd.print(DisplayLine1);
		DisplayLine1 = "  HDOP: ";
		lcd.print(DisplayLine1);
		lcd.print(gps.hdop.value());
		lcd.print("  ");


	//	lcd.setCursor(0, 11);
	//	lcd.print(gps.time.second());
	//	lcd.print("  ");

		break;


	//case dmWingVersion: //VER
	//	SetupPage(DisplayIndex);
	//	lcd.print(F(" Wingsail Version"));
	//	lcd.setCursor(0, 1);
	//	lcd.print(VoyagerSail_Version);
	//	lcd.setCursor(0, 2);
	//	lcd.print(VoyagerSail_Version_Date);
	//	break;

	//case dmWingPower:  // POW
	//	SetupPage(DisplayIndex);
	//	lcd.print(F(" Wingsail Power"));
	//	lcd.setCursor(0, 1);
	//	lcd.print(F("Ch1 Sol "));
	//	lcd.print(dtostrf(W_Ch1Voltage, 3, 2, MsgString));
	//	lcd.print("V ");
	//	lcd.print(dtostrf(W_Ch2Current, 3, 0, MsgString));
	//	lcd.print("mA");

	//	lcd.setCursor(0, 2);
	//	lcd.print(F("Ch2 Chg "));
	//	lcd.print(dtostrf(W_Ch2Voltage, 3, 2, MsgString));
	//	lcd.print("V ");
	//	lcd.print(dtostrf(W_Ch2Current, 3, 0, MsgString));
	//	lcd.print("mA");

	//	lcd.setCursor(0, 3);
	//	lcd.print(F("Ch3 Dis "));
	//	lcd.print(dtostrf(W_Ch3Voltage, 3, 2, MsgString));
	//	lcd.print("V ");
	//	lcd.print(dtostrf(W_Ch3Current, 3, 0, MsgString));
	//	lcd.print("mA");
	//	break;

	//case dmWingServo: // SVO
	//	SetupPage(DisplayIndex);
	//	// don't let the value exceed normal limits for Servo of 1000 to 2000
	//	if (ValueIndex > 25) ValueIndex = 25;
	//	if (ValueIndex < -25) ValueIndex = -25;

	//	Wing_Servo_us_Send = ValueIndex * 20 + 1500;

	//	lcd.print(F(" Wingsail Servo"));
	//	lcd.setCursor(0, 1);
	//	lcd.print(F("Servo "));

	//	lcd.setCursor(10, 1);
	//	lcd.print(Wing_Servo_us_Send);
	//	lcd.print(" us");

	//	lcd.setCursor(0, 2);
	//	lcd.print(F("Response: "));
	//	lcd.print(Wing_Servo_us_Response);
	//	lcd.print(" us");

	//	lcd.setCursor(0, 6);
	//	lcd.print(F("Press SEND or QUERY"));

	//	switch (EncoderMode)
	//	{
	//	case EncoderModeType::emPageMode:
	//		lcd.setCursor(1, 0);
	//		break;

	//	case EncoderModeType::emValue:
	//		lcd.setCursor(10, 1); // set the cursor on the parameter value to indicate that the rotary encoder will cause it to change.
	//		break;

	//	default:;
	//	}
	//	break;

	case dmLoRaTerminal:  // LoRa Terminal
		SetupPage(DisplayIndex);
		lcd.print(F(" LoRa Monitor"));
		break;

	default:;
	}
}



void CursorUpdateLoop(void*)
{
	static bool CursorOn;

	if (CursorOn)
	{
		switch (DisplayIndex)
		{

		//case dmWingServo: // SVO
		//			// set cursor based on Wing servo page and encoder mode
		//	switch (EncoderMode)
		//	{
		//	case EncoderModeType::emPageMode:
		//		lcd.SetBlockCursor(6, 0);
		//		break;

		//	case EncoderModeType::emValue:
		//		lcd.SetBlockCursor(18, 1);
		//		break;

		//	default:;
		//	}
		//	break;


		case dmParameters: // Parameters
			switch (EncoderMode)
			{
			case EncoderModeType::emPageMode:
				lcd.SetBlockCursor(6, 0);
				break;

			case EncoderModeType::emParameterIndex:
				lcd.SetBlockCursor(9, 1);
				break;

			case EncoderModeType::emValue:
				lcd.SetBlockCursor(16, 3);
				break;

			default:;
			}
			break;

		case dmSteerCompassCourse:
			switch (EncoderMode)
			{
			case EncoderModeType::emPageMode:
				lcd.SetBlockCursor(6, 0);
				break;

			case EncoderModeType::emValue:
				lcd.SetBlockCursor(16, 2);
				break;

			default:;
			}
			break;

		case dmSteerWindAngle:
			switch (EncoderMode)
			{
			case EncoderModeType::emPageMode:
				lcd.SetBlockCursor(6, 0);
				break;

			case EncoderModeType::emValue:
				lcd.SetBlockCursor(21, 1);
				break;

			case EncoderModeType::emValue2:
				lcd.SetBlockCursor(18, 2);
				break;

			default:;
			}
			break;

		case dmDisplay: // LCD Display
		// set cursor based on Wing servo page and encoder mode
			switch (EncoderMode)
			{
			case EncoderModeType::emPageMode:
				lcd.SetBlockCursor(6, 0);
				break;

			case EncoderModeType::emValue:
				lcd.SetBlockCursor(12, 3);
				break;

			default:;
			}
			break;

		default:;
		};

	}
	else
	{
		lcd.SetBlockCursor(-1, -1);
	}

	// toggle Cursor status
	CursorOn = !CursorOn;
}

String GetVoyagerDisplayDetails(DisplayDetailType DetailType, int ValueIndex)
{
	String Description;
	String Message;

	switch (ValueIndex)
	{
	case 0:
		Description = "Mission     ";
		Message = "m";
		break;

	case 1:
		Description = "Version   ";
		Message = "v";
		break;

	case 2:
		Description = "Equipment   ";
		Message = "e";
		break;

	case 3:
		Description = "GPS       ";
		Message = "g";
		break;

	case 4:
		Description = "GPS Detail  ";
		Message = "p";
		break;

	case 5:
		Description = "Attitude    ";
		Message = "a";
		break;

	case 6:
		Description = "Wing Sensor   ";
		Message = "y";
		break;

	case 7:
		Description = "Wing Sail      ";
		Message = "x";
		break;

	case 8:
		Description = "Mission Timing  ";
		Message = "t";
		break;

	case 9:
		Description = "Mission Steps  ";
		Message = "o";
		break;

	case 10:
		Description = "Mission     ";
		Message = "m";
		break;

	case 11:
		Description = "Situation   ";
		Message = "s";
		break;

	case 12:
		Description = "Navigation 1  ";
		Message = "n";
		break;

	case 13:
		Description = "Navigation 2  ";
		Message = "i";
		break;

	case 14:
		Description = "Sail Nav 1  ";
		Message = "U";
		break;

	case 15:
		Description = "Sail Nav 2  ";
		Message = "q";
		break;

	case 16:
		Description = "Sailable   ";
		Message = "1";
		break;

	case 17:
		Description = "TWD Calc     ";
		Message = "2";
		break;

	case 18:
		Description = "Fav Tack Calc   ";
		Message = "3";
		break;

	case 19:
		Description = "Waypoints     ";
		Message = "w";
	break;

	case 20:
		Description = "Wind        ";
		Message = "k";
		break;

	case 21:
		Description = "Compass Dtl   ";
		Message = "c";
		break;

	case 22:
		Description = "Power       ";
		Message = "z";
		break;

	case 23:
		Description = "Home        ";
		Message = "h";
		break;

	case 24:
		Description = "Wear Stats    ";
		Message = "j";
		break;

	case 25:
		Description = "Loiter    ";
		Message = "l";
		break;

	case 26:
		Description = "Sim Data   ";
		Message = "4";
		break;

	case 27:
		Description = "Steering Data   ";
		Message = "5";
		break;

	case 28:
		Description = "Wingsail Monitor  ";
		Message = "6";
		break;

	case 29:
		Description = "Wingsail Power  ";
		Message = "7";
		break;

	case 30:
		Description = "Wave Measurement  ";
		Message = "8";
		break;
	case 31:
		Description = "SD Card  ";
		Message = "9";
		break;
	case 32:
		Description = "Wingsail Ver/Pwr  ";
		Message = "W";
		break;

	case 33:
		Description = "GPS Power Mode    ";
		Message = "L";
		break;

	case 34:
		Description = "Bluetooth    ";
		Message = "B";
		break;

	case 35:
		Description = "Environment    ";
		Message = "E";
		break;

	case 36:
		Description = "Blank   ";
		Message = "0";
		break;
		// change  MaxVoyagerOLEDScreens to match 
	default:;
	}

	// return either the Description or the message
	return ( (DetailType == DisplayDetailType::ddDescription) ? Description : Message);
}

