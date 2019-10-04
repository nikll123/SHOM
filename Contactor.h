#ifndef Contactor_h
	#define Contactor_h
	
	//#include "Arduino.h"
	#include "Core.h"
	#include "BaseUzel.h"

	class Contactor : public BaseUzel
	{
	public:
         Contactor(String title, uint8_t pinInput, uint8_t pinOutput);
         Contactor(String title, uint8_t pinInput, uint8_t pinOutput, unsigned int timeOutOn, unsigned int timeOutOff);
         
	protected:

	private:

	};
#endif

