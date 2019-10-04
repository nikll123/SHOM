#include "Arduino.h"
//#include "Define.h"
#include "Contactor.h"

Contactor::Contactor(String title, uint8_t pinInput, uint8_t pinOutput) 
		: BaseUzel(title, pinInput, pinOutput, UT_CONTACTOR, LT_NORMAL)
{
}

Contactor::Contactor(String title, uint8_t pinInput, uint8_t pinOutput, unsigned int timeOutOn, unsigned int timeOutOff) 
		: BaseUzel(title, pinInput, pinOutput, UT_CONTACTOR, LT_NORMAL, timeOutOn, timeOutOff)
{
}

