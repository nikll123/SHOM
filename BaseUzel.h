#ifndef KeyUnit_h
	#define KeyUnit_h
	
	#include "Arduino.h"
	#include "Core.h"

	struct UzelInfo	{
					UzelType 	Type;
					String   	TypeText;
					String   	Title;
					UzelState	State;
					uint8_t		PinAutomat;
					uint8_t     PinContactor;
					String		Logic;
					KeyState1	StateAutomat;
					bool		Active;
					unsigned int TimeOutOn;
					unsigned int TimeOutOff;
					}; 


	class BaseUzel
	{
	public:
	    BaseUzel();
	    BaseUzel(String title, uint8_t pinAutomat, uint8_t pinContactor, UzelType uzelType, LogicType logicType);
	    BaseUzel(String title, uint8_t pinAutomat, uint8_t pinContactor, UzelType uzelType, LogicType logicType, unsigned int timeOutOn, unsigned int timeOutOff);

		UzelInfo	GetInfo();
		KeyState1 	CheckAutomatState();
		UzelState 	CheckState();
		
		void TurnOn();
		void TurnOff();
		void TurnOffAlarm();

	protected:

	private:
		String			_title;
		UzelState		_state;
		uint8_t 		_pinAutomat;
		uint8_t 		_pinContactor;
		UzelType 		_uzelType;
		LogicType		_logicType;
		KeyState1		_automatState;
		bool 			_active;
		unsigned int 	_timeOutOn;
		unsigned int 	_timeOutOff;
		unsigned long 	_millsCheck;
	};
#endif

