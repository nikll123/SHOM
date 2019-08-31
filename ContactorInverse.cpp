//#include "Arduino.h"
//#include "Define.h"
#include "ContactorInverse.h"

ContactorInverse::ContactorInverse(String title, uint8_t pinAutomat, uint8_t pinContactor) 
		: BaseUzel(title, pinAutomat, pinContactor, UT_CONTACTOR, LT_INVERSE)
{
}

ContactorInverse::ContactorInverse(String title, uint8_t pinAutomat, uint8_t pinContactor, unsigned int timeOutOn, unsigned int timeOutOff) 
		: BaseUzel(title, pinAutomat, pinContactor, UT_CONTACTOR, LT_INVERSE, timeOutOn, timeOutOff)
{
}

