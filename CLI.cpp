// 
// 
// 

#include "CLI.h"
#include "DisplayPages.h"
#include "LoRaMonitor.h"

extern volatile unsigned int DisplayIndex;

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

extern int InitValueIndex2;
extern DisplayModeType DisplayMode;

// ********** End Telemetry values *****************************************************************

extern int Wing_Servo_us_Send;
extern int Wing_Servo_us_Response;
extern char VoyagerSail_Version[20];
extern char VoyagerSail_Version_Date[20];
//message: LWS
extern float W_Ch1Voltage;
extern float W_Ch1Current;
extern float W_Ch2Voltage;
extern float W_Ch2Current;
extern float W_Ch3Voltage;
extern float W_Ch3Current;

// Command Line Interpreter - Global Variables
char CLI_Msg[70];
int CLI_i = 0;

// Collect the characters into a command string until end end of line,
// and then process it.
// V1.0 22/12/2015
void CLI_Process_Message(void)
{
	// Accumulate characters in a command string up to a CR or LF or buffer fills. 
	while (Serial1.available())
	{
		char received = Serial1.read();
		CLI_Msg[CLI_i++] = received;

		// Process message when new line character is received
		if (received == '\n' || received == '\r' || CLI_i == (sizeof(CLI_Msg) - 3))
		{
			CLI_Msg[CLI_i] = '\0';
			CLI_i = 0;

			if (DisplayIndex == DisplayModeType::dmLoRaTerminal)
			{
				if (strlen(CLI_Msg) > 1) // only add to the LoRa Montitor if the message has length
				{
					Serial.println(CLI_Msg);
					LoRaMonitorAdd(CLI_Msg);
				}
			}
			else
			{
				if (strlen(CLI_Msg)>3) // only call the processor if the command has length
						CLI_Processor();
			}
		}
	}
}

// Process the Command String.
// Split into command and parameters separated by commas. 
// V1.0 22/12/2015
void CLI_Processor(void)
{
	//char Fullcmd[8] = "";
	char cmd[8] = "";
	char param1[20] = "";
	char param2[20] = "";
	char param3[15] = "";
	char param4[15] = "";
	char param5[15] = "";
	char param6[15] = "";
	char param7[15] = "";
	char param8[15] = "";
	char param9[15] = "";

	strcat(CLI_Msg, ",,");

	// Split into command and parameters separated by commas. 
	strncpy(cmd, strtok(CLI_Msg, ","), sizeof(cmd) - 1);
	strncpy(param1, strtok(NULL, ","), sizeof(param1) - 1);
	strncpy(param2, strtok(NULL, ","), sizeof(param2) - 1);
	strncpy(param3, strtok(NULL, ","), sizeof(param3) - 1);
	strncpy(param4, strtok(NULL, ","), sizeof(param4) - 1);
	strncpy(param5, strtok(NULL, ","), sizeof(param5) - 1);
	strncpy(param6, strtok(NULL, ","), sizeof(param6) - 1);
	strncpy(param7, strtok(NULL, ","), sizeof(param7) - 1);
	strncpy(param8, strtok(NULL, ","), sizeof(param8) - 1);
	strncpy(param9, strtok(NULL, ","), sizeof(param9) - 1);

	//char NextAddr = Fullcmd[0];
	//char FinalAddr = Fullcmd[1];
	//char PrevAddr = Fullcmd[2];
	//char SourceAddr = Fullcmd[3];
	//cmd[0] = Fullcmd[4];
	//cmd[1] = Fullcmd[5];
	//cmd[2] = Fullcmd[6];
	//cmd[3] = '\0';

	//Serial.println(Fullcmd);
	Serial.println(cmd);

	// ===============================================
	// Command ech: lpo
	// ===============================================
	if (!strncmp(cmd, "lpo", 3))
	{
		V_Ch1Voltage = atof(param1);
		V_Ch1Current = atof(param2);
		V_Ch2Voltage = atof(param3);
		V_Ch2Current = atof(param4);
		V_Ch3Voltage = atof(param5);
		V_Ch3Current = atof(param6);
	}

	if (!strncmp(cmd, "lna", 3))
	{
		CTE = atoi(param1);
		DTW = atol(param2);
		BTW = atoi(param3);
		CDA = atoi(param4);
		Currentloc_lat = atof(param5);
		Currentloc_lng = atof(param6);
		COG = atoi(param7);
		SOG_mps = atof(param8);
		HDG = atoi(param9);
	}

	if (!strncmp(cmd, "lat", 3))
	{
		HDG = atoi(param1);
		pitch = atoi(param2);
		roll = atol(param3);
		ROLL_Avg = atoi(param4);
		VMG = atof(param5);
	}

	if (!strncmp(cmd, "lvs", 3))
	{
		CommandState = atoi(param1);

		// look for a CR or NULL and use a size to copy.
		int i = 0;
		char ch = param2[i];
		while ((ch != '\0') && (ch != '\r'))
		{
			i++;
			ch = param2[i];
		}
		strncpy(CommandStateDescription , param2,i);
		CommandStateDescription[i] = '\0';
	}

	if (!strncmp(cmd, "lpf", 3))
	{
		AWA = atoi(param1);
		SOG_mps = atof(param2);
		VMG = atof(param3);
		WingSail_TrimTabAngle = atoi(param4);
		InitValueIndex2 = WingSail_TrimTabAngle;
	}

	if (!strncmp(cmd, "lwp", 3))
	{
		prev_WP_lat = atof(param1);
		prev_WP_lng = atof(param2);
		next_WP_lat = atof(param3);
		next_WP_lng = atof(param4);
		MaxCTE = atoi(param5);
	}

	if (!strncmp(cmd, "lmi", 3))
	{
		mission_index = atoi(param1);
		mission_size = atoi(param2);
		MI_cmd = atoi(param3);
		MI_duration = atoi(param4);
		MI_SteerAWA = atoi(param5);
		MI_TrimTabAngle = atoi(param6);
	}

	if (!strncmp(cmd, "lwi", 3))
	{
		AWA = atoi(param1);
		TWD = atoi(param2);
		TWS = atof(param3);
		WingSail_Angle = atoi(param4);
		WingSail_TrimTabAngle = atoi(param5);
	}

	if (!strncmp(cmd, "ver", 3))
	{
		strncpy(VoyagerOS_Version, param1, sizeof(VoyagerOS_Version));
		strncpy(VoyagerOS_Date, param2, sizeof(VoyagerOS_Date));
	}


	if (!strncmp(cmd, "tim", 3))
	{
		 year = atoi(param1);
		 month = atoi(param2);
		 dayOfMonth = atoi(param3);
		 hour = atoi(param4);
		 minute = atoi(param5);
		 second = atoi(param6);
	}

	if (!strncmp(cmd, "eqg", 3))
	{
		strncpy(SDCardStatus , param1, sizeof(SDCardStatus));
		strncpy(WingSailAngleSensorStatus , param2, sizeof(WingSailAngleSensorStatus));
	}

	if (!strncmp(cmd, "prg", 3))
	{
		// look for a CR or NULL and use a size to copy.
		int i = 0;
		char ch = param2[i];
		while ((ch != '\0') && (ch != '\r'))
		{
			i++;
			ch = param2[i];
		}
		strncpy(ParameterDescription, param2, i);
		ParameterDescription[i] = '\0';

		// look for a CR or NULL and use a size to copy.
		 i = 0;
		 ch = param3[i];
		while ((ch != '\0') && (ch != '\r'))
		{
			i++;
			ch = param3[i];
		}
		strncpy(ParameterValueString, param3, i);
		ParameterValueString[i] = '\0';

		ParameterValueInt = atoi(param3);
	}

	if (!strncmp(cmd, "hlg", 3))
	{
		Home_lat = atof(param1);
		Home_lng = atof(param2);

		// look for a CR or NULL and use a size to copy.
		int i = 0;
		char ch = param3[i];
		while ((ch != '\0') && (ch != '\r'))
		{
			i++;
			ch = param3[i];
		}
		strncpy(Home_set, param3, i);
		Home_set[i] = '\0';

		DTH = atol(param4);
		BTH = atol(param5);
	}

	// Wingsail
	// pow - Power Query response
	if (!strncmp(cmd, "lws", 3))
	{
		W_Ch1Voltage = atof(param1);
		W_Ch1Current = atof(param2);
		Wing_Servo_us_Response = atof(param3);
	}

	//// Wingsail
	//// gsv - Get Servo response
	//if (!strncmp(cmd, "gsv", 3))
	//{
	//	Wing_Servo_us_Response = atoi(param1);
	//}

	//// Wingsail
	//// ssv Set Servo response
	//if (!strncmp(cmd, "ssv", 3))
	//{
	//	Wing_Servo_us_Response = atoi(param1);
	//}

	// Wingsail
	// ver - Version Query response
	if (!strncmp(cmd, "ver", 3))
	{
		strncpy(VoyagerSail_Version, param1, sizeof(VoyagerSail_Version));

		// look for a CR or NULL and use a size to copy.
		int i = 0;
		char ch = param2[i];
		while ((ch != '\0') && (ch != '\r'))
		{
			i++;
			ch = param2[i];
		}
		strncpy(VoyagerSail_Version_Date, param2, i);
		VoyagerSail_Version_Date[i] = '\0';
	}

}
