#ifndef ConveyorHandler_h
	#define ConveyorHandler_h
	#include "Unit.h"
	#include "Conveyor.h"

	class ConveyorHandler : public Unit
	{
	public:
					    ConveyorHandler();
					    ConveyorHandler(String title, uint8_t id);
					    ConveyorHandler(String title, uint8_t id, unsigned long timeOutOn, unsigned long timeOutOff);

/*		ConveyorState2	GetState();
		ConveyorState2	GetState(String logText);
		ConveyorInfo 	GetInfo();
		static String	GetConveyorStateText(ConveyorState state);
		void			Init();
		String			GetInfoTxt();
		void 			SetErrState(ConveyorState err);
		void 			LogStates(ConveyorState2 cs2);
		void 			LogStatesPrevCurr(ConveyorStatePrevCurr cs2);
		void 			IfChanged(ConveyorState2 cs2);
		Led				LedConveyor;
		Contactor		ContactorConveyor;
		PinIn			AtomatConveyor;

		void 			TurnOn();
		void 			TurnOff();
		void 			Halt();
        bool			IsActive();  */

	protected:
		void LogKeysState();
		
	private:
		ConveyorState2	_getStateAutomat();
		ConveyorState2	_getStateContactor();
		ConveyorState	_state;
		void 			_Turn(ConveyorState csNew);
	};

#endif

