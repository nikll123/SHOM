#include "Arduino.h"
//#include "Define.h"
#include "AutomatInverse.h"

AutomatInverse::AutomatInverse(String title, uint8_t pinInput) : BaseAutomat(title, pinInput, LT_INVERSE)
{
}

