#ifndef Led_h
	#define Led_h

	#include "Unit.h"
	
	struct LedInfo {
					String 		Title;
					String 		UnitType;
					uint8_t 	Pin;
					String		State;
					};	

	enum LedState 	{
					LED_NOTINIT = 1, 
					LED_ON = 2, 
					LED_OFF = 3, 
					LED_BLINK = 4
					}; 
	
	//-------------------------------
	class Led : public Unit
	{
	public:
					Led(String title, uint8_t pin);
		LedInfo 	GetInfo();
		void 		LogInfo();
		String 		GetLedStateText();

		
	protected:
	    uint8_t		_pin;
	    LedState	_state;
	private: 
	};
	    
#endif
