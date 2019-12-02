#include "ShomCanBus.h"

// =========   CONSTRUCTORS   =========

ShomCanBus::ShomCanBus() : Unit("ShomCanBus", UT_CANBUS)
	{
	Serial.begin(115200);
  	_canbus_id = 0;
	_title = "ShomCanBus(id="+ String(_canbus_id) + ")";
	Log("Init 0");
	}


// ------------------------------------
void ShomCanBus::Init(byte id, byte pin_ss)
	{
	if(_state != CBS_ON)
		{
		Log("Init");
		_canbus_id = id;
		_title = "ShomCanBus(id="+ String(_canbus_id) + ")";
		_canbus_pin_ss = pin_ss; 
		canbus=MCP_CAN(_canbus_pin_ss);
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
	/*else
		{
		Log("Init is done already");
		}*/
	}
		

// ------------------------------------
void ShomCanBus::Log(String str)
	{
	Serial.println(_title + " : "+ str);
	}
	
// ------------------------------------
void ShomCanBus::SetErrState(UnitError err)
	{
	LogErr(err);
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
byte ShomCanBus::GetDataByte(byte i)  // index
	{
	return _data_buffer[i];
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
	String str = GetCmdTitle(_data_buffer[0]) + "; ";
	for (int i = 1; i < DATA_LENGHT; i++)
		{
		str = str + String(_data_buffer[i]) + "; ";
		}
	Log(str);
	}

// ------------------------------------
void ShomCanBus::LogInfo()
	{
	String str = "Pin SS=" + String(_canbus_pin_ss);
	Log(str);
	}

// ------------------------------------
String ShomCanBus::GetCmdTitle(CanBusCmd cmd)
	{
	String res = "";
	if(cmd == CANBUS_READ)
	 	res = "CANBUS_READ";
	else if(cmd == CANBUS_WRITE) 
	 	res = "CANBUS_WRITE";
	else if(cmd == CANBUS_MODE)
	 	res = "CANBUS_MODE";
	else if(cmd == CANBUS_RESPONSE)
	 	res = "CANBUS_RESPONSE";
	else if(cmd == CANBUS_NOPE)
	 	res = "CANBUS_NOPE";
	else if(cmd == CANBUS_RESET)
	 	res = "CANBUS_RESET";
	else 
	 	res = "Unknown";
	return res; 
	}
	
// ------------------------------------
unsigned int ShomCanBus::GetMsgId()
	{
	_msgId = _msgId + 1;
	return _msgId;
	}
