#include "Arduino.h"
//#include "Define.h"
#include "Contactor.h"

Contactor::Contactor(String title, uint8_t pinAutomat, uint8_t pinContactor) 
		: BaseUzel(title, pinAutomat, pinContactor, UNIT_CONTACTOR, LOGIC_NORMAL)
{
}

Contactor::Contactor(String title, uint8_t pinAutomat, uint8_t pinContactor, unsigned int timeOutOn, unsigned int timeOutOff) 
		: BaseUzel(title, pinAutomat, pinContactor, UNIT_CONTACTOR, LOGIC_NORMAL, timeOutOn, timeOutOff)
{
}

