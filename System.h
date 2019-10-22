#ifndef System_h
	#define System_h

	#include "Conveyor.h"

	#define MAX_UNIT_NUMBER 10

enum SystemState 
				{
				CNV_UNKNOWN,
				CNV_ON,
				CNV_OFF
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
		SystemInfo 	GetInfo();
		void 			LogInfo();
		void 			SetupConveyor(uint8_t index, String title, uint8_t pinIn, uint8_t pinOut, uint8_t pinAuto, uint8_t pinLed);
		String 			GetConvSystemText(SystemState state);

		Conveyor 		Conveyors[MAX_UNIT_NUMBER];
		

	private:
			SystemState	_state;
			uint8_t			_pinBtnOn;
			uint8_t     	_pinBtnOff;
			uint8_t     	_pinBtnReset;
	};
#endif

