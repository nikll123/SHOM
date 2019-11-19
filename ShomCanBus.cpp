#include "ShomCanBus.h"

// =========   CONSTRUCTORS   =========
ShomCanBus::ShomCanBus() : Unit("dummy", UT_CANBUS)
	{
	}


ShomCanBus::ShomCanBus(String title, uint8_t pin_ss, unsigned long canbus_id) :  Unit(title, UT_CANBUS)
	{
	_logLevel = LL_NORMAL;
	_canbus_id = canbus_id; 
	canbus = MCP_CAN(pin_ss);
	int i = 1;
	bool canbus_ok = false;
	while (!canbus_ok)
		{
			canbus_ok = (CAN_OK == canbus.begin(CAN_50KBPS));
			
			if(canbus_ok || i > CREATE_TRY_MAX)
				break;
			else
				LogErr("CBS_ERR", CBS_ERR401);
			delay(100);
			i++;	
		}
	
	if (canbus_ok)
		{	
		_state = CBS_ON;
		Log("Init OK");
		}
	else
		{
		SetErrState(CBS_ERR402);
		}
	}

// ------------------------------------
void ShomCanBus::SetErrState(CanBusState err)
	{
	LogErr("CBS_ERR", err);
	_state = CBS_ERR;
	}


// ------------------------------------
void ShomCanBus::Send()
	{
    canbus.sendMsgBuf(_canbus_id, 0, DATA_LENGHT, _data_buffer);
	}

// ------------------------------------
unsigned char ShomCanBus::Receive()
	{
	unsigned char dataLen = 0;
 	if (CAN_MSGAVAIL == canbus.checkReceive())
	  	canbus.readMsgBuf(&dataLen, _data_buffer);

	return dataLen;  
	}

// ------------------------------------
void ShomCanBus::SetDataByte(byte data, byte i)
	{
	_data_buffer[i] = data;
	}

// ------------------------------------
void ShomCanBus::ResetData()
	{
	for (int i = 0; i < DATA_LENGHT; i++)
		{
		_data_buffer[i] = 0;
		}
	}

// ------------------------------------
void ShomCanBus::LogData()
	{
	String str = "_data_buffer ";
	for (int i = 0; i < DATA_LENGHT; i++)
		{
		str = str + String(_data_buffer[i]) + "; ";
		}
	Log(str);
	}
