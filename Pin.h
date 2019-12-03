#ifndef Pin_h
	#define Pin_h
	
	#include "Unit.h"
	#include "ShomCanBus.h"
		
   	//-------------------------------
	enum PinState {KS_NONE	= 1, 
					KS_ON	= 2, 
					KS_OFF	= 3,
					KS_ERR	= 500,
					};	

	//-------------------------------
	struct PinState2 {
					PinState 	Old;
					PinState 	New;
					bool Front()	{return Old == KS_OFF && New == KS_ON;};
					bool Back()		{return Old == KS_ON && New == KS_OFF;};
					bool Changed()	{return Old != New;};
					};	

   	//-------------------------------
	struct PinInfo {
					String 		Title;
					String 		UnitType;
					String 		PinMode;
					String 		State;
					uint8_t		Pin;
					};	

	//-------------------------------
	struct PinRespCode{
					int	RespCode = 0;
					bool Value = false;
					};	

	//-------------------------------
	enum LogicType {
					LT_NONE,
					LT_NORMAL, 
					LT_INVERSE
					};	

   	//-------------------------------
	class Pin : public Unit
	{
	public:
	    					Pin();
	    					Pin(String title, uint8_t pin, UnitType ut);
	    
		PinInfo 			GetInfo();
		PinState			GetState();
		void				SetState(PinState state, bool noLog);	
		void				LogInfo();	
		void 				LogStates(PinState2 ps2);

		String 				GetLogicTypeText();
		
		String 				GetPinStateText(PinState);
		String 				GetPinModeText();
		bool				IsHigh();
		bool				IsLow();
		bool				ShomPinRead();
		void 				ShomPinWrite(bool val);
		void 				ShomPinMode(uint8_t pinmode);
		static ShomCanBus 	CanBus;

	protected:
		uint8_t 			_pin;
		PinState			_state = KS_NONE;
		uint8_t 			_pinmode;
		LogicType			_logicType; 

	private:
	};
#endif

