#include "Slave.h"


// ------------------------------------
Slave::Slave()
	{
	//Slave::CanBus.Begin();
	}

ShomCanBus Slave::CanBus = ShomCanBus(); 

// ------------------------------------
/*void Slave::Init()
	{
	}*/


// ------------------------------------
byte Slave::GetData()
	{
	//Pin::CanBus.ResetData();
	byte len = Slave::CanBus.Receive();
	}
	
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
			if(data == 0 || data == 1)
				{
				res = 0;
				if(cmd == CANBUS_READ)
					{
					res = digitalRead(pin);
					Slave::CanBus.ResetData();
					Slave::CanBus.SetDataByte(0, CANBUS_RESPONSE);
					Slave::CanBus.SetDataByte(1, pin);
					Slave::CanBus.SetDataByte(2, res);
					Slave::CanBus.Send();
					}
				else if (cmd == CANBUS_WRITE)
					{
	//String str ="CANBUS_WRITE " + String(pin) + " " + String(data); 
	//Slave::CanBus.Log(str);
					digitalWrite(pin, data);
					}
				else if (cmd == CANBUS_MODE)
					{
	//String str ="CANBUS_MODE " + String(pin) + " " + String(data); 
	//Slave::CanBus.Log(str);
					pinMode(pin, data);
					}
				}
			else 
				res = -4;
			}
		else 
			res = -3;
		}
	else 
		res = -2;
	
	return res;	
	}

// ------------------------------------
void Slave::Do()
	{
	if (0 < GetData())
		{
		 Slave::CanBus.LogData();
		}
	}
