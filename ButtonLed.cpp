#include "ButtonLed.h"
// ------------------------------------
ButtonLed::ButtonLed() : ButtonLed("ButtonLed", 0, 0) 
{
}

ButtonLed::ButtonLed(String title, uint8_t pinBtn, uint8_t pinLed) : Unit(title, UT_PININLED) 
{
	Button = PinIn(title + "_but", pinBtn);
	LedIndicator = Led(title + "_led", pinLed);
}


// ------------------------------------
PinState2 ButtonLed::GetState()
	{
	PinState2 ps2 = Button.GetState();

	if(LedIndicator.Pin > 0)
		{
		if (ps2.New == KS_ON)
			LedIndicator.SetOn();
		else
			LedIndicator.SetBlink();
		LedState2 ls2 = LedIndicator.CheckState();
		}
		
	if (LOGLEVEL > LL_MIN && ps2.Old != ps2.New)
		{
		LogText(_title);
		LogText("  " + Button.GetPinStateText(ps2.Old));
		LogTextLn(" -> " + Button.GetPinStateText(ps2.New));
		}
	return ps2;
	}

ButtonLedInfo ButtonLed::GetInfo()
	{
	PinInInfo bi = Button.GetInfo();
	LedInfo li = LedIndicator.GetInfo();
	return{ _title,
			GetUnitTypeText(),
			bi.State,
			bi.Pin,
			li.Pin,
			bi.Logic
			};
	}
	
void ButtonLed::LogInfo()
	{
	ButtonLedInfo bli = GetInfo();
	LogText(bli.Title + "; ");
	LogText(bli.UnitType + "; ");
	LogText(String(bli.PinBut) + "; ");
	LogText(String(bli.PinLed) + "; ");
	LogText(bli.Logic + "; ");
	LogTextLn(bli.State + ".");
	}
	
void ButtonLed::LogicInverse()
	{
	Button.LogicInverse();
	}

void ButtonLed::LogicNormal()
	{
	Button.LogicNormal();
	}

