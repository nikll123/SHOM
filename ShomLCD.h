#ifndef ShomLCD_h
#define ShomLCD_h

#include "Unit.h"

#include <LiquidCrystal_I2C.h>

//-------------------------------
class ShomLCD : public Unit
{
public:
	ShomLCD();
	ShomLCD(const char *title, byte pin_sda, byte pin_slc);
	void Init();
	void Setup(const char *title, byte pin_sda, byte pin_slc);
	static LiquidCrystal_I2C lcd;

private:
	byte _pin_sda;
	byte _pin_slc;

};

#endif
