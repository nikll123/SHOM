#ifndef Led_h
	#define Led_h

	#include "Unit.h"
	#include "PinOut.h"
	#define INTERVAL_BLINK 300
    #define INTERVAL_BLINKFAST 100
	#define INTERVAL_BLINKSLOW 800

	struct LedInfo {
					String 		Title;
					String 		UnitType;
					uint8_t 	Pin;
					String		State;
					};

	enum LedState 	{
					LED_NOTINIT,
					LED_ON,
					LED_OFF,
					LED_BLINK,
					LED_BLINK2,
					LED_BLINK3,
					LED_BLINKFAST,
					LED_BLINKSLOW
					};

	struct LedState2 {
					LedState Old;
					LedState New;
					};

	//-------------------------------
	class Led : public Unit
	{
	public:
					Led();
					Led(String title, uint8_t pin);
					Led(String title, uint8_t pin, LedState ledState);
		LedInfo 	GetInfo();
		LedState	GetState();
		LedState2	Refresh();

		void 		LogInfo();
		void 		SetOn();
		void 		SetOff();
		void 		SetBlink();
		void 		SetBlinkFast();
		void 		SetBlinkSlow();
		String 		GetLedStateText(LedState ls);
	    PinOut		PinOutLed;
	    bool 		IsActive();

	protected:

	private:
		void 			_logState(LedState2);
	    LedState		_state;
		unsigned long 	_millis;
		void 			_setState(LedState ls);
		void 			_refreshState();
		bool 			_checkBlink(bool b, int interval);
	};

#endif
