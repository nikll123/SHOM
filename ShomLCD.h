#ifndef ShomLCD_h
#define ShomLCD_h

#include "Unit.h"

#include "LiquidCrystal_I2C.h"

// Set the pins on the I2C chip used for LCD connections:
#define addr 0x27 // 0x3f
#define en 2
#define rw 1
#define rs 0
#define d4 4
#define d5 5
#define d6 6
#define d7 7
#define bl 3
#define blpol POSITIVE

//-------------------------------
class ShomLCD : public Unit
{
public:
	ShomLCD();
	ShomLCD(const char *title, byte pin_sda, byte pin_scl);
	void Init();
	void Setup(const char *title, byte pin_sda, byte pin_scl);
	void LogInfo();

	static LiquidCrystal_I2C lcd1;

private:
	byte _pin_sda;
	byte _pin_scl;
};

#endif
