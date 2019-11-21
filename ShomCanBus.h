#ifndef ShomCanBus_h
	#define ShomCanBus_h

	#include "mcp_can.h"
	#include "ShomCanBusCfg.h"
	//-------------------------------
	class ShomCanBus 
	{
	public:
						ShomCanBus();
						//ShomCanBus(String title, uint8_t pin_ss, unsigned long canbus_id);
		MCP_CAN     	canbus;
		void 			Log(String str);
								
	protected:

	private:
 
	};
	    
#endif

