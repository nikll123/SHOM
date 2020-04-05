/*
System is a chain of conveyors.
This is a MASTER unit in the syste (in contrast to SALVE unit)
*/

#ifndef System_h
#define System_h

#include "Conveyor.h"
#define CANBUS_TIMEOUT 1000

enum SystemState
{
	SS_NOTINIT = 1,
	SS_UNKNOWN = 2,
	SS_ON = 3,
	SS_OFF = 4,
	SS_STARTING = 5,
	SS_STOPPING = 6,
	SS_ERR = 300,
};

struct SystemState2
{
	SystemState Old;
	SystemState New;
};

class System : Unit
{
public:
	System();
	System(const char *title, uint8_t pin_button_on, uint8_t _pin_button_off, uint8_t pin_button_reset);
	void Init();
	//SystemInfo 		GetInfo();
	void LogInfo();
	void LogInfo(bool conv);
	void SetupConveyor(const char *title, uint8_t pinIn, uint8_t pinOut, uint8_t pinAuto, uint8_t pinLed);
	PinIn SetupButton(const char *suffix, uint8_t pin);
	void SetupLogic(LogicType ltIn, LogicType ltOut);
	static const char *GetSystemStateText(SystemState state);
	uint8_t UnitCount = 0;
	void TurnLeds(bool on);

	Conveyor Conveyors[MAX_UNIT_NUMBER];
	ConveyorState2 ConveyorStates[MAX_UNIT_NUMBER];
	PinIn BtnOn;
	PinIn BtnOff;
	PinIn BtnReset;
	Unit Timer;
	PinIn ConnectChecker;

	void Start();
	void Stop();
	void Reset();
	void TurnOff();
	void TurnOffAlarm();
	void SetErrState(UnitError err);
	void SetErrState(UnitError err, const char *msg);
	void SetErrState(UnitError err, const char *msg1, const char *msg2);
	SystemState2 GetState();
	void CheckConnection();
	void Run();

private:
	void _setState(SystemState state);
	void _logIfChanged(SystemState2 cs2);
	void _logStates(SystemState2 cs2);
	void _updateConveyorStates();
	SystemState _checkStateOff();
	SystemState _checkStateOn();
	SystemState _checkStateStarting();
	SystemState _checkStateStopping();
	bool _checkButtons();
	void _ledRefresh();
	SystemState _state = SS_UNKNOWN;
	SystemState _calcState(int cntErr, int cntOn, int cntOff, int cntStoping, int cntStarting);
};
#endif
