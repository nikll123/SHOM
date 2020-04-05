/*
Class PinIn - for pins which are used in INPUT of INPUT_PULLUP modes  
*/
#ifndef PinIn_h
#define PinIn_h

#include "Pin.h"

//-------------------------------
class PinIn : public Pin
{
public:
	PinIn();
	PinIn(const char *title, uint8_t pin);
	PinIn(const char *title, uint8_t pin, LogicType lt);

	PinState2 GetState();

protected:
	void _refreshState();

private:
};
#endif
