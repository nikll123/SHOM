#include "Log.h"
	
//------------------------------
static void Log::BufClear()
	{
	strcpy(_logBuffer, "");	
	}
	
//------------------------------
static void Log::BufCat(char *str)
	{
	byte lb = strlen(_logBuffer);
	byte ls = strlen(str);
	char s[LOG_BUFFER_LENGHT];
	strcpy(s, str);	
	
	if(lb + ls >= LOG_BUFFER_LENGHT)
		{
		byte i = ls - (lb + ls - (LOG_BUFFER_LENGHT - 1));	
		s[i]= '\0';
		}
	strcat(_logBuffer, s);
	delete [] s;
	}
	
//------------------------------
static void Log::BufCat(int x)
	{
	char s[5];
	itoa(x, s, 10);
	strcat(_logBuffer, s);
	delete [] s;
	}

//------------------------------
static void Log::BufCat(byte x)
	{
	char s[3];
	itoa(x, s, 10);
	strcat(_logBuffer, s);
	delete [] s;
	}

//------------------------------
static void Log::BufCat(unsigned int x)
	{
	char s[5];
	itoa(x, s, 10);
	strcat(_logBuffer, s);
	delete [] s;
	}

//------------------------------
static void Log::BufPrint()
	{
  	Serial.println(_logBuffer);
	}
