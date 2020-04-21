#ifndef ShomLCD_h
#define ShomLCD_h

#include "Unit.h"

#include "LiquidCrystal_I2C.h"

// Set the pins on the I2C chip used for LCD connections:
#define ADDR 0x27 // 0x3f
#define EN 2
#define RW 1
#define RS 0
#define D4 4
#define D5 5
#define D6 6
#define D7 7
#define BL 3
#define BLPOL POSITIVE
#define MAXROW 1
#define MAXCOL 15

//-------------------------------
class ShomLCD : public Unit
{
public:
	ShomLCD();
	ShomLCD(const char *title, byte pin_sda, byte pin_scl);
	void Init();
	void Setup(const char *title, byte pin_sda, byte pin_scl);
	void LogInfo();
	void BackLightOn();
	void BackLightOff();
	void SetCursor(byte row, byte col);
	void Home();
	void Print(const char *str);
	void Print(byte row, byte col, const char *str);
	void Clear();
	void LogErrPosition(byte row, byte col);

	static LiquidCrystal_I2C lcd;

private:
	byte _pin_sda;
	byte _pin_scl;
};

#endif
