#ifndef Contactor_h
	#define Contactor_h
	
	//#include "Arduino.h"
	#include "Core.h"
	#include "BaseUzel.h"

	class Contactor : public BaseUzel
	{
	public:
         Contactor(String title, uint8_t pinAutomat, uint8_t pinContactor);
         Contactor(String title, uint8_t pinAutomat, uint8_t pinContactor, unsigned int timeOutOn, unsigned int timeOutOff);
         
	protected:

	private:

	};
#endif

