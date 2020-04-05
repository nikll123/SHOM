/*
Calss Pin - parent class for pins, Basic class for PinIn and PinOut classes
Should not be used for object creation directly  
*/

#ifndef Pin_h
#define Pin_h

#include "Unit.h"
#include "ShomCanBus.h"

//-------------------------------
enum PinState
{
	KS_NONE = 1,
	KS_ON = 2,
	KS_OFF = 3,
	KS_ERR = 500,
};

//-------------------------------
struct PinState2
{
	PinState Old;
	PinState New;
	bool High() { return Old == KS_ON && New == KS_ON; };
	bool Low() { return Old == KS_OFF && New == KS_OFF; };
	bool Front() { return Old == KS_OFF && New == KS_ON; };
	bool Back() { return Old == KS_ON && New == KS_OFF; };
	bool Changed() { return Old != New; };
};

//-------------------------------
struct PinInfo
{
	String Title;
	String UnitType;
	String PinModeText;
	String State;
	uint8_t Pin;
};

//-------------------------------
struct PinRespCode
{
	int RespCode = 0;
	bool Value = false;
};

//-------------------------------
enum LogicType
{
	LT_NONE,
	LT_NORMAL,
	LT_INVERSE
};

//-------------------------------
class Pin : public Unit
{
public:
	Pin();
	Pin(const char *title, uint8_t pin, UnitType ut);

	PinInfo GetInfo();
	PinState GetState();
	uint8_t GetPin();
	void LogInfo();
	void LogState();
	void LogStates(PinState2 ps2);
	void SetLogicType(LogicType logicType);

	const char *LogicTypeText();

	const char *PinStateText(PinState);
	const char *StateText();
	const char *PinModeText();

	void SetON();
	void SetOFF();
	void SetON(bool doLog);
	void SetOFF(bool doLog);
	void SetState(PinState state, bool doLog);

	bool IsHigh();
	bool IsLow();
	bool ShomPinRead();
	void ShomPinWrite(bool val);
	void ShomPinMode(uint8_t pinmode);
	static ShomCanBus CanBus;
	bool DoLogChanges = 1;

protected:
	uint8_t _pin;
	PinState _state = KS_NONE;
	uint8_t _pinmode;
	LogicType _logicType;

private:
};
#endif
