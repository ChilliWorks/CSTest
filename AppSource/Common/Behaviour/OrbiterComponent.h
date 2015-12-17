//
//  OrbiterComponent.h
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

#ifndef _COMMON_RENDERING_ORBITERCOMPONENT_H_
#define _COMMON_RENDERING_ORBITERCOMPONENT_H_

#include <CSTest.h>

#include <ChilliSource/Core/Entity.h>

namespace CSTest
{
    namespace Common
    {
        //------------------------------------------------------------------------------
        /// A component used in conjunction with the FollowerComponent to provide an
        /// object that will spin arround it's target.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        class OrbiterComponent final : public CSCore::Component
        {
        public:
            CS_DECLARE_NAMEDTYPE(OrbiterComponent);
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param in_angularVelocity - The angular velocity.
            //------------------------------------------------------------------------------
            OrbiterComponent(f32 in_angularVelocity);
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
            /// @param The angular velocity.
            //------------------------------------------------------------------------------
            f32 GetAngularVelocity() const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param in_angularVelocity - The angular velocity.
            //------------------------------------------------------------------------------
            void SetAngularVelocity(f32 in_angularVelocity);
            
        private:
            //------------------------------------------------------------------------------
            /// Called whenever this is added to the scene.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnAddedToScene() override;
            //------------------------------------------------------------------------------
            /// Called every frame the component is in the scene. Updates the camera
            /// angle.
            ///
            /// @author Ian Copland
            ///
            /// @param in_deltaTime - The delta time.
            //------------------------------------------------------------------------------
            void OnUpdate(f32 in_deltaTime) override;
            //------------------------------------------------------------------------------
            /// Called whenever this is removed from the scene.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnRemovedFromScene() override;
            
            FollowerComponentWPtr m_followerComponent;
            f32 m_angularVelocity = 0.0f;
        };
    }
}

#endif