/*
    Name:       VoyagerTerminalTFT.ino
    Created:	30/06/2021 9:35:49 PM
    Author:     ACS\john
*/

// V2.02 20/7/2021
// V2.03 29/7/2021 adding Sail functions.
// V2.04 1/8/2021 added flashing cursor
// V2.05 3/8/2021 updated CLI to support full addressing for both wingsail and vessel
// V2.06 5/8/2021 General cleanup 
// V2.07 6/8/2021 General cleanup
// V2.08 7/8/2021 added Short command for Wingsail servo, and improve display refresh behaviour
// V2.09 12/8/2021 added page for setting OLED Display.
// V2.10 13/8/2021 Relay support. Remembered to add Wakeup while sending Wingsail
// V2.11 14/8/2021 Bug Fixes
// V2.12 19/8/2021 further adustments
// V2.13 20/8/2021 Fully functioning release
// V2.14 16/10/2021 minor updates during testing prior to sail.
// V2.15 17/10/2021 minor updates
// V2.16 21/10/2021 added to list of OLED Display pages: 6 Wingsail Monitor
// V2.17 23/10/2021 fix LoRa Monitor remove excess carriage returns.
// V2.18 3/11/2021 add Wave Measurement screen for OLED, fix OLED wrap problem.
// V2.19 7/11/2021 tidied up Parameter page and added Config Page - save and restore
// V2.20 8/11/2021 add anotations to screens
// V2.21 11/11/2021 fixed the home display.
// V2.22 4/12/2021 remove polling from Parameters screen
// V2.23 9/1/2022 Added Voyager's SD Card OLED Page

// V3.01 21/5/2022 commence removing Relay addressing from LoRa, associated with change to BT5.
// V3.02 25/5/2022
// V3.03 5/6/2022 updated to handle the bluetooth wingsail.
// V3.04 19/6/2022  added OLED Page L: GPS Power mode
// V3.05 22/9/2022  added OLED Page B: Bluetooth
// V3.06 22/9/2022 moved the level change function from the Encoder switch to a separate up/down toggle.
//					The Encoder switch wasn't a good user input device.
//					The toggle switch is now used to explicitly change level up and down and dows not loop around.
// V3.07 22/9/2022 add local gps page
// V3.08 1/10/2022 continue gps work
// V3.09 9/10/2022 add a Wingsail data page
// V3.10 14/01/2022 display the size of the SERIAL_RX_BUFFER_SIZE on home page.
// V3.11 14/01/2022 added mySOG myCOG to Range and Bearing Page.
// 
// Build Notes: modified HardwareSerial.h located in C:\Program Files (x86)\Arduino\hardware\arduino\avr\cores\arduino.
// increase Serial Rx Buffer size to 256.
// this is needed because of the amount of time required to get around and back. the messages are about 80 char long, so 64 overflows easily
// #define SERIAL_RX_BUFFER_SIZE 256 //64


char Version[] = "Voyager TFT Terminal";
char VersionDate[] = "V3.11 14/01/2022";

#include "LCDWIKI_GUI.h" //Core graphics library
#include "LCDWIKI_KBV.h" //Hardware-specific library

#include "Location.h"
#include "DisplayPages.h"
#include "CLI.h"
#include <Wire.h>
#include "Adafruit_INA219.h"
#include "LoRa_E32.h"
#include "SchedulerCooperative.h"
#include "LoRaMonitor.h"
#include "PollMessages.h"
#include "TinyGPS++.h"
#include "AP_Math.h"
#include "GPSHandler.h"

#define encoder0PinA  2  //CLK Output A Do not use other pin for clock as we are using interrupt
#define encoder0PinB  9  //DT Output B
#define encoder0Switch 8 // Switch connection if available

#define	ToggleUpPin		6
#define	ToggleDownPin	7

#define	LevelUpPin		11
#define	LevelDownPin	12

#define GPSEnablePin	54

bool prevToggleUp;
bool prevToggleDown;
bool ToggleUp;
bool ToggleDown;

volatile unsigned int DisplayIndex = 0;
unsigned int PrevDisplayIndex;
volatile int ValueIndex = 0;
int PrevValueIndex;
volatile unsigned int DisplayIndex2 = 0;
unsigned int PrevDisplayIndex2;

bool EncoderSwitch = HIGH;
bool PrevEncoderSwitch = HIGH;

bool LevelUpSwitch = HIGH;
bool PrevLevelUpSwitch = HIGH;
bool LevelDownSwitch = HIGH;
bool PrevLevelDownSwitch = HIGH;

unsigned int MaxPageNumber = DisplayModeType::dmLoRaTerminal;

unsigned int MaxIndexNumber = 52; // max parameter number

volatile int ValueIndex2 = 0;
int PrevValueIndex2;
int InitValueIndex2;

unsigned int MaxVoyagerOLEDScreens = 36; // **** Max OLED Screens ****

EncoderModeType EncoderMode;
DisplayModeType DisplayMode;

// Loop Timer Constants used by the scheduler
static const int SwitchMonitorLoopTime = 50;  //ms 
static const int PollLoopTime = 5000;  //ms 5 seconds 
static const int DisplayRefreshTime = 1000; // 1 second testing loop
static const int CursorUpdateTime = 500; // 500ms loop
static const int GPSReadTime = 100; //ms

// **** local GPS 
Location myloc;
double mySOG;
double myCOG;
bool GPSEnabled;
int Range;
int Bearing;

// ********** Start Telemetry values *****************************************************************
//message: LPO
float V_Ch1Voltage;
float V_Ch1Current;
float V_Ch2Voltage;
float V_Ch2Current;
float V_Ch3Voltage;
float V_Ch3Current;

//message: LNA
int CTE;
long DTW;
int BTW;
int CDA;
float Currentloc_lat;
float Currentloc_lng;
int COG;
float SOG_mps;
int HDG;

//message: LAT
int pitch;
int roll;
int ROLL_Avg;
float VMG;

//message: LWP
float prev_WP_lat;
float prev_WP_lng;
float next_WP_lat;
float next_WP_lng;
int MaxCTE;

// message: LMI
int mission_index;
int mission_size;
int MI_cmd;
int MI_duration;
int MI_SteerAWA;
int MI_TrimTabAngle;

// message:  LWI
int AWA;
int TWD;
float TWS;
int WingSail_Angle;
int WingSail_TrimTabAngle;

// message: LVS
int CommandState;
char CommandStateDescription[20];

// message: VER
char VoyagerOS_Version[20];
char VoyagerOS_Date[20];

// message:TMG
int year;
byte month;
byte dayOfMonth;
byte hour;
byte minute;
byte second;

//message: EQG
char SDCardStatus[20];
char WingSailAngleSensorStatus[20];

// message: prg
char ParameterValueString[20];
int ParameterValueInt;
char ParameterDescription[25];

//message: HOM
float Home_lat;
float Home_lng;
char Home_set[10];
long DTH;
long BTH;

// ********** End Telemetry values *****************************************************************

// Wing sail variables ********************************************************

// message: VER
char VoyagerSail_Version[20];
char VoyagerSail_Version_Date[20];

//message: POW
float W_Ch1Voltage;
float W_Ch1Current;
float W_Ch2Voltage;
float W_Ch2Current;
float W_Ch3Voltage;
float W_Ch3Current;

int Wing_Servo_us_Send;
int Wing_Servo_us_Response;

// ********** End Wing sail variables *****************************************************************

//LoRa_E32(HardwareSerial* serial, byte auxPin, byte m0Pin, byte m1Pin, UART_BPS_RATE bpsRate = UART_BPS_RATE_9600);
LoRa_E32 e32ttl100(&Serial1, 10, 3, 4); // Arduino RX <-- e32 TX, Arduino TX --> e32 RX

LCDWIKI_KBV lcd(ILI9486, 40, 38, 39, -1, 41); //model,cs,cd,wr,rd,reset

//define some colour values
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Adafruit_INA219 ina219;
float busvoltage = 0;
float current_mA = 0;

volatile boolean InInterrupt = false;

// The TinyGPS++ object
TinyGPSPlus gps;

void setup()
{
	SchedulerInit();

    lcd.Init_LCD();
    lcd.Fill_Screen(BLACK);
    lcd.Set_Rotation(3);
    lcd.Set_Text_Mode(0);
    lcd.Set_Text_Size(2);
    lcd.Set_Text_colour(GREEN);
    lcd.Set_Text_Back_colour(BLACK);

	Serial.begin(9600);

	pinMode(encoder0Switch, INPUT);
	digitalWrite(encoder0Switch, HIGH);       // turn on pullup resistor

	pinMode(encoder0PinB, INPUT);
	digitalWrite(encoder0PinB, HIGH);       // turn on pullup resistor

	attachInterrupt(0, doEncoder,RISING); // encoder pin on interrupt 0 - pin2

	pinMode(encoder0PinA, INPUT);
	digitalWrite(encoder0PinA, HIGH);       // turn on pullup resistor

	pinMode(LED_BUILTIN, OUTPUT);

	pinMode(ToggleDownPin, INPUT);
	digitalWrite(ToggleDownPin, HIGH);

	pinMode(ToggleUpPin, INPUT);
	digitalWrite(ToggleUpPin, HIGH);

	pinMode(LevelUpPin, INPUT);
	digitalWrite(LevelUpPin, HIGH);
	pinMode(LevelDownPin, INPUT);
	digitalWrite(LevelDownPin, HIGH);

	pinMode(GPSEnablePin, OUTPUT); // commence with GPS off
	digitalWrite(GPSEnablePin, LOW);
	GPSEnabled = false;

	// serial #2
	Serial2.begin(9600);

	// Startup all pins and UART
	e32ttl100.begin();

	ResponseStructContainer c;	c = e32ttl100.getConfiguration();
	Configuration configuration = *(Configuration*)c.data;

	configuration.ADDL = 0x01;
	configuration.ADDH = 0x00;
	configuration.CHAN = 23;

	configuration.OPTION.fec = FEC_1_ON;
	configuration.OPTION.fixedTransmission = FT_TRANSPARENT_TRANSMISSION;
	configuration.OPTION.ioDriveMode = IO_D_MODE_PUSH_PULLS_PULL_UPS;
	configuration.OPTION.transmissionPower = POWER_10;
	configuration.OPTION.wirelessWakeupTime = WAKE_UP_250;

	configuration.SPED.airDataRate = AIR_DATA_RATE_010_24;
	configuration.SPED.uartBaudRate = UART_BPS_9600;
	configuration.SPED.uartParity = MODE_00_8N1;
	ResponseStatus rs = e32ttl100.setConfiguration(configuration, WRITE_CFG_PWR_DWN_LOSE);

	e32ttl100.setMode(MODE_TYPE::MODE_1_WAKE_UP);
	//e32ttl100.setMode(MODE_TYPE::MODE_2_POWER_SAVING);

	DisplayPage(DisplayIndex);

	ina219.begin(); // I2C Address 0x40
	ina219.setCalibration_16V_400mA();
}

void loop()
{
	CLI_Process_Message();
	//	populate values if found

	//give the scheduler a chance to act
	SchedulerTick(0, &SwitchMonitorLoop, SwitchMonitorLoopTime);
	SchedulerTick(1, &PollLoop, PollLoopTime);
	SchedulerTick(2, &DisplayRefreshLoop, DisplayRefreshTime);
	SchedulerTick(3, &CursorUpdateLoop, CursorUpdateTime);
	SchedulerTick(4, &GPSReadLoop, GPSReadTime);


	// if there's a change in Display index, then act on it.
	if (PrevDisplayIndex != DisplayIndex)
	{
		PrevDisplayIndex = DisplayIndex;
		InitValueIndex2 = 0;

		// log on usb serial
		Serial.print("DisplayIndex: ");	Serial.println(DisplayIndex);

		lcd.Fill_Screen(BLACK); // clear the screen when changing page
		DisplayPage(DisplayIndex);

		// disable local gps if not on local gps page
		GPSEnabled = (DisplayIndex == dmLocalGPS);
		digitalWrite(GPSEnablePin, GPSEnabled);	
	}

	// Change of Displayed Parameter Number on the Parameters Page
	if (PrevDisplayIndex2 != DisplayIndex2)
	{
		PrevDisplayIndex2 = DisplayIndex2;
		strncpy(ParameterValueString, "", 1);
		ParameterValueInt = 0;
		strncpy(ParameterDescription, "", 1);
		ValueIndex = 0;
		DisplayPage(DisplayIndex);
	}

	// if there's a change in Value number, then update display
	if (PrevValueIndex != ValueIndex)
	{
		PrevValueIndex = ValueIndex;
		DisplayPage(DisplayIndex);
	}

	// if there's a change in Value2 number, then update display
	if (PrevValueIndex2 != ValueIndex2)
	{
		PrevValueIndex2 = ValueIndex2;
		DisplayPage(DisplayIndex);
	}

	// monitor the Toggle Switch. It has up and down.
	toggleMonitor();

	busvoltage = ina219.getBusVoltage_V();
	current_mA = ina219.getCurrent_mA();
}

void ToggleUpAction()
{
	// this is the handler for the Toggle UP Switch (or Send).
	// its action is controlled by the current page mode (or index).

	Serial.println("Toggle Up");

	switch (DisplayIndex)
	{
	case dmStart:
		//NextDestination = 'v';
		//WakeupPrefix = "";
		break;

	case dmParameters:
		// Send the current values for parameter index and value to the vessel.
		Serial1.print("prs,");
		Serial1.print(DisplayIndex2);
		Serial1.print(",");
		Serial1.println(ParameterValueInt + ValueIndex);

		ValueIndex = 0;
		// change rotary encode mode from parameter value back to parameter index number, after sending.
		EncoderMode = EncoderModeType::emParameterIndex;
		break;

	case dmLVS:
		Serial1.print("scs,");
		Serial1.println(csLoiter);  //Loiter Here
		break;

	case dmResetMissionIndex:
		// send the command to zero the mission
		Serial1.println("mis,0");

		delay(5000); // delay 5 seconds

		Serial1.print("scs,");
		Serial1.println(csFollowMission);  //follow mission
		break;

	case dmSteerCompassCourse:
		// send command to compass course
		Serial1.print("scs,");
		Serial1.print(csSteerMagneticCourse);  //compass course
		Serial1.print(",");
		Serial1.println(wrap_360(ValueIndex * 5)); // x 5 
		break;

	case dmSteerWindAngle:
		// send command to wind course
		Serial1.print("scs,");
		Serial1.print(csSteerWindCourse);  // wind course
		Serial1.print(",");
		Serial1.print(wrap_180(ValueIndex)); // course -/+180 degrees
		Serial1.print(",");
		Serial1.println(InitValueIndex2 + ValueIndex2); // Trim Tab Angle degrees

		ValueIndex2 = 0;
		break;

	case dmHome:
		// send the command to set home as the current position, with no offset.
		Serial1.println("hls,0,0");
		break;

	case dmDisplay:
		Serial1.print("lcd,"); 
		Serial1.print(GetVoyagerDisplayDetails(DisplayDetailType::ddMessage, ValueIndex));
		Serial1.println();
		break;

	case dmConfig: // toggle up action causes current config to be saved.
		Serial1.print("sav,c");
		Serial1.println();
		break;

	//case dmLocalGPS:
	//	while (Serial2.available() > 0) { Serial2.read(); } // clear buffer
	//	break;

	default:;
	}
}

void ToggleDownAction()
{
	Serial.println("Toggle Down");

	// this is the handler for the Toggle DOWN Switch
	// currently not used.
	switch (DisplayIndex)
	{
	case dmStart:

		break;

	case dmHome:
		// send the command to set home as the current position, with no offset.
		Serial1.println("hlc");
		break;

	//case dmWingServo:
	//	// send command to Get current servo setting
	//	Serial1.println("gsv");
	//	break;

	case dmConfig: // toggle down action causes a the config to be returned to default at next power up.
		Serial1.print("rst,");
		Serial1.println();
		break;

	case dmParameters:
		Serial1.print("prg,");
		Serial1.println(DisplayIndex2);
		break;

	default:;
	}

}

void toggleMonitor()
{
	// monitor the toggle switch and debounce etc.
	// it has a centre off, up and down.
	ToggleUp = !digitalRead(ToggleUpPin);
	if (ToggleUp && !prevToggleUp)
	{
		prevToggleUp = true;
		ToggleUpAction();
	}

	if (!ToggleUp && prevToggleUp)
	{
		prevToggleUp = false;
	}

	ToggleDown = !digitalRead(ToggleDownPin);
	if (ToggleDown && !prevToggleDown)
	{
		prevToggleDown = true;
		ToggleDownAction();
	}

	if (!ToggleDown && prevToggleDown)
	{
		prevToggleDown = false;
	}
}

void SwitchMonitorLoop(void*)
{
	// called by the scheduled task. typically every 20ms.
	// monitor the Encoder Push Switch.

	LevelUpSwitch = digitalRead(LevelUpPin); //*******************
	if (PrevLevelUpSwitch != LevelUpSwitch)
	{
		PrevLevelUpSwitch = LevelUpSwitch;
		if (LevelUpSwitch)
		{
			Serial.println("Level Up Switch Pressed");
			// handle the Encode push button depending on which page we are on.
			switch (DisplayIndex)
			{
			case dmParameters:
				ChangeEncodeMode_ParameterPage_Up();
				break;
			case dmSteerCompassCourse:
			case dmDisplay:
				ChangeEncodeMode_2Level_Up();
				break;
			case dmSteerWindAngle:
				ChangeEncodeMode_3Level_Up();
				break;
			default:;
			}
		}
	}

	LevelDownSwitch = digitalRead(LevelDownPin); //*******************
	if (PrevLevelDownSwitch != LevelDownSwitch)
	{
		PrevLevelDownSwitch = LevelDownSwitch;
		if (LevelDownSwitch)
		{
			Serial.println("Level Up Switch Pressed");
			// handle the Encode push button depending on which page we are on.
			switch (DisplayIndex)
			{
			case dmParameters:
				ChangeEncodeMode_ParameterPage_Down();
				break;
			case dmSteerCompassCourse:
			case dmDisplay:
				ChangeEncodeMode_2Level_Down();
				break;
			case dmSteerWindAngle:
				ChangeEncodeMode_3Level_Down();
				break;
			default:;
			}
		}
	}
}

void PollLoop(void*)
{
	// called from the Polling loop, typically every 5 seconds
	PollVoyager(DisplayIndex);
}

void DisplayRefreshLoop(void*)
{
	DisplayPage(DisplayIndex);
}

// interrupt service routine to handler the Rotary Encoder, to ensure it feels fast. 
void doEncoder() {

	if (digitalRead(encoder0PinB) == HIGH) {
		// rotate right
		if (DisplayIndex < MaxPageNumber && EncoderMode == EncoderModeType::emPageMode)
			DisplayIndex++;

		if (DisplayIndex2 < MaxIndexNumber && EncoderMode == EncoderModeType::emParameterIndex)
			DisplayIndex2++;

		if (EncoderMode == EncoderModeType::emValue)
			ValueIndex++;

		if (EncoderMode == EncoderModeType::emValue2)
			ValueIndex2++;
	}
	else {
		// rotate left
		if (DisplayIndex > 0 && EncoderMode == EncoderModeType::emPageMode)
			DisplayIndex--;

		if (DisplayIndex2 > 0 && EncoderMode == EncoderModeType::emParameterIndex)
			DisplayIndex2--;

		if (EncoderMode == EncoderModeType::emValue)
			ValueIndex--;

		if (EncoderMode == EncoderModeType::emValue2)
			ValueIndex2--;
	}

	InInterrupt = false;
}

void ChangeEncodeMode_ParameterPage_Up()
{
	// three levels: page mode, index mode and value mode
	switch (EncoderMode)
	{
	case EncoderModeType::emParameterIndex:
		EncoderMode = EncoderModeType::emPageMode;
		break;

	case EncoderModeType::emValue:
		EncoderMode = EncoderModeType::emParameterIndex;
	}
}
void ChangeEncodeMode_ParameterPage_Down()
{
	// three levels: page mode, index mode and value mode
	switch (EncoderMode)
	{
	case EncoderModeType::emPageMode:
		EncoderMode = EncoderModeType::emParameterIndex;
		break;

	case EncoderModeType::emParameterIndex:
		EncoderMode = EncoderModeType::emValue;
		break;
	}
}

void ChangeEncodeMode_2Level_Up()
{
	// two levels: page mode and value mode
	if (EncoderMode== EncoderModeType::emValue)
		EncoderMode = EncoderModeType::emPageMode;
}
void ChangeEncodeMode_2Level_Down()
{
	// two levels: page mode and value mode
	if (EncoderMode == EncoderModeType::emPageMode)
		EncoderMode = EncoderModeType::emValue;
}

void ChangeEncodeMode_3Level_Up()
{
	// three levels: page mode and value mode and second Value Mode
	switch (EncoderMode)
	{
	case EncoderModeType::emValue:
		EncoderMode = EncoderModeType::emPageMode;
		break;

	case EncoderModeType::emValue2:
		EncoderMode = EncoderModeType::emValue;
		break;
	}
}
void ChangeEncodeMode_3Level_Down()
{
	// three levels: page mode and value mode and second Value Mode
	switch (EncoderMode)
	{
	case EncoderModeType::emPageMode:
		EncoderMode = EncoderModeType::emValue;
		break;

	case EncoderModeType::emValue:
		EncoderMode = EncoderModeType::emValue2;
		break;
	}
}

