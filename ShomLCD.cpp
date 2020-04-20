#include "ShomLCD.h"

LiquidCrystal_I2C ShomLCD::lcd1 = LiquidCrystal_I2C(addr, en, rw, rs, d4, d5, d6, d7, bl, blpol);


// ------------------------------------
ShomLCD::ShomLCD()
{
}

// ------------------------------------
ShomLCD::ShomLCD(const char *title, byte pin_sda, byte pin_scl) : Unit(title, UT_LCD)
{
    Setup(title, pin_sda, pin_scl);
}


// ------------------------------------
void ShomLCD::Setup(const char *title, byte pin_sda, byte pin_scl)
{
    _pin_sda = pin_sda;
    _pin_scl = pin_scl;
    _title = title;
    // 
}

void ShomLCD::LogInfo()
{
    Log_(_title);
    Log_("; addr=");
    LogInt_(addr);
    Log_("; en=");
    LogInt_(en);
    Log_("; rw=");
    LogInt_(rw);
    Log_("; rs=");
    LogInt_(rs);
    Log_("; d4=");
    LogInt_(d4);
    Log_("; d5=");
    LogInt_(d5);
    Log_("; d6=");
    LogInt_(d6);
    Log_("; d7=");
    LogInt_(d7);
    Log_("; bl=");
    LogInt_(bl);
    Log_("; blpol=");
    LogInt(blpol);

}