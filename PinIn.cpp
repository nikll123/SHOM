#include "PinIn.h"
// ------------------------------------
PinIn::PinIn()
{
}

// ------------------------------------
PinIn::PinIn(const char *title, uint8_t pin) : PinIn(title, pin, LT_NORMAL)
{
}

// ------------------------------------
PinIn::PinIn(const char *title, uint8_t pin, LogicType lt) : Pin(title, pin, UT_PININ)
{
	uint8_t inpmode;
	Pin::SetLogicType(lt);
	if (_logicType == LT_NORMAL)
		inpmode = INPUT;
	else
		inpmode = INPUT_PULLUP;

	ShomPinMode(inpmode);
}

// ------------------------------------
PinState2 PinIn::GetState()
{
	PinState2 ps2;
	ps2.Old = _state;
	_refreshState();
	ps2.New = _state;

	if (ps2.Old != ps2.New)
		LogStates(ps2);

	return ps2;
}

// ------------------------------------
void PinIn::_refreshState()
{
	bool newState = ShomPinRead();

	if (_logicType == LT_INVERSE)
		newState = !newState;

	if (newState == 1)
		_state = KS_ON;
	else //  if (newState == 0)
		_state = KS_OFF;
}

