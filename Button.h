#ifndef Button_h
	#define Button_h
	
	#include "PinIn.h"

	class Button : public PinIn
	{
	public:
	    Button();
	    Button(String title, uint8_t pin);

	protected:

	private:
	};
#endif

