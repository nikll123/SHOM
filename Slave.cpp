#include "Slave.h"


// ------------------------------------
Slave::Slave()
	{
	//Slave::CanBus.Begin();
	}

ShomCanBus Slave::CanBus = ShomCanBus(); 

	
// ------------------------------------
int Slave::DoCmd()
	{
	int res = -1;
	byte idh = Slave::CanBus.GetDataByte(DATA_ID_HIGH);
	byte idl = Slave::CanBus.GetDataByte(DATA_ID_LOW);
	CanBusCmd cmd = Slave::CanBus.GetDataByte(DATA_CMD);
	byte pin = Slave::CanBus.GetDataByte(DATA_PIN);
	byte data = Slave::CanBus.GetDataByte(DATA_VALUE);
	if (cmd == CANBUS_READ || cmd == CANBUS_WRITE || cmd == CANBUS_MODE)
	    {
		if( pin > 0 && pin < 54)
			{
			res = 0;
			if(cmd == CANBUS_WRITE)
				{
				if(data == 0 || data == 1)
					digitalWrite(pin, data);
				else
					res = -4;				
				}
			else if(cmd == CANBUS_READ)
				{
				res = digitalRead(pin);
				Slave::CanBus.ResetData();
				Slave::CanBus.SetDataByte(DATA_ID_HIGH, idh);
				Slave::CanBus.SetDataByte(DATA_ID_LOW, idl);
				Slave::CanBus.SetDataByte(DATA_CMD, CANBUS_RESPONSE);
				Slave::CanBus.SetDataByte(DATA_PIN, pin);
				Slave::CanBus.SetDataByte(DATA_VALUE, res);
				Slave::CanBus.Send();
				}
			else if (cmd == CANBUS_MODE)
				{
				if(data == INPUT || data == INPUT_PULLUP || data == OUTPUT)
					pinMode(pin, data);
				else
					res = -5;
				}
			else if (cmd == CANBUS_RESET)
				{
				Slave::CanBus.ResetData();
				Slave::CanBus.SetDataByte(DATA_ID_HIGH, idh);
				Slave::CanBus.SetDataByte(DATA_ID_LOW, idl);
				Slave::CanBus.SetDataByte(DATA_CMD, CANBUS_RESPONSE);
				Slave::CanBus.Send();		// send zeros
				}
			else if (cmd == CANBUS_NOPE)
				{
				Slave::CanBus.ResetData();
				Slave::CanBus.SetDataByte(DATA_CMD, CANBUS_NOPE);
				Slave::CanBus.Send();		// send zeros
				}
			}
		else 
			res = -3;
		}
	else 
		res = -2;
	
	return res;	
	}
