#ifndef Contactor_h
	#define Contactor_h
	
	#define TURN_ON_TIMEOUT 3000
	#define TURN_OFF_TIMEOUT 0 //2000
	
	#include "Unit.h"
	#include "PinIn.h"
	#include "PinOut.h"

	enum ContactorState{ 
						CS_NOTINIT	= 0,
						CS_ON		= 1,
						CS_OFF		= 2,
						CS_STARTING	= 3,
						CS_STOPPING	= 4,
						CS_UNKNOWN	= 5,
						CS_ERR			= 100,
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
						CS_HALT			= 199,
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
					bool			Active;
					unsigned int 	TimeOutOn;
					unsigned int 	TimeOutOff;
					}; 

	
	class Contactor : public Unit
	{
	public:
					    Contactor();
					    Contactor(String title, uint8_t pinIn, uint8_t pinOut);
					    Contactor(String title, uint8_t pinIn, uint8_t pinOut, unsigned long timeOutOn, unsigned long  timeOutOff);

		ContactorState2	GetState();
		ContactorState2	GetState(String logTexts);
		ContactorInfo 	GetInfo();
		String			GetContactorStateText(ContactorState state);
		void			Init();
		void			LogInfo();
		void 			Log(String str);
		void 			SetErrState(ContactorState state);
		void 			FixTime(bool x);
		PinIn			KeyIn;		//Why not PinIn? PinIn is already used as a class name
		PinOut			KeyOut;

		void TurnOn();
		void TurnOff();
		void Halt();

	protected:
		void LogKeysState();
		
	private:
		ContactorState2	_getStateAutomat();
		ContactorState2	_getStateContactor();
		ContactorState	_state = CS_UNKNOWN;
		void 			_ifChanged(ContactorState2 cs2);
		unsigned int 	_timeOutOn = 0;
		unsigned int 	_timeOutOff = 0;
		unsigned long 	_millsCheck = 0;
		void 			_Turn(ContactorState csNew);
	};
#endif

