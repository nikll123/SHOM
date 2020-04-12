/*
Conveyor class - represents a conveyor with contactor, automat and LED.
*/

#ifndef Conveyor_h
#define Conveyor_h

#include "Unit.h"
#include "Led.h"
#include "PinIn.h"
#include "PinOut.h"
#include "Contactor.h"

enum ConveyorState
{
	US_NOTINIT = 0,
	US_UNKNOWN = 1,
	US_ON = 2,
	US_OFF = 3,
	US_STARTING = 4,
	US_STOPPING = 5,
	US_ERR = 200,
	US_LOST_CONNECT = 300,
	US_HALT = 400,
};

struct ConveyorState2
{
	ConveyorState Old;
	ConveyorState New;

	ConveyorState2(ConveyorState csOld = US_NOTINIT, ConveyorState csNew = US_NOTINIT) : Old(csOld), New(csNew){};
};

struct ConveyorStatePrevCurr
{
	ConveyorState Prev;
	ConveyorState Curr;

	ConveyorStatePrevCurr(ConveyorState csPrev = US_NOTINIT, ConveyorState csCurr = US_NOTINIT) : Prev(csPrev), Curr(csCurr){};
};

static LogicType LogicTypeAutomat = LT_NORMAL;

class Conveyor : public Unit
{
public:
	Conveyor();
	Conveyor(const char *title, uint8_t pinIn, uint8_t pinOut, uint8_t pinAuto, uint8_t pinLed);
	//Conveyor(const char * title, uint8_t pinIn, uint8_t pinOut, uint8_t pinAuto, uint8_t pinLed, unsigned long timeOutOn, unsigned long  timeOutOff);

	ConveyorState2 GetState();
	static const char *GetConveyorStateText(ConveyorState state);
	void Init();
	void SetErrState(UnitError err);
	void LogStates(ConveyorState2 cs2);
	void LogStatesPrevCurr(ConveyorStatePrevCurr cs2);
	void IfChanged(ConveyorState2 cs2);
	static void SetupLogic(LogicType ltIn, LogicType ltOut, LogicType ltAuto);
	Led LedConveyor;
	Contactor ContactorConveyor;
	PinIn AtomatConveyor;

	void TurnOn();
	void TurnOff();
	void Halt(LedState ls);
	bool IsActive();

protected:
	void LogKeysState();

private:
	ConveyorState2 _getStateAutomat();
	ConveyorState2 _getStateContactor();
	ConveyorState _state;
	ConveyorState _stateprev;
	void _Turn(ConveyorState csNew);
};
#endif
