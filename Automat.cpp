#include "Automat.h"

// ------------------------------------
Automat::Automat(String title, uint8_t pinButton) : Button(title, pinButton)
{
	_type = UT_AUTOMAT;
}
 