#ifndef BaseButton_h
	#define BaseButton_h
	
	#include "Arduino.h"
	#include "Core.h"
	
	class BaseButton
	{
	public:
	    BaseButton();
	    BaseButton(String title, uint8_t pinButton, LogicType logicType);
	    
		KeyState2 	CheckState();
		KeyInfo 	GetInfo();
		String 		GetStateText();
		String 		GetTitle();
		String 		GetKeyStateText();

	protected:
		void 		_checkState();

	private:
		uint8_t 	_pinButton;
		String		_title;
		KeyState1	_state;
		LogicType	_logicType;
	};
#endif

