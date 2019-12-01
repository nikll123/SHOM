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
	    
		PinState2	GetState();
		PinInInfo 	GetInfo();
		void 		Init();
		void 		LogState();
		void 		LogicInInverse();
		void 		LogicInNormal();

		void		LogInfo();

	protected:
		void		_setLogicType(LogicType);
		void 		_refreshState();

	private:
	};
#endif

