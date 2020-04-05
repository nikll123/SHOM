#include "ShomCanBus.h"

// =========   CONSTRUCTORS   =========

ShomCanBus::ShomCanBus()
{
}

ShomCanBus::ShomCanBus(const char *title, byte canbus_id, byte pin_ss) : Unit(title, UT_CANBUS)
{
	Setup(title, canbus_id, pin_ss);
}

// ------------------------------------
void ShomCanBus::LogInfo()
{
	char buf[STRMAXLEN] = "";
	strcatShom(buf, (char *)_title);
	Log_(buf);
	Log_(" id=");
	LogInt_(_canbus_id);
	Log_("; pin SS=");
	LogInt(_canbus_pin_ss);
}

void ShomCanBus::Setup(const char *title, byte canbus_id, byte pin_ss)
{
	_canbus_pin_ss = pin_ss;
	_canbus_id = canbus_id;
	_title = title;
}

// ------------------------------------
void ShomCanBus::Init()
{
	if (_state != CBS_ON)
	{
		Log_(_title);
		Log(" Init");
		canbus = MCP_CAN(_canbus_pin_ss);
		bool canbus_ok = false;
		canbus_ok = (CAN_OK == canbus.begin(CANBUS_RATE));
		for (int i = 0; i < CREATE_TRY_MAX; i++)
		{
			canbus_ok = (CAN_OK == canbus.begin(CANBUS_RATE));
			if (canbus_ok)
				break;
			else
			{
				SetErrState(CBS_ERR401);
			}

			delay(100);
		}

		if (canbus_ok)
		{
			_state = CBS_ON;
			Log("Init OK");
		}
		else
		{
			SetErrState(CBS_ERR402, "Init fail");
		}
	}
}

// ------------------------------------
void ShomCanBus::SetErrState(int i, UnitError err, byte pin, const char *txt, byte data)
{
	Log_(_title);
	Log_(" Error: ");
	if (i > -1)
	{
		Log_("i=");
		LogInt_(i);
		Log_("; ");
	}
	Log_("Pin=");
	LogInt_(pin);
	Log_("; ");
	Log_(txt);
	Log_(": ");
	LogInt(data);
	SetErrState(err);
}

// ------------------------------------
void ShomCanBus::SetErrState(UnitError err, const char *txt)
{
	Log(txt);
	SetErrState(err);
}

// ------------------------------------
void ShomCanBus::SetErrState(UnitError err)
{
	_state = CBS_ERR;
	LogErr(err);
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
void ShomCanBus::SetDataByte(byte i, byte data) // index, data
{
	_data_buffer[i] = data;
}

// ------------------------------------
byte ShomCanBus::GetDataByte(byte i) // index
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
void ShomCanBus::LogData(const char *comment)
{
	unsigned int _id = GetMsgId();
	Log_(comment);
	Log_(" id=");
	LogInt_(_id);
	Log_("; cmd=");
	Log_(GetCmdTitle((CanBusCmd)_data_buffer[DATA_CMD]));
	Log_("; pin=");
	LogInt_(100 + _data_buffer[DATA_PIN]);
	Log_("; value=");
	LogInt(_data_buffer[DATA_VALUE]);
}

// ------------------------------------
const char *ShomCanBus::GetCmdTitle(CanBusCmd cmd)
{
	if (cmd == CANBUS_READ)
		return "CANBUS_READ";
	else if (cmd == CANBUS_WRITE)
		return "CANBUS_WRITE";
	else if (cmd == CANBUS_MODE)
		return "CANBUS_MODE";
	else if (cmd == CANBUS_RESPONSE)
		return "CANBUS_RESPONSE";
	else if (cmd == CANBUS_NOPE)
		return "CANBUS_NOPE";
	else if (cmd == CANBUS_RESET)
		return "CANBUS_RESET";
	else
		return "Unknown";
}

// ------------------------------------
unsigned int ShomCanBus::NewMsgId()
{
	_msgId++;
	if (_msgId == 0)
		_msgId = 1;
	return _msgId;
}

//------------------------------------
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
	//LogData();
	return id;
}

// ------------------------------------
CanBusState ShomCanBus::GetResponse(unsigned int id, byte pin)
{
	CanBusState res = CBS_UNKNOWN;
	int i;
	pin = pin + 100;
	for (i = 0; i < RESPONSE_TRY_CNT; i++)
	{
		byte len = Receive();
		/*Log_("i=");
		LogInt_(i);
		Log_(" len=");
		LogInt_(len);
		Log_(" :");
		LogData("GetResponse");*/
		if (len == DATA_LENGHT)
		{
			unsigned int _id = GetMsgId();
			if (_id == id)
			{
				CanBusCmd cmd = (CanBusCmd)GetDataByte(DATA_CMD);
				if (CANBUS_RESPONSE == cmd)
				{
					byte data = GetDataByte(DATA_VALUE);
					if (data == 0)
						res = CBS_LOW;
					else if (data == 1)
						res = CBS_HIGH;
					else
					{
						SetErrState(i, KS_ERR501, pin, "Wrong data", data);
					}
				}
				else
				{
					SetErrState(i, KS_ERR503, pin, "Wrong command", cmd);
				}
				if (res == CBS_HIGH || res == CBS_LOW)
					break;
			}
			else
			{
				String s = "ids: expected=" + String(id) + " received=" + String(_id);
				SetErrState(i, KS_ERR506, pin, s.c_str(), 0);
			}
		}
		else
		{
			SetErrState(i, KS_ERR504, pin, "Wrong data lenght", len);
		}
		delay(RESPONSE_DELAY);
	}

	if (res == CBS_UNKNOWN)
	{
		SetErrState(i, KS_ERR505, pin, "No data received", 0);
	}
	else if ((res != CBS_ERR) && (i > 0)) // if response is gotten not with one try
	{
		Log_(_title);
		Log_(".GetResponse count i=");
		LogInt(i);
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
bool ShomCanBus::ConnectionOK()
{
	return _state != CBS_ERR;
	;
}
