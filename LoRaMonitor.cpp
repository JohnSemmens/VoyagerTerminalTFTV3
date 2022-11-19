// 
// 
// 

#include "LoRaMonitor.h"
#include "LCDWIKI_GUI.h" //Core graphics library
#include "LCDWIKI_KBV.h" //Hardware-specific library

extern LCDWIKI_KBV  lcd;

int MonitorRow = 0;
int const TopMargin = 1;
int const LeftMargin = 0;

int const MaxRows = 15;

void LoRaMonitorAdd(char* CLI_Msg)
{
	// Display the incoming string on a monitor screen.
	lcd.Set_Text_colour(GREEN);

	// print the line.
	lcd.setCursor(LeftMargin, TopMargin + MonitorRow);
	lcd.print(CLI_Msg);
	MonitorRow++;

	//Serial.print(" char* MonitorRow: ");
	//Serial.println(MonitorRow);

	lcd.setCursor(LeftMargin, TopMargin + MonitorRow);
	// clear the next line
	lcd.print("                              ");
	//lcd.setCursor(LeftMargin, TopMargin + MonitorRow);

	if (MonitorRow >= MaxRows)
	{
		MonitorRow = 0;
	}
}

void LoRaMonitorAdd(String CLI_Msg)
{
	// Display the incoming string on a monitor screen.

	// print the line.
	lcd.Set_Text_colour(GREEN);

	lcd.setCursor(LeftMargin, TopMargin + MonitorRow); //LeftMargin
	lcd.print(CLI_Msg);
	MonitorRow++;

	//Serial.print(" string MonitorRow: ");
	//Serial.println(MonitorRow);

	lcd.setCursor(LeftMargin, TopMargin + MonitorRow);
	// clear the next line
	lcd.print("                              ");
	//lcd.setCursor(LeftMargin, TopMargin + MonitorRow);

	if (MonitorRow >= MaxRows)
	{
		MonitorRow = 0;
	}
}