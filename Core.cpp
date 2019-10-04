#include "Arduino.h"
#include "Core.h"
	
	Core::Core();

//------------------------------
static String Core::GetUzelStateText(UzelState us)
{
String stateText = "";
switch (us)
	{
	case US_NOTINIT		: return "NOTINIT";
	case US_ON 			: return "ON";
	case US_OFF			: return "OFF";
	case US_FAULT		: return "FAULT"; 
	case US_STARTING	: return "STARTING"; 
	case US_STOPPING	: return "STOPPING"; 
	case US_UNKNOWN		: return "UNKNOWN";
	case US_ERROR		: return "ERROR";
	case US_ERROR_01	: return "ERROR_01";
	case US_ERROR_02	: return "ERROR_02";
	case US_ERROR_03	: return "ERROR_03";
	case US_ERROR_04	: return "ERROR_04";
	case US_ERROR_05	: return "ERROR_05";
	default			    : return "GetUzelStateText: unknown-" + String(us);
	}
}

//------------------------------
static String Core::GetConveyorStateText(ConveyorState cs)
{
String stateText = "";
switch (cs)
	{
	case CS_NOTINIT		: return "NOTINIT";
	case CS_ON 			: return "ON";
	case CS_OFF			: return "OFF";
	case CS_FAULT		: return "FAULT"; 
	case CS_STARTING	: return "STARTING"; 
	case CS_STOPPING	: return "STOPPING"; 
	case CS_UNKNOWN		: return "UNKNOWN";
	case CS_ERROR		: return "ERROR";
	case CS_ERROR_01	: return "ERROR_01";
	case CS_ERROR_02	: return "ERROR_02";
	default			    : return "GetConveyorStateText: unknown-" + String(cs);
	}
}

//------------------------------
static String Core::GetInStateText(InState as)
{
String stateText = "";
switch (as)
	{
	case KS_NONE		: return "NONE";
	case KS_ON			: return "ON";
	case KS_OFF			: return "OFF";
	default			    : return "GetInStateText: unknown-" + String(as);
	}
}

//------------------------------
static String Core::GetUzelTypeText(UzelType ut)
{
String stateText = "";
switch (ut)
	{
	case UT_NONE 		: return "NONE";
	case UT_AUTOMAT 	: return "AUTOMAT";
	case UT_CONTACTOR 	: return "CONTACTOR";
	default			    : return "GetUzelTypeText: unknown-" + String(ut);
	}
}

//------------------------------
static String Core::GetLogicTypeText(LogicType lt)
{
String stateText = "";
switch (lt)
	{
	case LT_NONE 		: return "NONE";
	case LT_NORMAL	 	: return "NORMAL";
	case LT_INVERSE 	: return "INVERSE";
	default			    : return "GetLogicTypeText: unknown-" + String(lt);
	}
}

//------------------------------
static void Core::LogIntVal(String txt, int val)
	{
	Serial.print(RTC.getTimeStr());
	Serial.print(txt);
	Serial.print(" = ");
	Serial.println(val);
	}

//------------------------------
static void Core::LogTextLn(String txt)
	{
	LogText(txt , 1);
	}

//------------------------------
static void Core::LogText(String txt)
	{
	LogText(txt , 0);
	}

//------------------------------
/*static void Core::LogText(String txt, bool ln)
	{
	Serial.print(GetTime());
	Serial.print(" - ");
	Serial.print(txt);
	if (ln) Serial.println("");
	}      */

//------------------------------
static void Core::LogText(String txt, bool ln)
	{
	Serial.print(txt);
	if (ln) Serial.println("");
	}

//------------------------------
static String Core::GetDateTime()
{
	String txtTime = RTC.getDateTimeStr();
	return txtTime;
}

//------------------------------
static String Core::GetTime()
{
	String txtTime = RTC.getTimeStr();
	return txtTime;
}

//------------------------------
static String Core::GetDate()
{
	String txtTime = RTC.getDateStr();
	return txtTime;
}
