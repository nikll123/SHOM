#ifndef ContactorInverse_h
	#define ContactorInverse_h
	
//	#include "Arduino.h"
	#include "Core.h"
	#include "BaseUzel.h"

	class ContactorInverse : public BaseUzel
	{
	public:
         ContactorInverse(String title, uint8_t pinAutomat, uint8_t pinContactor);
         ContactorInverse(String title, uint8_t pinAutomat, uint8_t pinContactor, unsigned int timeOutOn, unsigned int timeOutOff);
         
	protected:

	private:

	};
#endif

