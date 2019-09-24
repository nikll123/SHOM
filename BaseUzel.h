#ifndef KeyUnit_h
	#define KeyUnit_h
	
	#include "Arduino.h"
	#include "Core.h"


	class BaseUzel
	{
	public:
	    BaseUzel();
	    BaseUzel(String title, uint8_t pinAutomat, uint8_t pinContactor, UzelType uzelType, LogicType logicType);
	    BaseUzel(String title, uint8_t pinAutomat, uint8_t pinContactor, UzelType uzelType, LogicType logicType, unsigned int timeOutOn, unsigned int timeOutOff);

		String 		GetTitle();
		UzelType 	GetUzelType();
		LogicType 	GetLogicType();
        String 		GetUzelTypeText();
		String 		GetLogicTypeText();
		UzelState 	GetState();
		String 		GetStateText();
		String 		GetKeyStateText();
		KeyState1 	CheckAutomatState();
		UzelState 	CheckState();
		void 		LogState(String);
		void 		LogType(String);
		void 		LogTitle(String);
		void 		LogDateTime(String);
		void 		LogText(String);
		void 		LogTextLn(String);
		
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
		bool 			_initialized;
		unsigned int 	_timeOutOn;
		unsigned int 	_timeOutOff;
		unsigned long 	_millsCheck;
		void 			logState(String, bool);
	};
#endif

