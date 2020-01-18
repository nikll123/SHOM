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
            static void             CheckConnection();
    		static ShomCanBus		CanBus;
            static Unit             Timer;
    
    		static int				DoCmd();
		    static void 			Run();

    	protected:
    		
    	private:
         
    	};


#endif