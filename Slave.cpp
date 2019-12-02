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
	CanBusCmd cmd = Slave::CanBus.GetDataByte(0);
	byte pin = Slave::CanBus.GetDataByte(1);
	byte data = Slave::CanBus.GetDataByte(2);
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
				Slave::CanBus.SetDataByte(0, CANBUS_RESPONSE);
				Slave::CanBus.SetDataByte(1, pin);
				Slave::CanBus.SetDataByte(2, res);
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
				Slave::CanBus.SetDataByte(0, CANBUS_RESPONSE);
				Slave::CanBus.Send();		// send zeros
				}
			else if (cmd == CANBUS_NOPE)
				{
				Slave::CanBus.ResetData();
				Slave::CanBus.SetDataByte(0, CANBUS_RESPONSE);
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
