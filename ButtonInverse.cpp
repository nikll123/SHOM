#include "Arduino.h"
#include "Core.h"
#include "ButtonInverse.h"

// ------------------------------------
ButtonInverse::ButtonInverse(String title, uint8_t pinButton) : BaseButton(title, pinButton, LT_INVERSE)
{

}
 
