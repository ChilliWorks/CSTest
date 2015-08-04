//
//  ModelFactory.h
//  CSTest
//  Created by Ian Copland on 03/08/2015.
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

#ifndef _COMMON_RENDERING_MODELFACTORY_H_
#define _COMMON_RENDERING_MODELFACTORY_H_

#include <CSTest.h>

#include <ChilliSource/Core/Math.h>
#include <ChilliSource/Core/System.h>

namespace CSTest
{
    namespace Common
    {
        //------------------------------------------------------------------------------
        /// A factory class for creating new models types. Prior to creating the
        /// requested model this will check if an identical model already exists and
        /// return it if it does.
        ///
        /// This is not thread-safe and must only be called from the main thread.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        class ModelFactory final : public CSCore::AppSystem
        {
        public:
            CS_DECLARE_NAMEDTYPE(ModelFactory);
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
            /// Creates a new plane model perpendicular to the Y axis of the given size. If
            /// an identical plane already exists it will be returned instead of creating
            /// a new instance.
            ///
            /// @author Ian Copland
            ///
            /// @param in_size - The size of the plane.
            ///
            /// @return The plane model.
            //------------------------------------------------------------------------------
            CSRendering::MeshCSPtr CreatePlane(const CSCore::Vector2& in_size) const;
            //------------------------------------------------------------------------------
            /// Creates a box model of the given size. If an identical box already exists
            /// it will be returned instead of creating a new instance.
            ///
            /// @author Ian Copland
            ///
            /// @param in_size - The size of the plane.
            /// @param in_textureRepeat - [Optional] The number of times a texture is
            /// repeated on each face of the model in each axis. Defaults to [1, 1]
            /// @param in_flipNormals -[Optional] Whether or not to flip the box inside out.
            /// This is useful for creating rooms. Defaults to false.
            ///
            /// @return The plane model.
            //------------------------------------------------------------------------------
            CSRendering::MeshCSPtr CreateBox(const CSCore::Vector3& in_size, const CSCore::Vector2& in_textureRepeat = CSCore::Vector2::k_one, bool in_flipNormals = false) const;
            
        private:
            friend class CSCore::Application;
            //------------------------------------------------------------------------------
            /// A factory method for creating new instances of the system.
            ///
            /// @author Ian Copland
            ///
            /// @return The new instance.
            //------------------------------------------------------------------------------
            static ModelFactoryUPtr Create();
            //------------------------------------------------------------------------------
            /// Default constructor. Declared private to ensure the system is created
            /// through State::CreateSystem<>().
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            ModelFactory() = default;
        };
    }
}

#endif