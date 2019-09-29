#include "Arduino.h"
//#include "Define.h"
#include "BaseAutomat.h"

BaseAutomat::BaseAutomat(String title, uint8_t pinAutomat, LogicType lt) : BaseUzel(title, pinAutomat, 0, UT_AUTOMAT, lt)
{

}

KeyState BaseAutomat::GetState()
{
	return BaseUzel::GetAutomatState();
}

String BaseAutomat::GetStateText()
{
	return BaseUzel::GetAutomatStateText();
}

// ------------------------------------
AutomatInfo BaseAutomat::GetInfo()
{
	return  { _title,
			_automatState,
			_pinAutomat,
			Core::GetLogicTypeText(_logicType),
			_active,
			_timeOutOn,
			_timeOutOff
			};
	
}
