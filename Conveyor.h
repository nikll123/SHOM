#ifndef Conveyor_h
	#define Conveyor_h
	
	#include "Unit.h"
	#include "Led.h"
	#include "PinIn.h"
	#include "PinOut.h"
	#include "Contactor.h"

	enum ConveyorState{ 
						US_NOTINIT		= 0,
						US_ON			= 1,
						US_OFF			= 2,
						US_STARTING		= 3,
						US_STOPPING		= 4,
						US_UNKNOWN		= 5,
						US_ERR1000		= 1000,
						US_ERR1001		= 1001,
						US_ERR1002		= 1002,
						US_ERR1003		= 1003,
						US_ERR1004		= 1004,
						US_ERR1005		= 1005,
						US_ERR1006		= 1006,
						US_ERR1007		= 1007,
						US_ERR1008		= 1008,
						US_ERR1009		= 1009,
						US_ERR1010		= 1010,
					};
						
	struct ConveyorState2
					{
					ConveyorState Old;  
					ConveyorState New;
					
					ConveyorState2(ConveyorState csOld = US_NOTINIT, ConveyorState csNew = US_NOTINIT):Old(csOld ),New(csNew ){};  
					}; 
	
    struct ConveyorInfo	
					{
					String   		Title;
					String	 		UnitType;
					String			State;
					uint8_t			PinIn;
					uint8_t     	PinOut;
					uint8_t     	PinAuto;
					uint8_t     	PinLed;
					bool			Active;
					unsigned int 	TimeOutOn;
					unsigned int 	TimeOutOff;
					}; 

	
	class Conveyor : public Unit
	{
	public:
					    Conveyor();
					    Conveyor(String title, uint8_t pinIn, uint8_t pinOut, uint8_t pinAuto, uint8_t pinLed);
					    Conveyor(String title, uint8_t pinIn, uint8_t pinOut, uint8_t pinAuto, uint8_t pinLed, unsigned long timeOutOn, unsigned long  timeOutOff);

		ConveyorState2	GetState();
		ConveyorInfo 	GetInfo();
		String			GetConveyorStateText(ConveyorState state);
		void			Init();
		void			LogInfo();
		void			Log(String str);
		void 			LogStates(ConveyorState2 cs2);
		void 			IfChanged(ConveyorState2 cs2);
		Led				LedConveyor;
		Contactor		ContactorConveyor;
		PinIn			AtomatConveyor;

		void TurnOn();
		void TurnOff();
		void TurnOffAlarm();

	protected:
		void LogKeysState();
		
	private:
		ConveyorState2	_getStateAutomat();
		ConveyorState2	_getStateContactor();
		ConveyorState	_state;
		unsigned long 	_millsCheck;
		void 			_Turn(ConveyorState csNew);
	};
#endif

