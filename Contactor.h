/*
Class Contactor - intended for dealing with contactor.
Contactor has a control pin - KeyOut and a feedback pin KeyIn
Contactor correct states are when KeyOut and KeyIn have equal states    
States when KeyOut != KeyIn are erroneous.     
*/

#ifndef Contactor_h
#define Contactor_h

#include "Unit.h"
#include "PinIn.h"
#include "PinOut.h"

enum ContactorState
{
	CS_NOTINIT = 0,
	CS_ON = 1,
	CS_OFF = 2,
	CS_STARTING = 3,
	CS_STOPPING = 4,
	CS_UNKNOWN = 5,
	CS_ERR = 100,
	CS_HALT = 199,
	CS_LOST_CONNECT = 300,
};

struct ContactorState2
{
	ContactorState Old;
	ContactorState New;

	ContactorState2(ContactorState csOld = CS_NOTINIT, ContactorState csNew = CS_NOTINIT) : Old(csOld), New(csNew){};
};

static LogicType LogicTypeIn = LT_NORMAL;
static LogicType LogicTypeOut = LT_NORMAL;

class Contactor : public Unit
{
public:
	Contactor();
	Contactor(const char *title, uint8_t pinIn, uint8_t pinOut);

	ContactorState2 GetState();
	const char *GetContactorStateText(ContactorState state);
	void Init();
	void LogInfo();
	void SetErrState(UnitError err);
	void SetErrState(UnitError err, String msg);
	PinIn KeyIn; //Why not PinIn? PinIn is already used as a class name
	PinOut KeyOut;
	static void SetupLogic(LogicType ltIn, LogicType ltOut);

	void TurnOn();
	void TurnOff();
	void Halt();

protected:
	void LogKeysState();

private:
	ContactorState2 _getStateAutomat();
	ContactorState2 _getStateContactor();
	ContactorState _state = CS_UNKNOWN;
	PinState _stateIn = KS_NONE;
	PinState _stateOut = KS_NONE;
	void _logIfChanged(ContactorState2 cs2);
	unsigned int _timeOutOn = 3000;
	void _Turn(ContactorState csNew);
};
#endif
