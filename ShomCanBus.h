#ifndef ShomCanBus_h
	#include "Unit.h"
	#include "mcp_can.h"
	#define ShomCanBus_h
	#define CREATE_TRY_MAX 10
	#define DATA_LENGHT 8
	

  	enum CanBusState{ 
					CBS_UNKNOWN		= 1,
					CBS_ON			= 2,
					CBS_ERR			= 400,
					CBS_ERR401		= 401,
					CBS_ERR402		= 402,
					CBS_ERR403		= 403,
					CBS_ERR404		= 404,
					CBS_ERR405		= 405
				};

	//-------------------------------
	class ShomCanBus : public Unit
	{
	public:
						ShomCanBus();
						ShomCanBus(String title, uint8_t pin_ss, unsigned long canbus_id);
		MCP_CAN     	canbus;
		void			SetErrState(CanBusState err);
		void			Send();
		unsigned char	Receive();
		void 			SetDataByte(byte data, byte i);
		void 			ResetData();
		void			LogData();						
	protected:

	private:
		CanBusState 	_state = CBS_UNKNOWN;
		unsigned long 	_canbus_id;
		byte 			_data_buffer[DATA_LENGHT];
 
	};
	    
#endif

