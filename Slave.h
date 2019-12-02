#ifndef Slave_h
	#define Slave_h

	#include "ShomCanBus.h"

	//-------------------------------
	class Slave
	{
	public:
								Slave();
		static ShomCanBus		CanBus;

		static int				DoCmd();

	protected:

	private: 
	};


#endif