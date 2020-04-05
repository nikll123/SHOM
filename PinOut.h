/*
Class PinOut - for pins which are used in OUTPUT mode  
*/

#ifndef PinOut_h
#define PinOut_h

#include "Pin.h"

//-------------------------------
class PinOut : public Pin
{
public:
	PinOut();
	PinOut(const char *title, uint8_t pin);
	PinOut(const char *title, uint8_t pin, LogicType lt);
	PinOut(const char *title, uint8_t pin, LogicType lt, UnitType ut);

	void Inverse();

protected:
private:
};
#endif
