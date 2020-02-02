/*
SLAVE unit is controlled by the System (MASTER) unit.  
*/

#ifndef Slave_h
	#define Slave_h

	#include "ShomCanBus.h"
    #define CANBUS_TIMEOUT 1000
    #define DEFAULTSTATE   1

	//-------------------------------
	class Slave
    	{
    	public:
    								Slave();
    		static ShomCanBus		CanBus;
            static Unit             Timer;
    
    		static int				DoCmd();
		    static void 			Run();
            static void             CheckConnection();

    	protected:
    		
    	private:
         
    	};


#endif