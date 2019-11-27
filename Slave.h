#ifndef Slave_h
	#define Slave_h

	#include "ShomCanBus.h"


	//-------------------------------
	class Slave
	{
	public:
							Slave();
		static ShomCanBus 	CanBus;
		static int 		DoCmd();

		byte				GetData();
		void 				Do();
		//String				GetUnitTypeText();
		//String 				GetLogicTypeText();

	protected:

/*		String			_title;
		UnitType		_type;
		byte			_logLevel;
*/
	private: 
	};


#endif