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
	class PinIn : public Pin
	{
	public:
	    PinIn();
		PinIn(String title, uint8_t pin);
	    PinIn(String title, uint8_t pin, LogicType lt);
	    
		PinState2	GetState();
		String		GetStateTxt();
		PinInInfo 	GetInfo();
		void 		Init();
		void 		LogState();

		void		LogInfo();

	protected:
		void 		_refreshState();
		//void		_setLogicType(LogicType);

	private:
	};
#endif

