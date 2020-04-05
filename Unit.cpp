#include "Arduino.h"
#include "Unit.h"

// =========   CONSTRUCTORS   =========
Unit::Unit()
{
	_title = "DummyUnit";
	_type = UT_NONE;
}

Unit::Unit(const char *title, UnitType type)
{
	_title = strdup(title);
	_type = type;
}

//------------------------------
void Unit::LogErr(UnitError err)
{
	char pref[STRMAXLEN] ="";
	errPrefix(err, pref);
	Log_("   Error! ");
	Log_(_title);
	Log_(" ");
	Log_(pref);
	Log_("_ERR");
	LogInt(err);
}

//------------------------------
void Unit::LogInfo()
{
	Log_(_title);
	Log_(" ");
	Log(UnitTypeText());
}

const char * Unit::GetTitle()
{
	return _title;
}

//------------------------------
const char *Unit::UnitTypeText()
{
	switch (_type)
	{
	case UT_NONE:
		return "NONE";
	case UT_LED:
		return "LED";
	case UT_PIN:
		return "PIN";
	case UT_PININ:
		return "PININ";
	case UT_PINOUT:
		return "PINOUT";
	case UT_BUTTONLED:
		return "BUTTONLED";
	case UT_SYSTEM:
		return "SYSTEM";
	case UT_CONTACTOR:
		return "CONTACTOR";
	case UT_CONVEYOR:
		return "CONVEYOR";
	case UT_CONVEYORHANDLER:
		return "CONVEYORHANDLER";
	case UT_CANBUS:
		return "CANBUS";
	default:
		return "UnitTypeText: unknown";
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
