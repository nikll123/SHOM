/*
Class Led for LED control  
*/

#ifndef Led_h
#define Led_h

#include "Unit.h"
#include "PinOut.h"
#define INTERVAL_BLINK 300
#define INTERVAL_BLINKFAST 100
#define INTERVAL_BLINKSLOW 800
#define DONOTLOG false

enum LedState
{
	LED_NOTINIT,
	LED_ON,
	LED_OFF,
	LED_BLINK,
	LED_BLINK2,
	LED_BLINK3,
	LED_BLINKFAST,
	LED_BLINKSLOW
};

struct LedState2
{
	LedState Old;
	LedState New;
};

//-------------------------------
class Led : public PinOut
{
public:
	Led();
	Led(const char *title, uint8_t pin);
	Led(const char *title, uint8_t pin, LedState ledState);

	LedState GetState();
	LedState2 Refresh();

	void LogInfo();
	void SetOn();
	void SetOff();
	void Inverse();
	void SetBlink();
	void SetBlinkFast();
	void SetBlinkSlow();
	void SetState(LedState ls);
	const char *GetLedStateText(LedState ls);

protected:
private:
	void _logState(LedState2);
	LedState _state;
	unsigned long _millis;
	void _refreshState();
	bool _checkBlink(bool b, int interval);
};

#endif
