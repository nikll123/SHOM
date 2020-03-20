#include "Log.h"

//------------------------------
void Log(const char *s1)
{
	_log(s1, true);
}

//------------------------------
void Log(const char *s1, const char *s2)
{
	_log(s1, s2, true);
}

//------------------------------
void Log(const char *s1, const char *s2, const char *s3)
{
	_log(s1, s2, s3, true);
}

//------------------------------
void Log(const char *s1, const char *s2, const char *s3, const char *s4)
{
	_log(s1, s2, s3, s4, true);
}

//------------------------------
void Log(const char *s1, const char *s2, const char *s3, const char *s4, const char *s5)
{
	_log(s1, s2, s3, s4, s5, true);
}

//------------------------------
void Log(const char *s1, const char *s2, const char *s3, const char *s4, const char *s5, const char *s6)
{
	_log(s1, s2, s3, s4, s5, s6, true);
}

//------------------------------
void Log(const char *s1, const char *s2, const char *s3, const char *s4, const char *s5, const char *s6, const char *s7)
{
	_log(s1, s2, s3, s4, s5, s6, s7, true);
}

//------------------------------
void Log(const char *s1, const char *s2, const char *s3, const char *s4, const char *s5, const char *s6, const char *s7, const char *s8)
{
	_log(s1, s2, s3, s4, s5, s6, s7, s8, true);
}

//------------------------------
void Log(const char *s1, const char *s2, const char *s3, const char *s4, const char *s5, const char *s6, const char *s7, const char *s8, const char *s9)
{
	_log(s1, s2, s3, s4, s5, s6, s7, s8, s9, true);
}

//------------------------------
void Log(const char *s1, const char *s2, const char *s3, const char *s4, const char *s5, const char *s6, const char *s7, const char *s8, const char *s9, const char *s10)
{
	_log(s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, true);
}

//------------------------------
void Log_(const char *s1)
{
	_log(s1, false);
}

//------------------------------
void Log_(const char *s1, const char *s2)
{
	_log(s1, s2, false);
}

//------------------------------
void Log_(const char *s1, const char *s2, const char *s3)
{
	_log(s1, s2, s3, false);
}

//------------------------------
void Log_(const char *s1, const char *s2, const char *s3, const char *s4)
{
	_log(s1, s2, s3, s4, false);
}

//------------------------------
void Log_(const char *s1, const char *s2, const char *s3, const char *s4, const char *s5)
{
	_log(s1, s2, s3, s4, s5, false);
}

//------------------------------
void Log_(const char *s1, const char *s2, const char *s3, const char *s4, const char *s5, const char *s6)
{
	_log(s1, s2, s3, s4, s5, s6, false);
}

//------------------------------
void Log_(const char *s1, const char *s2, const char *s3, const char *s4, const char *s5, const char *s6, const char *s7)
{
	_log(s1, s2, s3, s4, s5, s6, s7, false);
}

//------------------------------
void Log_(const char *s1, const char *s2, const char *s3, const char *s4, const char *s5, const char *s6, const char *s7, const char *s8)
{
	_log(s1, s2, s3, s4, s5, s6, s7, s8, false);
}

//------------------------------
void Log_(const char *s1, const char *s2, const char *s3, const char *s4, const char *s5, const char *s6, const char *s7, const char *s8, const char *s9)
{
	_log(s1, s2, s3, s4, s5, s6, s7, s8, s9, false);
}

//------------------------------
void Log_(const char *s1, const char *s2, const char *s3, const char *s4, const char *s5, const char *s6, const char *s7, const char *s8, const char *s9, const char *s10)
{
	_log(s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, false);
}

//------------------------------
void _log(const char *s1, bool ln)
{
	Serial.print(s1);
	if (ln)
		Serial.println("");
}

//------------------------------
void _log(const char *s1, const char *s2, bool ln)
{
	_log(s1, false);
	_log(s2, ln);
}

//------------------------------
void _log(const char *s1, const char *s2, const char *s3, bool ln)
{
	_log(s1, s2, false);
	_log(s3, ln);
}

//------------------------------
void _log(const char *s1, const char *s2, const char *s3, const char *s4, bool ln)
{
	_log(s1, s2, false);
	_log(s3, s4, ln);
}

//------------------------------
void _log(const char *s1, const char *s2, const char *s3, const char *s4, const char *s5, bool ln)
{
	_log(s1, s2, s3, false);
	_log(s4, s5, ln);
}

//------------------------------
void _log(const char *s1, const char *s2, const char *s3, const char *s4, const char *s5, const char *s6, bool ln)
{
	_log(s1, s2, s3, false);
	_log(s4, s5, s6, ln);
}

//------------------------------
void _log(const char *s1, const char *s2, const char *s3, const char *s4, const char *s5, const char *s6, const char *s7, bool ln)
{
	_log(s1, s2, s3, s4, s5, false);
	_log(s6, s7, ln);
}

//------------------------------
void _log(const char *s1, const char *s2, const char *s3, const char *s4, const char *s5, const char *s6, const char *s7, const char *s8, bool ln)
{
	_log(s1, s2, s3, s4, s5, false);
	_log(s6, s7, s8, ln);
}

//------------------------------
void _log(const char *s1, const char *s2, const char *s3, const char *s4, const char *s5, const char *s6, const char *s7, const char *s8, const char *s9, bool ln)
{
	_log(s1, s2, s3, s4, s5, false);
	_log(s6, s7, s8, s9, ln);
}

//------------------------------
void _log(const char *s1, const char *s2, const char *s3, const char *s4, const char *s5, const char *s6, const char *s7, const char *s8, const char *s9, const char *s10, bool ln)
{
	_log(s1, s2, s3, s4, s5, false);
	_log(s6, s7, s8, s9, s10, ln);
}

/*
//------------------------------
const char *errPrefix(UnitError err)
{
	const char *res;
	if (100 < err && err < 200)
		return "CS";
	else if (200 < err && err < 300)
		return "US";
	else if (300 < err && err < 400)
		return "SS";
	else if (400 < err && err < 500)
		return "CBS";
	else if (500 < err && err < 600)
		return "KS";
	else if (600 < err && err < 700)
		return "SL";
	else
		return "Unknown";
}*/

//------------------------------
void _logInt(int x, bool ln)
{
	char p[4] = "err";

	if (0 <= x || x < 256)
		itoa(x, p, 10);

	_log(p, ln);
}

//------------------------------
void LogInt(int x)
{
	_logInt(x, true);
}

//------------------------------
void LogInt_(int x)
{
	_logInt(x, false);
}


//------------------------------
void LogIndent()
{
	_log("    ", false);
}
