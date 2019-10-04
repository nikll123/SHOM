#ifndef BaseButton_h
	#define BaseButton_h
	
	#include "Arduino.h"
	#include "Core.h"
	
	class BaseButton
	{
	public:
	    BaseButton();
	    BaseButton(String title, uint8_t pinButton, LogicType logicType);
	    
		InState2 	CheckState();
		InState		GetState();
		ButtonInfo 	GetInfo();

	protected:
		void 		_refreshState();

	private:
		uint8_t 	_pinButton;
		String		_title;
		InState	_state;
		LogicType	_logicType;
	};
#endif

