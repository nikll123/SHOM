#include "AutomatInverse.h"

// ------------------------------------
AutomatInverse::AutomatInverse(String title, uint8_t pinButton) : ButtonInverse(title, pinButton)
{
	_type = UT_AUTOMAT;
}
 