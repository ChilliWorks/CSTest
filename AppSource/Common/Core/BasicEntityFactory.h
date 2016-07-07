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

#ifndef _COMMON_CORE_BASICENTITYFACTORY_H_
#define _COMMON_CORE_BASICENTITYFACTORY_H_

#include <CSTest.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Math.h>
#include <ChilliSource/Core/System.h>

namespace CSTest
{
    namespace Common
    {
        //------------------------------------------------------------------------------
        /// A factory for common entity types.
        ///
        /// This is not thread-safe and must only be called from the main thread.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        class BasicEntityFactory final : public CS::AppSystem
        {
        public:
            CS_DECLARE_NAMEDTYPE(BasicEntityFactory);
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
            bool IsA(CS::InterfaceIDType in_interfaceId) const override;
            //------------------------------------------------------------------------------
            /// Creates a basic third person camera which will follow the given entity.
            ///
            /// @author Ian Copland
            ///
            /// @param in_target - The target entity that the camera will follow.
            /// @param in_targetOffset - [Optional] The offset from the target (in it's
            /// local space) that the camera will follow. Defaults to [0, 0].
            /// @param in_distance - [Optional] The distance from the target the camera will
            /// follow. Defaults to 7.5.
            /// @param in_horizontalAngle - [Optional] The initial horizontal angle of the
            /// camera from the target. Defaults to 0.
            /// @param in_verticalAngle - [Optional] The vertical angle of the camera from
            /// the target. Defaults to 30 degrees.
            /// @param in_horizontalAngularVelocity - [Optional] The horizontal rotation
            /// velocity around the target. Defaults to 0.1f.
            ///
            /// @return The entity.
            //------------------------------------------------------------------------------
            CS::EntityUPtr CreateThirdPersonCamera(const CS::EntitySPtr& in_target, const CS::Vector3& in_targetOffset = CS::Vector3::k_zero, f32 in_distance = 7.5f, f32 in_horizontalAngle = 0.0f,
                                                       f32 in_verticalAngle = CS::MathUtils::k_pi / 6.0f, f32 in_horizontalAngularVelocity = 0.1f);
            //------------------------------------------------------------------------------
            /// Creates an ambient light with the given colour.
            ///
            /// @author Ian Copland
            ///
            /// @param in_colour - The colour of the light.
            ///
            /// @return The entity.
            //------------------------------------------------------------------------------
            CS::EntityUPtr CreateAmbientLight(const CS::Colour& in_colour);
            //------------------------------------------------------------------------------
            /// Creates a directional light with the given colour.
            ///
            /// @author Ian Copland
            ///
            /// @param in_colour - The colour of the directional light.
            /// @param in_shadowVolume - [Optional] The shadow volume. Defaults to
            /// [30, 30, 1, 30]
            ///
            /// @return The entity.
            //------------------------------------------------------------------------------
            CS::EntityUPtr CreateDirectionalLight(const CS::Colour& in_colour, const CS::Vector4& in_shadowVolume = CS::Vector4(30.0f, 30.0f, 1.0f, 30.0f));
            //------------------------------------------------------------------------------
            /// Creates a point light with the given colour and radius.
            ///
            /// @author Ian Copland
            ///
            /// @param in_colour - The colour of the point light.
            /// @param in_radius - [Optional] The radius of the point light.
            /// @param in_intensity - [Optional] The intensity of the point light.
            ///
            /// @return The entity.
            //------------------------------------------------------------------------------
            CS::EntityUPtr CreatePointLight(const CS::Colour& in_colour, f32 in_radius = 3.0f, f32 in_intensity = 3.0f) noexcept;
            //------------------------------------------------------------------------------
            /// Creates a grey checkered room for graphical tests to take place in.
            ///
            /// @author Ian Copland
            ///
            /// @param in_size - [Optional] The size of the room. Defaults to
            /// [20, 20, 20].
            ///
            /// @return The entity.
            //------------------------------------------------------------------------------
            CS::EntityUPtr CreateRoom(const CS::Vector3& in_size = CS::Vector3(20.0f, 20.0f, 20.0f));
            //------------------------------------------------------------------------------
            /// Creates a sprite with the requested size, colour and image id. Sprite images
            /// are loaded from the sprite texture atlas.
            ///
            /// @author Ian Copland
            ///
            /// @param in_imageId - The Id of the image of the texture atlas.
            /// @param in_colour - [Optional] The colour of the sprite. Defaults to white.
            /// @param in_size - [Optional] The size of the sprite world space size of the
            ///     sprite. Defaults to (1, 1)
            ///
            /// @return The sprite entity.
            //------------------------------------------------------------------------------
            CS::EntityUPtr CreateSprite(const std::string& in_imageId, const CS::Colour& in_colour = CS::Colour::k_white, const CS::Vector2& in_size = CS::Vector2::k_one) noexcept;
    
        private:
            friend class CS::Application;
            //------------------------------------------------------------------------------
            /// A factory method for creating new instances of the system.
            ///
            /// @author Ian Copland
            ///
            /// @return The new instance.
            //------------------------------------------------------------------------------
            static BasicEntityFactoryUPtr Create();
            //------------------------------------------------------------------------------
            /// Default constructor. Declared private to ensure the system is created
            /// through State::CreateSystem<>().
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            BasicEntityFactory() = default;
            //------------------------------------------------------------------------------
            /// Initialises the system.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnInit() override;
            //------------------------------------------------------------------------------
            /// Cleans up the system.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnDestroy() override;
            
            CS::ResourcePool* m_resourcePool = nullptr;
            CS::RenderComponentFactory* m_renderComponentFactory = nullptr;
            CS::PrimitiveModelFactory* m_primitiveModelFactory = nullptr;
            u32 m_entityCount = 0;
        };
    }
}

#endif
