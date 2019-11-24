#ifndef PinOut_h
	#define PinOut_h
	
	#include "Pin.h"

   	//-------------------------------
	struct PinOutInfo {
					String 		Title;
					String 		UnitType;
					String 		State;
					uint8_t		Pin;
					};	
	
   	//-------------------------------
	class PinOut : public Pin
	{
	public:
				    PinOut();
				    PinOut(String title, uint8_t pin);
		void		Init();
		void		SetOn();
		void		SetOff();
		void		SetOn(bool noLog);
		void		SetOff(bool noLog);
		void		Inverse();
		PinOutInfo 	GetInfo();
		void 		LogInfo();
		void 		LogState();


	protected:

	private:
	};
#endif

