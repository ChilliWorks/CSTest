//
//  ThirdPersonCameraComponent.cpp
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

#include <Common/Rendering/ThirdPersonCameraComponent.h>

namespace CSTest
{
    namespace Common
    {
        CS_DEFINE_NAMEDTYPE(ThirdPersonCameraComponent);
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        ThirdPersonCameraComponent::ThirdPersonCameraComponent(const CSCore::EntitySPtr& in_target, const CSCore::Vector3& in_offset, f32 in_distance, f32 in_horizontalAngle, f32 in_verticalAngle)
        : m_offset(in_offset), m_distance(in_distance), m_horizontalAngle(in_horizontalAngle), m_verticalAngle(in_verticalAngle)
        {
            SetTarget(in_target);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool ThirdPersonCameraComponent::IsA(CSCore::InterfaceIDType in_interfaceId) const
        {
            return (ThirdPersonCameraComponent::InterfaceID == in_interfaceId);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        const CSCore::Vector3& ThirdPersonCameraComponent::GetOffset() const
        {
            return m_offset;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        f32 ThirdPersonCameraComponent::GetDistance() const
        {
            return m_distance;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        f32 ThirdPersonCameraComponent::GetHorizontalAngle() const
        {
            return m_horizontalAngle;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        f32 ThirdPersonCameraComponent::GetVerticalAngle() const
        {
            return m_verticalAngle;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void ThirdPersonCameraComponent::SetTarget(const CSCore::EntitySPtr& in_target)
        {
            m_target = CSCore::EntityWPtr(in_target);
            m_transformChangedConnection = in_target->GetTransform().GetTransformChangedEvent().OpenConnection([=]()
            {
                UpdatePosition();
            });
            
            UpdatePosition();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void ThirdPersonCameraComponent::SetOffset(const CSCore::Vector3& in_offset)
        {
            m_offset = in_offset;
            
            UpdatePosition();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void ThirdPersonCameraComponent::SetDistance(f32 in_distance)
        {
            m_distance = in_distance;
            
            UpdatePosition();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void ThirdPersonCameraComponent::SetHorizontalAngle(f32 in_horizontalAngle)
        {
            m_horizontalAngle = in_horizontalAngle;
            
            UpdatePosition();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void ThirdPersonCameraComponent::SetVerticalAngle(f32 in_verticalAngle)
        {
            m_verticalAngle = in_verticalAngle;
            
            UpdatePosition();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void ThirdPersonCameraComponent::UpdatePosition()
        {
            if (GetEntity() != nullptr)
            {
                if (auto target = m_target.lock())
                {
                    auto rotation = CSCore::Quaternion(CSCore::Vector3(1.0f, 0.0f, 0.0f), m_verticalAngle) * CSCore::Quaternion(CSCore::Vector3(0, 1.0f, 0.0f), m_horizontalAngle);
                    
                    CSCore::Vector3 direction(0.0f, 0.0f, 1.0f);
                    direction.Rotate(rotation);
                    
                    CSCore::Vector3 targetPosition = m_offset * target->GetTransform().GetWorldTransform();
                    CSCore::Vector3 cameraPosition = targetPosition - direction * m_distance;
                    
                    GetEntity()->GetTransform().SetLookAt(cameraPosition, targetPosition, CSCore::Vector3::k_unitPositiveY);
                }
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void ThirdPersonCameraComponent::OnAddedToEntity()
        {
            CS_ASSERT(GetEntity()->GetParent() == nullptr, "Third Person Camera Component only works for entities without parents.");
            
            UpdatePosition();
        }
    }
}