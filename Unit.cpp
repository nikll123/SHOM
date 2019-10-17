#include "Arduino.h"
#include "Unit.h"

// =========   CONSTRUCTORS   =========
Unit::Unit()
	{
	_title = "";
	_type = UT_NONE;
	}

Unit::Unit(String title, UnitType type)
	{
	_title = title;
	_type = type;
	}

//------------------------------
void Unit::LogTextLn(int x)
	{
	LogText(x);
	LogLn();
	}
	
//------------------------------
void Unit::LogText(int x)
	{
	Serial.print(x);
	}

//------------------------------
void Unit::LogTextLn(bool b)
	{
	LogText(b);
	LogLn();
	}
	
//------------------------------
void Unit::LogText(bool b)
	{
	if (b)
		Serial.print("True");
	else
		Serial.print("False");
	}

//------------------------------
void Unit::LogTextLn(String txt)
	{
	LogText(txt);
	LogLn();
	}
	
//------------------------------
void Unit::LogText(String txt)
	{
	Serial.print(txt);
	}

void Unit::LogLn()
	{
	Serial.println("");
	}

//------------------------------
UnitInfo Unit::GetInfo()
	{
	return	{
			_title,
			GetUnitTypeText()
			};
	}

//------------------------------
void Unit::LogInfo()
	{
	UnitInfo ui = GetInfo();
	LogTextLn(ui.Title);
	LogTextLn(ui.UnitType);
	}

//------------------------------
String Unit::GetUnitTypeText()
	{
	switch (_type)
		{
		case UT_NONE 		: return "NONE";
		case UT_BUTTON 		: return "BUTTON";
		case UT_LED         : return "LED";
		case UT_PIN         : return "PIN";
		case UT_PININ		: return "PININ";
		case UT_PINOUT		: return "PINOUT";
		case UT_PININLED	: return "PININLED";
		case UT_AUTOMAT 	: return "AUTOMAT";
		case UT_CONTACTOR 	: return "CONTACTOR";
		case UT_CONVEYOR 	: return "CONVEYOR";
		default			    : return "GetUnitTypeText: unknown-" + String(_type);
		}
	}

