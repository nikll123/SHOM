#ifndef Automat_h
	#define Automat_h
	
	#include "PinIn.h"

	class Automat : public PinIn
	{
	public:
	    Automat();
	    Automat(String title, uint8_t pin);
	    Automat(String title, uint8_t pin, LogicType lt);

	protected:

	private:
	};
#endif

