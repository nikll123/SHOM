#include "Arduino.h"
//#include "Define.h"
#include "Contactor.h"

Contactor::Contactor(String title, uint8_t pinAutomat, uint8_t pinContactor) 
		: BaseUzel(title, pinAutomat, pinContactor, UNIT_CONTACTOR, LOGIC_NORMAL)
{
}

