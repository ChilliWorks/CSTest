/*
 *  AmbientLightComponent.cpp
 *  moFlo
 *
 *  Created by Scott Downie on 27/1/2014.
 *  Copyright 2014 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/Lighting/AmbientLightComponent.h>

namespace ChilliSource
{
	namespace Rendering
	{
		DEFINE_NAMED_INTERFACE(AmbientLightComponent);
        
		//----------------------------------------------------------
		/// Is A
		//----------------------------------------------------------
		bool AmbientLightComponent::IsA(ChilliSource::Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == LightComponent::InterfaceID || inInterfaceID == AmbientLightComponent::InterfaceID;
		}
        //----------------------------------------------------------
        /// Get Light Matrix
        //----------------------------------------------------------
        const Core::CMatrix4x4& AmbientLightComponent::GetLightMatrix() const
        {
            return mmatLight;
        }
	}
}

