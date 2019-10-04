#include "Arduino.h"
//#include "Define.h"
#include "BaseAutomat.h"

BaseAutomat::BaseAutomat(String title, uint8_t pinInput, LogicType lt) : BaseUzel(title, pinInput, 0, UT_AUTOMAT, lt)
{

}

InState BaseAutomat::GetState()
{
	return BaseUzel::GetInputState();
}

String BaseAutomat::GetStateText()
{
	return BaseUzel::GetInStateText();
}

// ------------------------------------
AutomatInfo BaseAutomat::GetInfo()
	{
	return  { 
			_title,
			_inputState,
			_pinIn,
			Core::GetLogicTypeText(_logicType),
			_active,
			_timeOutOn,
			_timeOutOff
			};
	}
