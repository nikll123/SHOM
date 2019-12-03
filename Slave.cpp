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
	unsigned int id = Slave::CanBus.GetMsgId();
	CanBusCmd cmd = Slave::CanBus.GetDataByte(DATA_CMD);
	byte pin = Slave::CanBus.GetDataByte(DATA_PIN);
	byte data = Slave::CanBus.GetDataByte(DATA_VALUE);
	if (cmd == CANBUS_READ || cmd == CANBUS_WRITE || cmd == CANBUS_MODE || cmd == CANBUS_RESET || cmd == CANBUS_NOPE)
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
				id = Slave::CanBus.SendCmd(id, CANBUS_RESPONSE, pin, res);
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
				id = Slave::CanBus.SendCmd(id, CANBUS_RESPONSE, 0, 0); // send zeros
				}
			else if (cmd == CANBUS_NOPE)
				{
				id = Slave::CanBus.SendCmd(id, CANBUS_NOPE, 0, 0); // send zeros
				}
			}
		else 
			res = -3;
		}
	else 
		res = -2;
	
	return res;	
	}
