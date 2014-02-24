/** 
 * File: InputDeviceFactory.h
 * Date: 5 Oct 2010
 * Description: Interface for a moFlo InputDeviceFactory
 */

/** 
 *
 * Author Stuart 
 * Version 1.0 - moFlo
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MOFLO_INPUT_INPUTDEVICEFACTORY_H_
#define _MOFLO_INPUT_INPUTDEVICEFACTORY_H_

namespace ChilliSource
{
	namespace Input
	{
		class IInputDeviceFactory
		{
		public:
			virtual bool CanCreateDeviceWithInterface(Core::InterfaceIDType inInterfaceID) = 0;
			
		};
	}
}

#endif

