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
					KeyState	StateAutomat;
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
		KeyState 	GetAutomatState();
		String 		GetAutomatStateText();
		KeyState2 	CheckAutomatState();
		UzelState 	GetState();
		String 		GetStateText();		
		UzelState2 	CheckState();
		bool		IsActive();
		
		void TurnOn();
		void TurnOff();
		void TurnOffAlarm();

	protected:
		String			_title;
		uint8_t 		_pinAutomat;
		LogicType		_logicType;
		KeyState		_automatState;
		bool 			_active;
		unsigned int 	_timeOutOn;
		unsigned int 	_timeOutOff;

	private:
		UzelState		_state;
		uint8_t 		_pinContactor;
		UzelType 		_uzelType;
		unsigned long 	_millsCheck;
	};
#endif

