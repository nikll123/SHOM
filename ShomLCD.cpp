#include "ShomLCD.h"

LiquidCrystal_I2C ShomLCD::lcd = LiquidCrystal_I2C(ADDR, EN, RW, RS, D4, D5, D6, D7, BL, BLPOL);

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
    lcd.begin(MAXCOL + 1, MAXROW + 1);
    //
}

// ------------------------------------
void ShomLCD::BackLightOn()
{
    lcd.backlight();
}

// ------------------------------------
void ShomLCD::BackLightOff()
{
    lcd.noBacklight();
}

// ------------------------------------
void ShomLCD::SetCursor(byte row, byte col)
{
    if (row > MAXROW || col > MAXCOL)
        this->LogErrPosition(row, col);
    else
        lcd.setCursor(col, row);
}

// ------------------------------------
void ShomLCD::Home()
{
    lcd.setCursor(0, 0);
}

// ------------------------------------
void ShomLCD::Print(const char *str)
{
    lcd.print(str);
}

void ShomLCD::Print(byte row, byte col, const char *str)
{
    this->SetCursor(row, col);
    this->Print(str);
}

// ------------------------------------
void ShomLCD::Clear()
{
    lcd.clear();
}

// ------------------------------------
void ShomLCD::LogErrPosition(byte row, byte col)
{
    LogIndent_();
    Log_(_title);
    Log_(" Error: Wrong position: row=");
    LogInt_(row);
    Log_("; col=");
    LogInt(col);
}

// ------------------------------------
void ShomLCD::LogInfo()
{
    Log_(_title);
    Log_("; ADDR=");
    LogInt_(ADDR);
    Log_("; EN=");
    LogInt_(EN);
    Log_("; RW=");
    LogInt_(RW);
    Log_("; RS=");
    LogInt_(RS);
    Log_("; D4=");
    LogInt_(D4);
    Log_("; D5=");
    LogInt_(D5);
    Log_("; D6=");
    LogInt_(D6);
    Log_("; D7=");
    LogInt_(D7);
    Log_("; BL=");
    LogInt_(BL);
    Log_("; BLPOL=");
    LogInt(BLPOL);
}