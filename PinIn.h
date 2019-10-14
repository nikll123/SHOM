#ifndef PinIn_h
	#define PinIn_h
	
	#include "Pin.h"

   	//-------------------------------
	struct PinInInfo {
					String 		Title;
					String 		UnitType;
					String 		State;
					uint8_t		Pin;
					String		Logic;
					};	

	//-------------------------------
	enum LogicType {
					LT_NONE,
					LT_NORMAL, 
					LT_INVERSE
					};	

   	//-------------------------------
	class PinIn : public Pin
	{
	public:
	    PinIn();
	    PinIn(String title, uint8_t pin);
	    
		PinState2	GetState();
		PinInInfo 	GetInfo();
		void 		LogicInverse();
		void 		LogicNormal();
		void 		LogState();

		void		LogInfo();
		String 		GetLogicTypeText();

	protected:
		void		_setLogicType(LogicType);
		void 		_refreshState();
		LogicType	_logicType; 

	private:
	};
#endif

