/*
Class PinIn - for pins which are used in INPUT of INPUT_PULLUP modes  
*/
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
	    PinIn(const char * title, uint8_t pin);
	    PinIn(const char * title, uint8_t pin, LogicType lt);
	    
		PinState2	GetState();
		PinInInfo 	GetInfo();
		void 		LogState();

//		void		LogInfo();

	protected:
		void 		_refreshState();

	private:
	};
#endif

