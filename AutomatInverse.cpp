#include "Arduino.h"
//#include "Define.h"
#include "AutomatInverse.h"

AutomatInverse::AutomatInverse(String title, uint8_t pinAutomat) : BaseAutomat(title, pinAutomat, LT_INVERSE)
{
}

