#include "Arduino.h"
//#include "Define.h"
#include "Automat.h"

Automat::Automat(String title, uint8_t pinAutomat) : BaseUzel(title, pinAutomat, 0, UNIT_AUTOMAT, LOGIC_NORMAL)
{
}

