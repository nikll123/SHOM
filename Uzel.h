#ifndef Uzel_h
	#define Uzel_h
	
	#define TURN_ON_TIMEOUT 3000
	#define TURN_OFF_TIMEOUT 2000
	
	#include "Unit.h"
	#include "Led.h"
	#include "PinIn.h"
	#include "PinOut.h"

	enum UzelState{ 
						CS_NOTINIT	= 0,
						CS_ON		= 1,
						CS_OFF		= 2,
						CS_STARTING	= 3,
						CS_STOPPING	= 4,
						CS_UNKNOWN	= 5,
						CS_ERR100		= 100,
						CS_ERR101		= 101,
						CS_ERR102		= 102,
						CS_ERR103		= 103,
						CS_ERR104		= 104,
						CS_ERR105		= 105,
						CS_ERR106		= 106,
						CS_ERR107		= 107,
						CS_ERR108		= 108,
						CS_ERR109		= 109,
						CS_ERR110		= 110,
					};
						
	struct UzelState2
					{
					UzelState Old;  
					UzelState New;
					
					UzelState2(UzelState csOld = CS_NOTINIT, UzelState csNew = CS_NOTINIT):Old(csOld ),New(csNew ){};  
					}; 
	
    struct UzelInfo	
					{
					String   		Title;
					String	 		UnitType;
					String			State;
					uint8_t			PinIn;
					uint8_t     	PinOut;
					uint8_t     	PinLed;
					bool			Active;
					unsigned int 	TimeOutOn;
					unsigned int 	TimeOutOff;
					}; 

	
	class Uzel : public Unit
	{
	public:
					    Uzel();
					    Uzel(UnitType ut, String title, uint8_t pinIn, uint8_t pinOut);
					    Uzel(UnitType ut, String title, uint8_t pinIn, uint8_t pinOut, uint8_t pinLed);
					    Uzel(UnitType ut, String title, uint8_t pinIn, uint8_t pinOut, uint8_t pinLed, unsigned long timeOutOn, unsigned long  timeOutOff);

		UzelState2	GetState();
		UzelInfo 	GetInfo();
		String			GetUzelStateText(UzelState state);
		void			Init();
		void			LogInfo();
		void 			IfChanged(UzelState2 cs2);
		Led				LedIndicator;
		PinIn			KeyIn;
		PinOut			KeyOut;


		void TurnOn();
		void TurnOff();
		void TurnOffAlarm();

	protected:
		void LogKeysState();
		
	private:
		UzelState2	_getStateAutomat();
		UzelState2	_getStateContactor();
		UzelState	_state;
		unsigned int 	_timeOutOn;
		unsigned int 	_timeOutOff;
		unsigned long 	_millsCheck;
		void 			_Turn(UzelState csNew);
	};
#endif

