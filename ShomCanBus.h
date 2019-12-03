#ifndef ShomCanBus_h
	#define ShomCanBus_h

	#include "mcp_can.h"
	#include "Unit.h"
	
	
	#define CREATE_TRY_MAX 10
	#define DATA_LENGHT 	5
	#define DATA_ID_HIGH 	0
	#define DATA_ID_LOW 	1
	#define DATA_CMD 		2
	#define DATA_PIN 		3
	#define DATA_VALUE	 	4
//  data structure
//	0			1			2		3		4
//	msgIdHigh	msgIdLow	Cmd		Pin		Data

  	enum CanBusCmd	{ 
					CANBUS_READ,
					CANBUS_WRITE,
					CANBUS_MODE,
					CANBUS_RESPONSE,
					CANBUS_RESET,
					CANBUS_NOPE
					};

  	enum CanBusState{ 
					CBS_UNKNOWN		= 1,
					CBS_ON			= 2,
					CBS_ERR			= 400,
				};
	
	//-------------------------------
	class ShomCanBus : Unit
	{
	public:
						ShomCanBus();
						//ShomCanBus(String title, uint8_t pin_ss, unsigned long canbus_id);
		MCP_CAN     	canbus;
		void 			Log(String str);
		void 			Init(byte id, byte pin_ss);
		void			SetErrState(UnitError err);
		void			Send();
		unsigned char	Receive();
		void 			SetDataByte(byte i, byte data);
		byte 			GetDataByte(byte i);
		void 			ResetData();
		void			LogData();
		void			LogInfo();
		String			GetCmdTitle(CanBusCmd cmd);
		unsigned int 	GetMsgId();
								
	protected:

	private:
		byte		 	_canbus_id;
		byte 			_canbus_pin_ss;
		unsigned int 	_msgId;
		String			_title;
		CanBusState 	_state = CBS_UNKNOWN;
		byte 			_data_buffer[DATA_LENGHT];
	};
	    
#endif

