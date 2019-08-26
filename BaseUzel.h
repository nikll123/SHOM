#ifndef KeyUnit_h
	#define KeyUnit_h
	
	#include "Arduino.h"
	#include "Core.h"


	class BaseUzel
	{
	public:
	    BaseUzel();
	    BaseUzel(String title, uint8_t pinAutomat, uint8_t pinContactor, bool UnitType, bool LogicType);
	    BaseUzel(String title, uint8_t pinAutomat, uint8_t pinContactor, bool UnitType, bool LogicType, unsigned int timeOutOn, unsigned int timeOutOff);
		uint8_t GetState();
		uint8_t CheckState();
		String GetStateTxt();
		String GetTitle();
		bool GetUzelType();
		void TurnOn();
		void TurnOff();

	protected:
		uint8_t _pinAutomat;
		uint8_t _pinContactor;
		bool _unitType;
		bool _logicType;
		bool _initialized;
		unsigned long _millsCheck;

	private:
		String	_title;
		uint8_t	_state;
		//uint8_t	_prevState;
		unsigned int _timeOutOn;
		unsigned int _timeOutOff;
		uint8_t CheckAutomatState();
	};
#endif

