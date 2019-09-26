#ifndef Conveyor_h
	#define Conveyor_h

	#include "Arduino.h"
	#include "BaseUzel.h"
	#include "BaseButton.h"

	#define KOLICHESTVO_UZLOV 10
	
	struct ConveyorInfo	{
				ConveyorState 	State;
				String			Title;
				}; 
	
	
	
	class Conveyor 
	{
	public:
	    Conveyor(String title, uint8_t pin_button_on, uint8_t _pin_button_off, uint8_t pin_button_reset);
	    
		ConveyorState GetState();
		ConveyorState2 CheckState();

        ConveyorInfo GetInfo();
        
		//String GetStateTxt();
		//String GetTitle();
		String GetTime();

		void ReadUzelStates();
		void SetupUzelAutomat(uint8_t, String, uint8_t);
		void SetupUzelAutomatInverse(uint8_t, String, uint8_t);
		void SetupUzelContactor(uint8_t, String, uint8_t, uint8_t);
		void SetupUzelContactor(uint8_t, String, uint8_t, uint8_t, unsigned int, unsigned int);
		void SetupUzelContactorInverse(uint8_t, String, uint8_t, uint8_t);
		void SetupUzelContactorInverse(uint8_t, String, uint8_t, uint8_t, unsigned int, unsigned int);
		void Reset();
		void LogText(String);
		void LogTextln(String);

		bool ButtonOnIsPressed();
		bool ButtonOffIsPressed();
		bool ButtonResetIsPressed();
		
		BaseUzel Uzelki[KOLICHESTVO_UZLOV]= 
			{
				BaseUzel(), 
				BaseUzel(), 
				BaseUzel(), 
				BaseUzel(), 
				BaseUzel(), 
				BaseUzel(), 
				BaseUzel(), 
				BaseUzel(), 
				BaseUzel(), 
				BaseUzel() 
			};

		ConveyorState TurnOn();
		ConveyorState TurnOff();
	private:
		//uint8_t 		_currentState;
		//uint8_t 		_keyUnitCount;
		ConveyorState	_state;
		String      	_title;
		TurnOnUzelAction 		TurnOn_TurnOn_NextAction(UzelState prevUnitState, UzelState currUnitState);
		TurnOffUzelAction 		TurnOff_NextAction(UzelState prevUnitState, UzelState currUnitState);
	};
#endif

