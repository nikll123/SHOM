#ifndef Conveyor_h
	#define Conveyor_h

	#include "Uzel.h"

	#define MAX_UNIT_NUMBER 10

enum ConveyorState 
				{
				CNV_UNKNOWN,
				CNV_ON,
				CNV_OFF
				};
	
struct	ConveyorInfo 
				{
				String   		Title;
				String	 		UnitType;
				String			State;
				uint8_t			PinOn;
				uint8_t     	PinOff;
				uint8_t     	PinReset;
				};
	
	class Conveyor : Unit 
	{
	public:
	    				Conveyor();
			    		Conveyor(String title, uint8_t pin_button_on, uint8_t _pin_button_off, uint8_t pin_button_reset);
		ConveyorInfo 	GetInfo();
		void 			LogInfo();
		String 			GetConvStateText(ConveyorState state);

		Uzel Uzelki[MAX_UNIT_NUMBER];

	private:
			ConveyorState	_state;
			uint8_t			_pinBtnOn;
			uint8_t     	_pinBtnOff;
			uint8_t     	_pinBtnReset;
	};
#endif

