//
//  OrbiterComponent.cpp
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

#include <Common/Behaviour/OrbiterComponent.h>

#include <Common/Behaviour/FollowerComponent.h>

namespace CSTest
{
    namespace Common
    {
        CS_DEFINE_NAMEDTYPE(OrbiterComponent);
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        OrbiterComponent::OrbiterComponent(f32 in_angularVelocity)
        : m_angularVelocity(in_angularVelocity)
        {
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool OrbiterComponent::IsA(CSCore::InterfaceIDType in_interfaceId) const
        {
            return (OrbiterComponent::InterfaceID == in_interfaceId);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        f32 OrbiterComponent::GetAngularVelocity() const
        {
            return m_angularVelocity;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void OrbiterComponent::SetAngularVelocity(f32 in_angularVelocity)
        {
            m_angularVelocity = in_angularVelocity;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void OrbiterComponent::OnAddedToScene()
        {
            auto followerComponent = GetEntity()->GetComponent<FollowerComponent>();
            CS_ASSERT(followerComponent, "OrbiterComponent requires there to be a FollowerComponent on the entity.");
            
            m_followerComponent = FollowerComponentWPtr(followerComponent);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void OrbiterComponent::OnUpdate(f32 in_deltaTime)
        {
            auto followerComponent = m_followerComponent.lock();
            CS_ASSERT(followerComponent, "OrbiterComponent requires there to be a FollowerComponent on the entity.");
            
            followerComponent->SetHorizontalAngle(followerComponent->GetHorizontalAngle() + m_angularVelocity * in_deltaTime);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void OrbiterComponent::OnRemovedFromScene()
        {
            m_followerComponent.reset();
        }
    }
}