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
		canbus_ok = (CAN_OK == canbus.begin(CANBUS_RATE));
		for (int i = 0; i<CREATE_TRY_MAX; i++)
			{
			canbus_ok = (CAN_OK == canbus.begin(CANBUS_RATE));
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
	}
		

// ------------------------------------
void ShomCanBus::Log(String str)
	{
	Serial.println(_title + " : "+ str);
	}
	
// ------------------------------------
void ShomCanBus::SetErrState(UnitError err)
	{
	LogData();
	LogErr(err);
	_state = CBS_ERR;
	}
	
// ------------------------------------
void ShomCanBus::SetErrState(UnitError err, String str)
	{
	Log(str);
	SetErrState(err);
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
	LogData("");
	}

// ------------------------------------
void ShomCanBus::LogData(String comment)
	{
	unsigned int _id = GetMsgId();
	String str = comment + " id=" + String(_id) + "; ";
	str = str + GetCmdTitle(_data_buffer[DATA_CMD]) + "; ";
	str = str + String(_data_buffer[DATA_PIN]) + "; ";
	str = str + String(_data_buffer[DATA_VALUE]);
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
unsigned int ShomCanBus::NewMsgId()
	{
	_msgId++;
	if(_msgId == 0) 
		_msgId = 1;
	return _msgId;
	}

// ------------------------------------
unsigned int ShomCanBus::SendCmd(CanBusCmd cmd, byte pin)
	{
	return SendCmd(cmd, pin, 0);
	}

// ------------------------------------
unsigned int ShomCanBus::SendCmd(CanBusCmd cmd, byte pin, bool value)
	{
	return SendCmd(0, cmd, pin, value);
	}

// ------------------------------------
unsigned int ShomCanBus::SendCmd(unsigned int id, CanBusCmd cmd, byte pin, bool value)
	{
		ResetData();
		if (id == 0)
			id = NewMsgId();
		byte idh = highByte(id);
		byte idl = lowByte(id);
		SetDataByte(DATA_ID_HIGH, idh);
		SetDataByte(DATA_ID_LOW, idl);
		SetDataByte(DATA_CMD, cmd);
		SetDataByte(DATA_PIN, pin);
		SetDataByte(DATA_VALUE, value);
		Send();
		//LogData();
		return id;
	}

// ------------------------------------
CanBusState	ShomCanBus::GetResponse(unsigned int id)
	{
	CanBusState res = CBS_ERR; 
	bool received = false;
	byte len = 0;
	byte tryId = 0;
	for (int i=0; i < RESPONSE_TRY_CNT; i++)
		{
		delay(RESPONSE_DELAY);
		len = Receive();
		//Log("i=" + String(i) + " len=" + String(len));
		//LogData();
		received = (len > 0);
		if(received)
			{
			if (len == DATA_LENGHT)
				{
				unsigned int _id = GetMsgId();
				if (_id == id)
				    {
					CanBusCmd cmd = GetDataByte(DATA_CMD);
					if(CANBUS_RESPONSE == cmd)
						{
						byte data = GetDataByte(DATA_VALUE);
						if(data == 0)
							res = CBS_LOW;
						else if(data == 1)
							res = CBS_HIGH;
						else 
							SetErrState(KS_ERR501, "Error: Wrong data " + String(data));
						}
					else
						SetErrState(KS_ERR503, "Error: Wrong cmd " + String(cmd));
	
	                received = (res == CBS_HIGH || res == CBS_LOW);
	                
					if(received)
						break;
					}
				else
					SetErrState(KS_ERR506, "Error: id expected " + String(id) + ", got " + String(_id));
				}
			else
				SetErrState(KS_ERR504, "wrong data lenght = " + String(len));
            }
		tryId++;
		}
	if(!received)
		{
		SetErrState(KS_ERR505, "No data received");
		}
	else if (tryId > 0) 
		{
		Log("tryId = " + String(tryId));
		}
	return res;
	}
	
// ------------------------------------
unsigned int ShomCanBus::GetMsgId()
	{
	byte idh = GetDataByte(DATA_ID_HIGH);
	byte idl = GetDataByte(DATA_ID_LOW);
	unsigned int res = 0;
	for (int i = 0; i < 8; i++)
		{
		bitWrite(res, i, bitRead(idl, i));
		}
	for (int i = 8; i < 16; i++)
		{
		bitWrite(res, i, bitRead(idh, i - 8));
		}
	return res;
	}
	