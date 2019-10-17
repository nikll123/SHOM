#ifndef Contactor_h
	#define Contactor_h
	
	#define TURN_ON_TIMEOUT 3000
	#define TURN_OFF_TIMEOUT 2000
	
	#include "Unit.h"
	#include "Led.h"
	#include "PinIn.h"
	#include "PinOut.h"
	
	enum ContactorState{ 
						CS_NOTINIT	= 0,
						CS_ON		= 1,
						CS_OFF		= 2,
						CS_STARTING	= 3,
						CS_STOPPING	= 4,
						CS_UNKNOWN	= 5,
						CS_ERR		= 100,
						CS_ERR1		= 101,
						CS_ERR2		= 102,
						CS_ERR3		= 103,
						CS_ERR4		= 104,
						CS_ERR5		= 105,
						CS_ERR6		= 106,
						CS_ERR7		= 107,
						CS_ERR8		= 108,
						CS_ERR9		= 109,
						CS_ERR10	= 110,
						};
						
	struct ContactorState2
					{
					ContactorState Old;  
					ContactorState New;
					
					ContactorState2(ContactorState csOld = CS_NOTINIT, ContactorState csNew = CS_NOTINIT):Old(csOld ),New(csNew ){};  
					}; 
	
    struct ContactorInfo	
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

	
	class Contactor : public Unit
	{
	public:
					    Contactor();
					    Contactor(String title, uint8_t pinOut, uint8_t pinIn);
					    Contactor(String title, uint8_t pinOut, uint8_t pinIn, uint8_t pinLed);
					    Contactor(String title, uint8_t pinOut, uint8_t pinIn, uint8_t pinLed, unsigned int timeOutOn, unsigned int timeOutOff);


		ContactorState2	GetState();
		ContactorInfo 	GetInfo();
		String			GetContactorStateText(ContactorState state);
		void			Init();
		void			LogInfo();
		void 			IfChanged(ContactorState2 cs2);
		Led				LedIndicator;
		PinIn			KeyIn;
		PinOut			KeyOut;


		void TurnOn();
		void TurnOff();
		void TurnOffAlarm();

	protected:
		void LogKeysState();
		
	private:
		ContactorState	_state;
		unsigned int 	_timeOutOn;
		unsigned int 	_timeOutOff;
		unsigned long 	_millsCheck;
		void 			_Turn(ContactorState csNew);
	};
#endif

