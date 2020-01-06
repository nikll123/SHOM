#ifndef Contactor_h
	#define Contactor_h
	
	#include "Unit.h"
	#include "PinIn.h"
	#include "PinOut.h"

	enum ContactorState{ 
						CS_NOTINIT			= 0,
						CS_ON				= 1,
						CS_OFF				= 2,
						CS_STARTING1		= 3,
						CS_STARTING2		= 4,
						CS_STOPPING1		= 5,
						CS_STOPPING2		= 6,
						CS_UNKNOWN			= 7,
						CS_ERR				= 100,
						CS_HALT				= 199,
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
					 
	static LogicType LogicTypeIn = LT_NORMAL;
	static LogicType LogicTypeOut = LT_NORMAL;
	
	class Contactor : public Unit
	{
	public:
					    Contactor();
					    Contactor(String title, uint8_t pinIn, uint8_t pinOut);

		ContactorState2	GetState();
		ContactorState2	GetState(String logTexts);
		String			GetStateTxt();		
		ContactorInfo 	GetInfo();
		static String	GetContactorStateText(ContactorState state);
		void			Init();
		void			LogInfo();
		void			SetErrState(UnitError err);
		void			SetErrState(UnitError err, String msg);
		PinIn			KeyIn;		//Why not PinIn? PinIn is already used as a class name
		PinOut			KeyOut;
		static void 	SetupLogic(LogicType ltIn, LogicType ltOut);

		void TurnOn();
		void TurnOff();
		void Halt();

	protected:
		void LogKeysState();
		
	private:
		ContactorState2	_getStateAutomat();
		ContactorState2	_getStateContactor();
		ContactorState	_state = CS_UNKNOWN;
		PinState        _stateIn = KS_NONE;
		PinState        _stateOut = KS_NONE;
		void 			_logIfChanged(ContactorState2 cs2);
		unsigned int 	_timeOutOn = 3000;
		unsigned int 	_timeOutRelayOn = 50;
		unsigned int 	_timeOutOff = 0;
		void 			_Turn(ContactorState csNew);
	};
#endif

