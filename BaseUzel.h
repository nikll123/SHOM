#ifndef KeyUnit_h
	#define KeyUnit_h
	
	#include "Arduino.h"
	#include "Core.h"

	struct UzelInfo	{
					UzelType 	Type;
					String   	TypeText;
					String   	Title;
					UzelState	State;
					uint8_t		PinInput;
					uint8_t     PinOutput;
					String		Logic;
					InputState	StateAutomat;
					bool		Active;
					unsigned int TimeOutOn;
					unsigned int TimeOutOff;
					}; 

	class BaseUzel
	{
	public:
	    BaseUzel();
	    BaseUzel(String title, uint8_t pinInput, uint8_t pinOutput, UzelType uzelType, LogicType logicType);
	    BaseUzel(String title, uint8_t pinInput, uint8_t pinOutput, UzelType uzelType, LogicType logicType, unsigned int timeOutOn, unsigned int timeOutOff);

		UzelInfo	GetInfo();
		InputState 	GetInputState();
		String 		GetInputStateText();
		UzelState 	GetState();
		String 		GetStateText();		
		//InputState2 	CheckInState();
		InputState2 	CheckInState(uint8_t loglevel);
		
		UzelState2 	CheckState();
		bool		IsActive();
		
		void TurnOn();
		void TurnOff();
		void TurnOffAlarm();

	protected:
		String			_title;
		uint8_t 		_pinIn;
		LogicType		_logicType;
		InputState		_inputState;
		bool 			_active;
		unsigned int 	_timeOutOn;
		unsigned int 	_timeOutOff;

	private:
		UzelState		_state;
		uint8_t 		_pinOut;
		UzelType 		_uzelType;
		unsigned long 	_millsCheck;
	};
#endif

