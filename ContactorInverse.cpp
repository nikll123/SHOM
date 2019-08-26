//#include "Arduino.h"
//#include "Define.h"
#include "ContactorInverse.h"

ContactorInverse::ContactorInverse(String title, uint8_t pinAutomat, uint8_t pinContactor) 
		: BaseUzel(title, pinAutomat, pinContactor, UNIT_CONTACTOR, LOGIC_INVERSE)
{
}

ContactorInverse::ContactorInverse(String title, uint8_t pinAutomat, uint8_t pinContactor, unsigned int timeOutOn, unsigned int timeOutOff) 
		: BaseUzel(title, pinAutomat, pinContactor, UNIT_CONTACTOR, LOGIC_INVERSE, timeOutOn, timeOutOff)
{
}

