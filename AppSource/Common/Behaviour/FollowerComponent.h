//
//  FollowerComponent.h
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

#ifndef _COMMON_RENDERING_FOLLOWERCOMPONENT_H_
#define _COMMON_RENDERING_FOLLOWERCOMPONENT_H_

#include <CSTest.h>

#include <ChilliSource/Core/Entity.h>

namespace CSTest
{
    namespace Common
    {
        //------------------------------------------------------------------------------
        /// A component that provides the "follow" logic for a third person camera. This
        /// takes an "target" entity for the component and follows it at the given
        /// distance.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        class FollowerComponent final : public CSCore::Component
        {
        public:
            CS_DECLARE_NAMEDTYPE(FollowerComponent);
            //------------------------------------------------------------------------------
            /// Constructor. Sets the target and other base properties of the component.
            ///
            /// @author Ian Copland
            ///
            /// @param in_target - The entity to follow.
            /// @param in_offset - The offset from the target (in it's local space)
            /// that the camera will follow.
            /// @param in_distance - The distance from the target the camera will follow.
            /// @param in_horizontalAngle - The horizontal angle of the camera from the
            /// target.
            /// @param in_verticalAngle - The vertical angle of the camera from the target.
            //------------------------------------------------------------------------------
            FollowerComponent(const CSCore::EntitySPtr& in_target, const CSCore::Vector3& in_offset, f32 in_distance, f32 in_horizontalAngle, f32 in_verticalAngle);
            //------------------------------------------------------------------------------
            /// Allows querying of whether or not this system implements the interface
            /// described by the given interface Id. Typically this is not called directly
            /// as the templated equivalent IsA<Interface>() is preferred.
            ///
            /// @author Ian Copland
            ///
            /// @param in_interfaceId - The interface Id.
            ///
            /// @return Whether or not the interface is implemented.
            //------------------------------------------------------------------------------
            bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The offset from the target (in it's local space) that the camera
            /// will follow.
            //------------------------------------------------------------------------------
            const CSCore::Vector3& GetOffset() const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The distance from the target.
            //------------------------------------------------------------------------------
            f32 GetDistance() const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The horizontal angle of the camera from the target.
            //------------------------------------------------------------------------------
            f32 GetHorizontalAngle() const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The vertical angle of the camera from the target.
            //------------------------------------------------------------------------------
            f32 GetVerticalAngle() const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param in_target - The target to follow.
            //------------------------------------------------------------------------------
            void SetTarget(const CSCore::EntitySPtr& in_target);
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param in_offset - The offset from the target (in it's local space) that
            /// the camera will follow.
            //------------------------------------------------------------------------------
            void SetOffset(const CSCore::Vector3& in_offset);
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param in_distance - The distance from the target.
            //------------------------------------------------------------------------------
            void SetDistance(f32 in_distance);
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param in_horizontalAngle - The horizontal angle of the camera from the
            /// target.
            //------------------------------------------------------------------------------
            void SetHorizontalAngle(f32 in_horizontalAngle);
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param in_verticalAngle - The vertical angle of the camera from the target.
            //------------------------------------------------------------------------------
            void SetVerticalAngle(f32 in_verticalAngle);
            
        private:
            //------------------------------------------------------------------------------
            /// Sets the position of the owning entity based on the target entity.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void UpdatePosition();
            //------------------------------------------------------------------------------
            /// Called whenever this is added to an entity. This will update the position of
            /// the entity to ensure it is always in the correct place.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnAddedToEntity() override;
            
            CSCore::EntityWPtr m_target;
            CSCore::EventConnectionUPtr m_transformChangedConnection;
            CSCore::Vector3 m_offset;
            f32 m_distance = 0.0f;
            f32 m_horizontalAngle = 0.0f;
            f32 m_verticalAngle = 0.0f;
        };
    }
}

#endif