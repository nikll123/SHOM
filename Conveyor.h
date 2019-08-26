#ifndef Conveyor_h
	#define Conveyor_h

	//#include "Arduino.h"
	//#include "Define.h"
	#include "BaseUzel.h"
	#include "Button.h"
	//#include "Automat.h"
	//#include "Contactor.h"

	#define KOLICHESTVO_UZLOV 10
	
	class Conveyor 
	{
	public:
	    Conveyor(String title, uint8_t pin_button_on, uint8_t _pin_button_off, uint8_t pin_button_reset);
		uint8_t GetState();
		String GetStateTxt();
		String GetTitle();
		void GetUzelStates(uint8_t * result);
		void ReadUzelStates();
		void SetupUzelAutomat(uint8_t index, String title, uint8_t pinAutomat);
		void SetupUzelAutomatInverse(uint8_t index, String title, uint8_t pinAutomat);
		void SetupUzelContactor(uint8_t index, String title, uint8_t pinAutomat, uint8_t pinContactor);
		void SetupUzelContactor(uint8_t index, String title, uint8_t pinAutomat, uint8_t pinContactor, unsigned int timeOutOn, unsigned int timeOutOff);
		void SetupUzelContactorInverse(uint8_t index, String title, uint8_t pinAutomat, uint8_t pinContactor);
		void SetupUzelContactorInverse(uint8_t index, String title, uint8_t pinAutomat, uint8_t pinContactor, unsigned int timeOutOn, unsigned int timeOutOff);
		bool ButtonOnIsPressed();
		bool ButtonOffIsPressed();
		bool ButtonResetIsPressed();
		void Reset();
		
		//Button ButtonOff("  ", 0);
		//Automat automat1;
		
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

		//uint8_t UzelState[KOLICHESTVO_UZLOV]={0,0,0,0,0,0,0,0,0,0};
		
		uint8_t TurnOn();
		uint8_t TurnOff();
	private:
		uint8_t 	_currentState;
		uint8_t 	_keyUnitCount;
		uint8_t 	_state;
		String      _title;
		Button 		_buttonOn; 
		Button 		_buttonOff;
		Button 		_buttonReset;
		uint8_t 	TurnOn_CheckUnitStates(uint8_t prevUnitState, uint8_t currUnitState);
		uint8_t 	TurnOff_CheckUnitStates(uint8_t prevUnitState, uint8_t currUnitState);
				 
	};
#endif

