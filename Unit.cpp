#include "Arduino.h"
#include "Unit.h"

// =========   CONSTRUCTORS   =========
Unit::Unit(String title, UnitType type)
	{
	_title = title;
	_type = type;
	}

//------------------------------
void Unit::LogTextLn(String txt)
	{
	LogText(txt , 1);
	}
	
//------------------------------
void Unit::LogText(String txt)
	{
	LogText(txt, 0);
	}
	
//------------------------------
void Unit::LogText(String txt, bool ln)
	{
	Serial.print(txt);
	if (ln) Serial.println("");
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
String Unit::GetUnitTypeText()
	{
	switch (_type)
		{
		case UT_NONE 		: return "NONE";
		case UT_BUTTON 		: return "BUTTON";
		case UT_LED         : return "LED";
		case UT_AUTOMAT 	: return "AUTOMAT";
		case UT_CONTACTOR 	: return "CONTACTOR";
		default			    : return "GetUnitTypeText: unknown-" + String(_type);
		}
	}


