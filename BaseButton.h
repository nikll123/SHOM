#ifndef BaseButton_h
	#define BaseButton_h
	
	//#include "Arduino.h"
	#include "Unit.h"
	
   	//-------------------------------
	enum InState {KS_NONE, 
					KS_ON, 
					KS_OFF,
					};	

	//-------------------------------
	enum LogicType {LT_NONE,
					LT_NORMAL, 
					LT_INVERSE
					};	

	//-------------------------------
	struct InState2 {
					InState 	Old;
					InState 	New;
					};	

   	//-------------------------------
	struct ButtonInfo {
					InState 	Value;
					String 		Title;
					String 		UnitType;
					String 		State;
					uint8_t		Pin;
					String 		Logic;
					uint8_t		PinLed;
					};	

   	//-------------------------------
	class BaseButton : public Unit
	{
	public:
	    BaseButton(String title, uint8_t pinButton, LogicType logicType);
	    BaseButton(String title, uint8_t pinButton, LogicType logicType, uint8_t _pinLed);
	    
		InState2 	CheckState();
		InState		GetState();
		ButtonInfo 	GetInfo();
		String 		GetLogicTypeText();
		void		LogInfo();

	protected:
		String 		GetInStateText(InState);
		uint8_t 	_pinButton;
		void 		_refreshState();
		uint8_t 	_pinLed;
		InState		_in_state;
		LogicType	_logicType;

	private:
	};
#endif

