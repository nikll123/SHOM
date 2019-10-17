#ifndef Conveyor_h
	#define Conveyor_h

	#include "Unit.h"

	#define MAX_UNIT_NUMBER 10
	
	class Conveyor : Unit 
	{
	public:
	    Conveyor();
	    Conveyor(String title, uint8_t pin_button_on, uint8_t _pin_button_off, uint8_t pin_button_reset);
		Unit units[];

	private:
	};
#endif

