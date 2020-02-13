#include "ShomCanBus.h"

// =========   CONSTRUCTORS   =========

ShomCanBus::ShomCanBus()
	{
	}

ShomCanBus::ShomCanBus(char* title) : Unit(title, UT_CANBUS)
	{
	}

// ------------------------------------
void ShomCanBus::Init(byte id, byte pin_ss)
	{
	_canbus_id = id;
	_canbus_pin_ss = pin_ss;
	Init(); 
	}
		
// ------------------------------------
void ShomCanBus::Init()
	{
	if(_state != CBS_ON)
		{
		Log::BufClear();
		Log::BufCat(_title);
		Log::BufCat(" Init");
		Log::BufPrint();
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

		_ConnectionOK = canbus_ok;
		if (canbus_ok)
			{	
			_state = CBS_ON;
			Log::BufClear();
			Log::BufCat("Init OK");
			Log::BufPrint();
			}
		else
			{
			Log("Init fail");
			SetErrState(CBS_ERR402);
			}
		}
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
void ShomCanBus::LogData(char* comment)
	{
	Log::BufClear();
	Log::BufCat(_title);
	Log::BufCat(" ");
	Log::BufCat(comment);
	Log::BufCat(" Data: ");
	Log::BufCat(" id=");
	Log::BufCat(GetMsgId());
	Log::BufCat("; ");
	Log::BufCat(GetCmdTitle(_data_buffer[DATA_CMD]));
	Log::BufCat("; ");
	Log::BufCat(_data_buffer[DATA_PIN]);
	Log::BufCat("; ");
	Log::BufCat(_data_buffer[DATA_VALUE]);
	Log::BufPrint();
	}
	
// ------------------------------------
char* ShomCanBus::GetCmdTitle(CanBusCmd cmd)
	{
	if(cmd == CANBUS_READ)			return "CANBUS_READ";
	else if(cmd == CANBUS_WRITE)	return "CANBUS_WRITE";
	else if(cmd == CANBUS_MODE)		return "CANBUS_MODE";
	else if(cmd == CANBUS_RESPONSE)	return "CANBUS_RESPONSE";
	else if(cmd == CANBUS_NOPE)		return "CANBUS_NOPE";
	else if(cmd == CANBUS_RESET)	return "CANBUS_RESET";
	else							return "Unknown";
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
unsigned int ShomCanBus::SendCmd(CanBusCmd cmd, byte pin, byte value)
	{
	return SendCmd(0, cmd, pin, value);
	}

// ------------------------------------
unsigned int ShomCanBus::SendCmd(unsigned int id, CanBusCmd cmd, byte pin, byte value)
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
		//LogData("SendCmd");
		return id;
	}

// ------------------------------------
CanBusState	ShomCanBus::GetResponse(unsigned int id, byte pin)
	{
	CanBusState res = CBS_ERR; 
	int i;
	for (i=0; i < RESPONSE_TRY_CNT; i++)
		{
		byte len = Receive();
		/*
		Log::BufClear();
		Log::BufCat("i=");
		Log::BufCat(i);
		Log::BufCat(" len=");
		Log::BufCat(len);
		Log::BufPrint();
		*/
		if(len > 0)
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
							{
							Log(pin, "Wrong data", data);
							SetErrState(KS_ERR501);
							}
						}
					else
						{
						Log(pin, "Wrong cmd", cmd);
						SetErrState(KS_ERR503);
						}
	                
					if(res == CBS_HIGH || res == CBS_LOW)
						break;
					}
				else
					{
					Log::BufClear();
					Log::BufCat("id expected=");
					Log::BufCat(id);
					Log::BufCat(", recieved=");
					Log::BufCat(_id);
					Log::BufPrint();
					SetErrState(KS_ERR504);
					}
				}
			else
				{
				Log(pin, "Wrong data lenght", len);
				SetErrState(KS_ERR504);
				}
            }
		delay(RESPONSE_DELAY);
		}

	//LogData("GetResponse");

	if(_state == CBS_ERR)
		{
		Log(pin, "No data received", 0);
		SetErrState(KS_ERR505);
		_ConnectionOK = false;
		}
	else if (i > 0) 
		{
		Log::BufClear();
		Log::BufCat("not received. i=");
		Log::BufCat(i);
		Log::BufPrint();
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
	
// ------------------------------------
void ShomCanBus::ResponseDelay()
	{
	delay(RESPONSE_DELAY);
	}

// ------------------------------------
void ShomCanBus::Log(char* str)
	{
	Log::BufClear();
	Log::BufCat(_title);
	Log::BufCat(" ");
	Log::BufCat(str);
	Log::BufPrint();
	}

// ------------------------------------
void ShomCanBus::Log(byte pin, char* txt, byte data)
	{
	Log::BufClear();
	Log::BufCat(_title);
	Log::BufCat(" ");
	Log::BufCat("Pin=");
	Log::BufCat(pin);
	Log::BufCat(" ");
	Log::BufCat(txt);
	Log::BufCat(" : ");
	Log::BufCat(data);
	Log::BufPrint();
	}

// ------------------------------------
void ShomCanBus::LogInfo()
	{
	Log::BufClear();
	Log::BufCat(_title);
	Log::BufCat(" id=");
	Log::BufCat(_canbus_id);
	Log::BufCat("; pin SS=");
	Log::BufCat(_canbus_pin_ss);
	Log::BufPrint();
	}

// ------------------------------------
bool ShomCanBus::ConnectionOK()
	{
	return _state != CBS_ERR;;
	}
	