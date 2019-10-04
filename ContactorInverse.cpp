//#include "Arduino.h"
//#include "Define.h"
#include "ContactorInverse.h"

ContactorInverse::ContactorInverse(String title, uint8_t pinInput, uint8_t pinOutput) 
		: BaseUzel(title, pinInput, pinOutput, UT_CONTACTOR, LT_INVERSE)
{
}

ContactorInverse::ContactorInverse(String title, uint8_t pinInput, uint8_t pinOutput, unsigned int timeOutOn, unsigned int timeOutOff) 
		: BaseUzel(title, pinInput, pinOutput, UT_CONTACTOR, LT_INVERSE, timeOutOn, timeOutOff)
{
}

