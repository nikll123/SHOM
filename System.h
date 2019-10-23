#ifndef System_h
	#define System_h

	#include "Conveyor.h"

	#define MAX_UNIT_NUMBER 20

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
		void 			SetupConveyor(uint8_t index, String title, uint8_t pinIn, uint8_t pinOut, uint8_t pinAuto, uint8_t pinLed);
		PinIn 			SetupButton(String suffix, uint8_t pin);
		String 			GetSystemStateText(SystemState state);

		Conveyor 		Conveyors[MAX_UNIT_NUMBER];
		PinIn			BtnOn;
		PinIn			BtnOff;
		PinIn			BtnReset;

		void 			Start();
		void 			TurnOff();
		void 			TurnOffAlarm();
		SystemState2	GetState();

	private:
		void 			_setState(SystemState state);
		void 			_ifChanged(SystemState2 cs2);
		void 			_logStates(SystemState2 cs2);
		SystemState		_state;
		
	};
#endif

