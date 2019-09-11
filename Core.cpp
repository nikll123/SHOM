#include "Arduino.h"
#include "Core.h"
	
	Core::Core();

//------------------------------
static String Core::GetUzelStateText(UzelState us)
{
String stateText = "";
switch (us)
	{
	case US_NOTINIT		: return "US_NOTINIT";
	case US_ON 			: return "US_ON";
	case US_OFF			: return "US_OFF";
	case US_FAULT		: return "US_FAULT"; 
	case US_STARTING	: return "US_STARTING"; 
	case US_STOPPING	: return "US_STOPPING"; 
	case US_UNKNOWN		: return "US_UNKNOWN";
	case US_ERROR		: return "US_ERROR";
	case US_ERROR_01	: return "US_ERROR_01";
	case US_ERROR_02	: return "US_ERROR_02";
	case US_ERROR_03	: return "US_ERROR_03";
	case US_ERROR_04	: return "US_ERROR_04";
	default			    : return "GetUzelStateText: unknown-" + String(us);
	}
}

//------------------------------
static String Core::GetConveyorStateText(ConveyorState cs)
{
String stateText = "";
switch (cs)
	{
	case CS_NOTINIT		: return "CS_NOTINIT";
	case CS_ON 			: return "CS_ON";
	case CS_OFF			: return "CS_OFF";
	case CS_FAULT		: return "CS_FAULT"; 
	case CS_STARTING	: return "CS_STARTING"; 
	case CS_STOPPING	: return "CS_STOPPING"; 
	case CS_UNKNOWN		: return "CS_UNKNOWN";
	case CS_ERROR		: return "CS_ERROR";
	case CS_ERROR_01	: return "CS_ERROR_01";
	case CS_ERROR_02	: return "CS_ERROR_02";
	default			    : return "GetConveyorStateText: unknown-" + String(cs);
	}
}

//------------------------------
static String Core::GetKeyStateText(KeyState as)
{
String stateText = "";
switch (as)
	{
	case KS_NONE		: return "KS_NONE";
	case KS_ON			: return "KS_ON";
	case KS_OFF			: return "KS_OFF";
	case KS_FRONT		: return "KS_FRONT";		// ?????
	case KS_BACK		: return "KS_BACK";			// ????
	default			    : return "GetKeyStateText: unknown-" + String(as);
	}
}

//------------------------------
static String Core::GetUzelTypeText(UzelType ut)
{
String stateText = "";
switch (ut)
	{
	case UT_NONE 		: return "UT_NONE";
	case UT_AUTOMAT 	: return "UT_AUTOMAT";
	case UT_CONTACTOR 	: return "UT_CONTACTOR";
	default			    : return "GetUzelTypeText: unknown-" + String(ut);
	}
}

//------------------------------
static String Core::GetLogicTypeText(LogicType lt)
{
String stateText = "";
switch (lt)
	{
	case LT_NONE 		: return "LT_NONE";
	case LT_NORMAL	 	: return "LT_NORMAL";
	case LT_INVERSE 	: return "LT_INVERSE";
	default			    : return "GetLogicTypeText: unknown-" + String(lt);
	}
}

//------------------------------
static void Core::LogIntVal(String txt, int val)
{
#ifdef PortMonitorLog
	Serial.print(RTC.getTimeStr());
	Serial.print(txt);
	Serial.print(" = ");
	Serial.println(val);
#endif
}

//------------------------------
static void Core::LogText(String txt)
{
#ifdef PortMonitorLog
	Serial.print(RTC.getTimeStr());
	Serial.print(" - ");
	Serial.println (txt);
#endif
}