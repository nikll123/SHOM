#ifndef ButtonLed_h
	#define ButtonLed_h
	
	#include "PinIn.h"
	#include "Led.h"

	struct ButtonLedInfo {
					String 		Title;
					String 		UnitType;
					String 		State;
					uint8_t		PinBut;
					uint8_t		PinLed;
					String 		Logic;
					};	


   	//-------------------------------
	class ButtonLed : public Unit
	{
	public:
	    ButtonLed();
	    ButtonLed(String title, uint8_t pinBtn, uint8_t pinLed);
	    
		PinState2		GetState();
		ButtonLedInfo 	GetInfo();
		void			LogInfo();
		void			LogicNormal();
		void			LogicInverse();
		PinIn			Button;
		Led				LedIndicator;

	protected:

	private:
	};
#endif

