//
//  Copyright (c) 2016 Tag Games Limited
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

#include <Sprite/State.h>

#include <Common/Core/TestNavigator.h>
#include <Common/Core/BasicEntityFactory.h>
#include <Gesture/State.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Entity.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Core/Scene.h>
#include <ChilliSource/Rendering/Base.h>
#include <ChilliSource/Rendering/Sprite.h>

namespace CSTest
{
	namespace Sprite
	{
        namespace
        {
            /// Adds a grid of coloured circle sprites to the scene.
            ///
            /// @param scene
            ///     The scene to add the circles to.
            ///
            void AddSprites(CS::Scene* scene) noexcept
            {
                constexpr u32 k_numX = 5;
                constexpr u32 k_numZ = 5;
                constexpr f32 k_offset = -7.0f;
                constexpr f32 k_stride = 3.0f;
                
                static const std::array<CS::Colour, 4> k_boxColours =
                {{
                    CS::Colour(1.0f, 0.1f, 0.1f, 1.0f),
                    CS::Colour(0.1f, 1.0f, 0.1f, 1.0f),
                    CS::Colour(0.1f, 0.1f, 1.0f, 1.0f),
                    CS::Colour(1.0f, 1.0f, 1.0f, 1.0f)
                }};
                
                auto basicEntityFactory = CS::Application::Get()->GetSystem<Common::BasicEntityFactory>();
                
                for (u32 x = 0; x < k_numX; ++x)
                {
                    for (u32 z = 0; z < k_numZ; ++z)
                    {
                        CS::Vector3 positionInGrid(k_offset + x * k_stride, 0.0f, k_offset + z * k_stride);
                        CS::Vector2 size(2.0f, 2.0f);
                        auto worldPosition = positionInGrid + 0.5f * CS::Vector3(size, 0.0f);
                        auto colour = k_boxColours[CS::Random::Generate<u32>(0, u32(k_boxColours.size()) - 1)];
                        
                        CS::EntitySPtr box = basicEntityFactory->CreateSprite("Circle01", colour, size);
                        box->GetTransform().SetPosition(worldPosition);
                        scene->Add(box);
                    }
                }
            }
        }
        
		//------------------------------------------------------------------------------
		void State::CreateSystems() noexcept
		{
			CreateSystem<Common::TestNavigator>("Sprites");
		}
        
		//------------------------------------------------------------------------------
		void State::OnInit() noexcept
		{
			GetScene()->SetClearColour(CS::Colour::k_black);

			auto basicEntityFactory = CS::Application::Get()->GetSystem<Common::BasicEntityFactory>();

			CS::EntitySPtr room = basicEntityFactory->CreateRoom();
			room->GetTransform().SetPosition(0.0f, 10.0f, 0.0f);
			GetScene()->Add(room);

			auto camera = basicEntityFactory->CreateThirdPersonCamera(room, CS::Vector3(0.0f, -9.0f, 0.0f));
			GetScene()->Add(std::move(camera));

			CS::EntitySPtr ambientLight = basicEntityFactory->CreateAmbientLight(CS::Colour(0.65f, 0.65f, 0.65f, 1.0f));
			GetScene()->Add(ambientLight);

            AddSprites(GetScene());
		}
	}
}
