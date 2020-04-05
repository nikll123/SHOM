#include "Log.h"

//------------------------------
void Log(char *s1)
{
	_log(s1, true);
}

void Log(const char *s1)
{
	_log((char *)s1, true);
}

//------------------------------
void Log_(char *s1)
{
	_log(s1, false);
}

void Log_(const char *s1)
{
	_log((char *)s1, false);
}

//------------------------------
void _log(char *s1, bool ln)
{
	Serial.print(s1);
	if (ln)
		Serial.println("");
}

//------------------------------
const char *errPrefix(UnitError err, char *buf)
{
	if (100 < err && err < 200)
		strcpy(buf, "CS");
	else if (200 < err && err < 300)
		strcpy(buf, "US");
	else if (300 < err && err < 400)
		strcpy(buf, "SS");
	else if (400 < err && err < 500)
		strcpy(buf, "CBS");
	else if (500 < err && err < 600)
		strcpy(buf, "KS");
	else if (600 < err && err < 700)
		strcpy(buf, "SL");
	else
		return "Unknown";
	return buf;
}

//------------------------------
void _logInt(unsigned int x, bool ln)
{
	char p[6] = "";
	itoa(x, p, 10);
	_log(p, ln);
}

//------------------------------
void LogInt(unsigned int x)
{
	_logInt(x, true);
}

//------------------------------
void LogInt_(unsigned int x)
{
	_logInt(x, false);
}

//------------------------------
void LogIndent_()
{
	_log((char *)"    ", false);
}


//------------------------------
char *strcatShom(char *dest, const char *txt)
{
	strcatShom(dest, (char *)txt);
}

//------------------------------
char *strcatShom(char *dest, char *txt)
{
	int len = STRMAXLEN - 1 - strlen(dest);
	strncat(dest, txt, len);
	if (strlen(txt) > len)
	{
		dest[STRMAXLEN] = '\0';
		dest[STRMAXLEN - 1] = '.';
		dest[STRMAXLEN - 2] = '.';
		dest[STRMAXLEN - 3] = '.';
	}
	return dest;
}