#ifndef BaseButton_h
	#define BaseButton_h
	
	#include "Arduino.h"
	#include "Core.h"
	
	class BaseButton
	{
	public:
	    BaseButton();
	    BaseButton(String title, uint8_t pinButton, LogicType logicType);
	    
		InputState2 	CheckState();
		InputState	GetState();
		KeyInfo 	GetInfo();

	protected:
		void 		_checkState();

	private:
		uint8_t 	_pinButton;
		String		_title;
		InputState	_state;
		LogicType	_logicType;
	};
#endif

