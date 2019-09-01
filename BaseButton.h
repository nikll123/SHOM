#ifndef BaseButton_h
	#define BaseButton_h
	
	#include "Arduino.h"
	#include "Core.h"

	class BaseButton
	{
	public:
	    BaseButton();
	    BaseButton(String title, uint8_t pinButton, LogicType logicType);
		void LogState(String txt);
				
		KeyState 			CheckState();
		KeyState 			GetState();
		String 				GetStateText();
		String 				GetTitle();

	protected:
		KeyState 			_checkState();

	private:
		uint8_t 	_pinButton;
		String		_title;
		KeyState	_state;
		LogicType	_logicType;
	};
#endif

