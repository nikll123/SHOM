#include "ShomCanBus.h"

// =========   CONSTRUCTORS   =========

ShomCanBus::ShomCanBus()
	{
	Serial.begin(115200);
  	canbus=MCP_CAN(CANBUS_PIN_SS);
  	_canbus_id = CANBUS_ID;
	_title = "ShomCanBus";
	Log("Init 1");
	}


// ------------------------------------
void ShomCanBus::Begin()
	{
	if(_state != CBS_ON)
		{
		Log("Init");
		bool canbus_ok = false;
		canbus_ok = (CAN_OK == canbus.begin(CAN_50KBPS));
		for (int i = 0; i<CREATE_TRY_MAX; i++)
			{
			canbus_ok = (CAN_OK == canbus.begin(CAN_50KBPS));
			if(canbus_ok)
				break;
			else
				SetErrState(CBS_ERR401);
				
			delay(100);
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
	else
		{
		Log("Init is done already");
		}
	}
		

// ------------------------------------
void ShomCanBus::Log(String str)
	{
	Serial.println(_title + " : "+ str);
	}
	
// ------------------------------------
void ShomCanBus::SetErrState(CanBusState err)
	{
	Log("CBS_ERR" + String(err));
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
void ShomCanBus::SetDataByte(byte i, byte data)  // index, data
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
