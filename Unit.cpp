#include "Unit.h"

// =========   CONSTRUCTORS   =========
Unit::Unit()
	{
	_title = "DummyUnit";
	_type = UT_NONE;
	}

Unit::Unit(char *title, UnitType type)
	{
	_title = title;
	_type = type;
	}

//------------------------------
void Unit::Log(char * str)
	{
	Log::BufClear();
	Log::BufCat(_title);
	Log::BufCat(" ");
	Log::BufCat(str);
	Log::BufPrint();
	}


//------------------------------
void Unit::LogErr(UnitError err)
	{
	Log::BufClear();
	Log::BufCat("   Error! ");
	Log::BufCat(_title);
	Log::BufCat(" ");
	if (100 < err && err < 200)			Log::BufCat("CS");
	else if (200 < err && err < 300)	Log::BufCat("US");
	else if (300 < err && err < 400)	Log::BufCat("SS");
	else if (400 < err && err < 500)	Log::BufCat("CBS");
	else if (500 < err && err < 600)	Log::BufCat("KS");
	else if (600 < err && err < 700)	Log::BufCat("SL");
	else								Log::BufCat("Unknown");
	
	Log::BufCat("_ERR");
	Log::BufCat(err);
	Log::BufPrint();
	}	


//------------------------------
void Unit::LogInfo()
	{
	Log::BufClear();
	Log::BufCat(_title);
	Log::BufCat(" ");
	Log::BufCat(Unit::UnitTypeText(_type));
	Log::BufPrint();
	}

//------------------------------
static char *Unit::UnitTypeText(UnitType ut)
	{
	switch (ut)
		{
		case UT_NONE 			: return "NONE";
		case UT_LED         	: return "LED";
		case UT_PIN         	: return "PIN";
		case UT_PININ			: return "PININ";
		case UT_PINOUT			: return "PINOUT";
		case UT_BUTTONLED		: return "BUTTONLED";
		case UT_SYSTEM 			: return "SYSTEM";
		case UT_CONTACTOR 		: return "CONTACTOR";
		case UT_CONVEYOR 		: return "CONVEYOR";
		case UT_CONVEYORHANDLER	: return "CONVEYORHANDLER";
		case UT_CANBUS 			: return "CANBUS";
		default			    	: return "UnitTypeText: unknown-" + char(ut);
		}
	}

// ------------------------------------
unsigned long Unit::Time(TimeAct ta)
	{
	unsigned long res = 0; 
	if (ta == TA_FIX)
		_millsCheck = millis();
	else if (ta == TA_GET)
		res = _millsCheck;
	else if (ta == TA_RESET)
		_millsCheck = 0;
	else if (ta == TA_PERIOD)
	    res = millis() - _millsCheck;
	    
	return res;
	}
 
