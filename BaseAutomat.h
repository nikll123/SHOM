#ifndef BaseAutomat_h
	#define BaseAutomat_h
	
//	#include "Arduino.h"
	#include "Core.h"
	#include "BaseUzel.h"

	struct AutomatInfo	{
					String   	Title;
					KeyState	State;
					uint8_t		Pin;
					String		Logic;
					bool		Active;
					unsigned int TimeOutOn;
					unsigned int TimeOutOff;
					}; 
	

	class BaseAutomat : public BaseUzel
	{
	public:
		BaseAutomat(String title, uint8_t pinAutomat, LogicType lt);
		KeyState	GetState();
		String		GetStateText();
		AutomatInfo GetInfo();
         
	protected:

	private:

	};
#endif

