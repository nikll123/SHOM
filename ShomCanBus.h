#ifndef ShomCanBus_h
	#define ShomCanBus_h

	#include "mcp_can.h"
	
	
	#define CREATE_TRY_MAX 10
	#define DATA_LENGHT 4

  	enum CanBusCmd	{ 
					CANBUS_READ,
					CANBUS_WRITE,
					CANBUS_MODE
					};

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
	class ShomCanBus 
	{
	public:
						ShomCanBus();
						//ShomCanBus(String title, uint8_t pin_ss, unsigned long canbus_id);
		MCP_CAN     	canbus;
		void 			Log(String str);
		void 			Init(byte id, byte pin_ss);
		void			SetErrState(CanBusState err);
		void			Send();
		unsigned char	Receive();
		void 			SetDataByte(byte i, byte data);
		byte 			GetDataByte(byte i);
		void 			ResetData();
		void			LogData();
		void			LogInfo();
		String			GetCmdTitle(CanBusCmd cmd);
								
	protected:

	private:
		unsigned long 	_canbus_id;
		byte 			_canbus_pin_ss;
		String			_title;
		CanBusState 	_state = CBS_UNKNOWN;
		byte 			_data_buffer[DATA_LENGHT];
	};
	    
#endif

