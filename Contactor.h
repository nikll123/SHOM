#ifndef Contactor_h
	#define Contactor_h
	
	//#include "Arduino.h"
	#include "Core.h"
	#include "BaseUzel.h"

	class Contactor : public BaseUzel
	{
	public:
         Contactor(String title, uint8_t pinAutomat, uint8_t pinContactor);
         
	protected:

	private:

	};
#endif

