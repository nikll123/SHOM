#include "Arduino.h"
//#include "Define.h"
#include "AutomatInverse.h"

AutomatInverse::AutomatInverse(String title, uint8_t pinAutomat) : BaseUzel(title, pinAutomat, 0, UT_AUTOMAT, LT_INVERSE)
{
}

