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
	_title = title;
	_type = type;
}

//------------------------------
void Unit::LogErr(UnitError err)
{
	const char *pref = errPrefix(err);
	Log_("   Error! ", _title, " ", pref, "_ERR");
	LogInt(err);
}

//------------------------------
void Unit::LogInfo()
{
	char buf[STRMAXLEN];
	Log(_title, " ", UnitTypeText(buf));
}

//------------------------------
const char *Unit::UnitTypeText(const char *buf)
{
	switch (_type)
	{
	case UT_NONE:
		buf = "NONE";
		break;
	case UT_LED:
		buf = "LED";
		break;
	case UT_PIN:
		buf = "PIN";
		break;
	case UT_PININ:
		buf = "PININ";
		break;
	case UT_PINOUT:
		buf = "PINOUT";
		break;
	case UT_BUTTONLED:
		buf = "BUTTONLED";
		break;
	case UT_SYSTEM:
		buf = "SYSTEM";
		break;
	case UT_CONTACTOR:
		buf = "CONTACTOR";
		break;
	case UT_CONVEYOR:
		buf = "CONVEYOR";
		break;
	case UT_CONVEYORHANDLER:
		buf = "CONVEYORHANDLER";
		break;
	case UT_CANBUS:
		buf = "CANBUS";
		break;
	default:
		buf = "UnitTypeText: unknown";
	}
	return buf;
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
