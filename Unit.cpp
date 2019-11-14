#include "Arduino.h"
#include "Unit.h"

// =========   CONSTRUCTORS   =========
Unit::Unit()
	{
	_title = "DummyUnit";
	_type = UT_NONE;
	}

Unit::Unit(String title, UnitType type)
	{
	_title = title;
	_type = type;
	}

//------------------------------
static void Unit::LogTextLn(int x)
	{
	LogText(x);
	LogLn();
	}
	
//------------------------------
static void Unit::LogText(int x)
	{
	Serial.print(x);
	}

//------------------------------
static void Unit::LogTextLn(bool b)
	{
	LogText(b);
	LogLn();
	}
	
//------------------------------
static void Unit::LogText(bool b)
	{
	if (b)
		Serial.print("True");
	else
		Serial.print("False");
	}

//------------------------------
static void Unit::LogTextLn(String txt)
	{
	LogText(txt);
	LogLn();
	}
	
//------------------------------
static void Unit::LogText(String txt)
	{
	Serial.print(txt);
	}

static void Unit::LogLn()
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
		case UT_LED         : return "LED";
		case UT_PIN         : return "PIN";
		case UT_PININ		: return "PININ";
		case UT_PINOUT		: return "PINOUT";
		case UT_BUTTONLED	: return "BUTTONLED";
		case UT_SYSTEM 		: return "SYSTEM";
		case UT_CONTACTOR 	: return "CONTACTOR";
		case UT_CONVEYOR 	: return "CONVEYOR";
		default			    : return "GetUnitTypeText: unknown-" + String(_type);
		}
	}

