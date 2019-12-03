#include "Pin.h"

// ------------------------------------
Pin::Pin() : Pin("DummyPin", 0, UT_PIN) 
{
}

Pin::Pin(String title, uint8_t pin, UnitType ut) : Unit(title, ut)
{
	_logLevel = LL_NORMAL;
	_pin = pin;
	_state = KS_NONE;
}

ShomCanBus Pin::CanBus = ShomCanBus(); 

// ------------------------------------
bool Pin::IsHigh()
	{
	return ShomPinRead(); 
	}

// ------------------------------------
bool Pin::IsLow()
	{
 	return !IsHigh();
	}

// ------------------------------------
PinState Pin::GetState()
{
	return _state;
}

// ------------------------------------
void Pin::SetState(PinState state, bool noLog)
	{
	PinState2 ps2;
	ps2.Old = _state;
	bool val = (state == KS_ON);
	if (_logicType == LT_INVERSE)
		 val = !val;
	ShomPinWrite(val);
	_state = state; 
	ps2.New = _state;
	if (ps2.Changed() && !noLog) 
		LogStates(ps2);
	}
// ------------------------------------
void Pin::LogStates(PinState2 ps2)
	{
	String str  = String(_pin) + "; ";
	str = str + GetPinStateText(ps2.Old) + " -> " + GetPinStateText(ps2.New); 
	Log(str);
	}

// ------------------------------------
PinInfo Pin::GetInfo()
	{
	UnitInfo ui = Unit::GetInfo();
    return {ui.Title,
			ui.UnitType,
			GetPinModeText(), 
			GetPinStateText(_state), 
			_pin}; 
	}

//------------------------------
String Pin::GetPinStateText(PinState instate)
{
switch (instate)
	{
	case KS_NONE		: return "KS_NONE";
	case KS_ON			: return "KS_ON";
	case KS_OFF			: return "KS_OFF";
	default			    : return "KS_unknown-" + String(instate);
	}
}

//------------------------------
bool Pin::ShomPinRead()
	{
	bool res = false;
	_state = KS_NONE;
	byte pin = _pin; 
	if (pin < 100)
		res = digitalRead(pin);
	else
		{
		pin = _pin - 100; 
		Pin::CanBus.ResetData();
		unsigned int id = Pin::CanBus.GetMsgId();
		byte idh = highByte(id);
		byte idl = lowByte(id);
		Pin::CanBus.SetDataByte(DATA_ID_HIGH, idh);
		Pin::CanBus.SetDataByte(DATA_ID_LOW, idl);
		Pin::CanBus.SetDataByte(DATA_CMD, CANBUS_READ);
		Pin::CanBus.SetDataByte(DATA_PIN, pin);
		Pin::CanBus.Send();
		bool received = false;
		byte len = 0;
		byte tryId = 0;
		for (int i=0; i < RESPONSE_TRY_CNT; i++)
			{
			delay(RESPONSE_DELAY);
			len = Pin::CanBus.Receive();
			//Log("i=" + String(i) + " len=" + String(len));
			//Pin::CanBus.LogData();
			received = (len > 0);
			if(received)
				{
				if (len == DATA_LENGHT)
					{
					byte idh1 = Pin::CanBus.GetDataByte(DATA_ID_HIGH);
					byte idl1 = Pin::CanBus.GetDataByte(DATA_ID_LOW);
					if (idh == idh1 && idl == idl1)
					    {
						CanBusCmd cmd = Pin::CanBus.GetDataByte(DATA_CMD);
						if(CANBUS_RESPONSE == cmd)
							{
							byte pin_resp = Pin::CanBus.GetDataByte(DATA_PIN);
							if(pin == pin_resp)
								{
								byte data = Pin::CanBus.GetDataByte(DATA_VALUE);
								if(data == 0 || data == 1)
									res = data;
								else 
									{
									Log("wrong data " + String(data));
									SetErrState(KS_ERR501);	// wrong data
									}
								}
							else
								{
								Log("wrong pin " + String(pin)); 
								SetErrState(KS_ERR502);	// wrong pin
								}
							}
						else
							{ 
							Log("wrong cmd " + String(cmd));
							SetErrState(KS_ERR503);		// wrong cmd (<> CANBUS_RESPONSE)
							} 
		
		                received = (_state == KS_NONE);
		                
						if(received)
							break;
						}
					else
						{
						Log("idh =" + String(idh) +  " idl= " + String(idl));
						Log("idh1=" + String(idh1) + " idl1=" + String(idl1));
						SetErrState(KS_ERR506);		// wrong msgId
						} 
					}
				else
					{
					Log("wrong data lenght =" + String(len));
					SetErrState(KS_ERR504);			// wrong data lenght
					}
	            }
			tryId++;
			}
		if(!received)
			{
			Log("No data received");
			SetErrState(KS_ERR505);
			}
		else if (tryId > 0) 
			{
			Log("tryId = " + String(tryId));
			}
		}
	if (_state == KS_NONE)
		{
		if(res)
			_state = KS_ON;
		else
			_state = KS_OFF;
		}

	return res;
	}

//------------------------------
void Pin::ShomPinWrite(bool val)
	{
	if(val == true)
		_state = KS_ON;
	else
		_state = KS_OFF;
		
	if (_pin < 100)
		{
		digitalWrite(_pin, val);
		}
	else
		{
		unsigned int id = Pin::CanBus.GetMsgId();
		byte idh = highByte(id);
		byte idl = lowByte(id);
		Pin::CanBus.ResetData();
		Pin::CanBus.SetDataByte(DATA_ID_HIGH, idh);
		Pin::CanBus.SetDataByte(DATA_ID_LOW, idl);
		Pin::CanBus.SetDataByte(DATA_CMD, CANBUS_WRITE);
		Pin::CanBus.SetDataByte(DATA_PIN, _pin - 100);
		Pin::CanBus.SetDataByte(DATA_VALUE, val);
		//Pin::CanBus.LogData();
		Pin::CanBus.Send();
		}
	}

//------------------------------
void Pin::ShomPinMode(byte pinmode)
	{
	Log("ShomPinMode " + GetPinModeText());
	_pinmode = pinmode;
	if (_pin < 100)
		{
		pinMode(_pin, _pinmode);
		}
	else
		{
		unsigned int id = Pin::CanBus.GetMsgId();
		byte idh = highByte(id);
		byte idl = lowByte(id);
		Pin::CanBus.ResetData();
		Pin::CanBus.SetDataByte(DATA_ID_HIGH, idh);
		Pin::CanBus.SetDataByte(DATA_ID_LOW, idl);
		Pin::CanBus.SetDataByte(DATA_CMD, CANBUS_MODE);
		Pin::CanBus.SetDataByte(DATA_PIN, _pin - 100);
		Pin::CanBus.SetDataByte(DATA_VALUE, _pinmode);
		Pin::CanBus.Send();
		}
	}

//------------------------------
void Pin::LogInfo()
	{
	PinInfo	pi = GetInfo();
	String str = pi.UnitType + ", " + pi.PinMode + ", " + pi.State + ", " + String(pi.Pin);
	Log(str);   
  	}

//------------------------------
String Pin::GetPinModeText()
	{
	if(_pinmode == INPUT_PULLUP)
		return "INPUT_PULLUP";
	else if(_pinmode == INPUT)
		return "INPUT";
	else if(_pinmode == OUTPUT)
		return "OUTPUT";
	else
		return "Unknown";
	}

// ------------------------------------
void Pin::SetErrState(UnitError err)
	{
	LogErr(err);
	_state = KS_ERR;
	}

//------------------------------
String Pin::GetLogicTypeText()
{
switch (_logicType)
	{
	case LT_NONE 		: return "NONE";
	case LT_NORMAL	 	: return "NORMAL";
	case LT_INVERSE 	: return "INVERSE";
	default			    : return "GetLogicTypeText: unknown-" + String(_logicType);
	}
}

