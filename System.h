#ifndef System_h
	#define System_h

	
	#define MAX_UNIT_NUMBER 15

	#include "Conveyor.h"

enum SystemState 
				{
				SS_NOTINIT		= 1,
				SS_UNKNOWN		= 2,
				SS_ON			= 3,
				SS_OFF			= 4,
				SS_STARTING		= 5,
				SS_STOPPING		= 6,
				SS_ERR300		= 300,
				SS_ERR301		= 301,
				SS_ERR302		= 302,
				SS_ERR303		= 303,
				SS_ERR304		= 304,
				SS_ERR305		= 305,
				SS_ERR306		= 306,
				SS_ERR307		= 307,
				SS_ERR308		= 308,
				SS_ERR309		= 309,
				};
				
struct	SystemState2 
				{
				SystemState		Old;
				SystemState		New;
				};
					
struct	SystemInfo 
				{
				String   		Title;
				String	 		UnitType;
				String			State;
				uint8_t			PinOn;
				uint8_t     	PinOff;
				uint8_t     	PinReset;
				};
	
	class System : Unit 
	{
	public:
	    				System();
			    		System(String title, uint8_t pin_button_on, uint8_t _pin_button_off, uint8_t pin_button_reset);
		void 			Init();
		SystemInfo 		GetInfo();
		void 			LogInfo();
		void 			LogInfo(bool conv);
		void			Log(String str);
		void 			SetupConveyor(String title, uint8_t pinIn, uint8_t pinOut, uint8_t pinAuto, uint8_t pinLed);
		PinIn 			SetupButton(String suffix, uint8_t pin);
		String 			GetSystemStateText(SystemState state);
		uint8_t			UnitCount = 0;

		Conveyor 		Conveyors[MAX_UNIT_NUMBER];
		ConveyorState2	ConveyorStates[MAX_UNIT_NUMBER];
		PinIn			BtnOn;
		PinIn			BtnOff;
		PinIn			BtnReset;

		void 			Start();
		void 			Stop();
		void 			TurnOff();
		void 			TurnOffAlarm();
		//void 			Test();
		SystemState2	GetState();

	private:
		void 			_setState(SystemState state);
		void 			_ifChanged(SystemState2 cs2);
		void 			_logStates(SystemState2 cs2);
		void 			_updateConveyorStates();
		SystemState		_checkStateOff();
		SystemState		_checkStateOn();
		SystemState		_checkStateStarting();
		SystemState		_checkStateStopping();
		SystemState		_state = SS_UNKNOWN;
		SystemState 	_calcState(int cntErr, int cntOn, int cntOff, int cntStoping, int cntStarting);
		
	};
#endif

