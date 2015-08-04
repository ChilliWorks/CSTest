//
//  CameraSpinnerComponent.cpp
//  CSTest
//  Created by Ian Copland on 04/08/2015.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2015 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#include <Common/Rendering/CameraSpinnerComponent.h>

#include <Common/Rendering/ThirdPersonCameraComponent.h>

namespace CSTest
{
    namespace Common
    {
        CS_DEFINE_NAMEDTYPE(CameraSpinnerComponent);
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        CameraSpinnerComponent::CameraSpinnerComponent(f32 in_angularVelocity)
        : m_angularVelocity(in_angularVelocity)
        {
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool CameraSpinnerComponent::IsA(CSCore::InterfaceIDType in_interfaceId) const
        {
            return (CameraSpinnerComponent::InterfaceID == in_interfaceId);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        f32 CameraSpinnerComponent::GetAngularVelocity() const
        {
            return m_angularVelocity;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void CameraSpinnerComponent::SetAngularVelocity(f32 in_angularVelocity)
        {
            m_angularVelocity = in_angularVelocity;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void CameraSpinnerComponent::OnAddedToScene()
        {
            auto thirdPersonCameraComponent = GetEntity()->GetComponent<ThirdPersonCameraComponent>();
            CS_ASSERT(thirdPersonCameraComponent, "CameraSpinnerComponent requires there to be a ThirdPersonCameraComponent on the entity.");
            
            m_thirdPersonCameraComponent = ThirdPersonCameraComponentWPtr(thirdPersonCameraComponent);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void CameraSpinnerComponent::OnUpdate(f32 in_deltaTime)
        {
            auto thirdPersonCameraComponent = m_thirdPersonCameraComponent.lock();
            CS_ASSERT(thirdPersonCameraComponent, "CameraSpinnerComponent requires there to be a ThirdPersonCameraComponent on the entity.");
            
            thirdPersonCameraComponent->SetHorizontalAngle(thirdPersonCameraComponent->GetHorizontalAngle() + m_angularVelocity * in_deltaTime);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void CameraSpinnerComponent::OnRemovedFromScene()
        {
            m_thirdPersonCameraComponent.reset();
        }
    }
}