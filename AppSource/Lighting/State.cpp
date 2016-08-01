//
//  State.cpp
//  CSTest
//  Created by Ian Copland on 13/07/2015.
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

#include <Lighting/State.h>

#include <Common/Core/TestNavigator.h>
#include <Common/Core/BasicEntityFactory.h>
#include <Common/Behaviour/FollowerComponent.h>
#include <Common/Behaviour/OrbiterComponent.h>
#include <Common/Input/BackButtonSystem.h>
#include <Particle/State.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Scene.h>

#include <array>

namespace CSTest
{
    namespace Lighting
    {
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::CreateLights(const CS::EntitySPtr& in_room)
        {
            const CS::Vector3 k_offset(0.0f, -10.0f, 0.0f);
            const f32 k_distance = 10.0f;
            const f32 k_lowerVerticalAngle = CS::MathUtils::k_pi / 4.0f;
            const f32 k_upperVerticalAngle = CS::MathUtils::k_pi / 3.0f;
            
            auto basicEntityFactory = CS::Application::Get()->GetSystem<Common::BasicEntityFactory>();
            
            // ambient light
            CS::EntitySPtr ambientLight = basicEntityFactory->CreateAmbientLight(CS::Colour(0.3f, 0.3f, 0.3f, 1.0f));
            GetMainScene()->Add(ambientLight);
            
            // directional light
            //TODO: We should provide multiple spinning lights, however issues with shadows in the engine mean we cannot for the time being.
            f32 horizontalAngle = CS::Random::Generate<f32>(0.0f, CS::MathUtils::k_pi * 2.0f);
            f32 verticalAngle = CS::Random::Generate<f32>(k_lowerVerticalAngle, k_upperVerticalAngle);
            auto followerComponent = std::make_shared<Common::FollowerComponent>(in_room, k_offset, k_distance, horizontalAngle, verticalAngle);
            
            CS::EntitySPtr directionalLight = basicEntityFactory->CreateDirectionalLight(CS::Colour(0.8f, 0.5f, 0.4f));
            directionalLight->AddComponent(followerComponent);
            GetMainScene()->Add(directionalLight);
            
            // point light
            CS::EntitySPtr pointLight = basicEntityFactory->CreatePointLight(CS::Colour(0.1f, 1.0f, 0.2f));
            pointLight->GetTransform().SetPosition(7.0f, 3.0f, 7.0f);
            GetMainScene()->Add(pointLight);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::CreateBoxes()
        {
            const u32 k_numX = 5;
            const u32 k_numZ = 5;
            const f32 k_offset = -7.0f;
            const f32 k_stride = 3.0f;
            
            static const std::array<CS::Colour, 4> k_boxColours =
            {{
                CS::Colour(1.0f, 0.1f, 0.1f, 1.0f),
                CS::Colour(0.1f, 1.0f, 0.1f, 1.0f),
                CS::Colour(0.1f, 0.1f, 1.0f, 1.0f),
                CS::Colour(1.0f, 1.0f, 1.0f, 1.0f)
            }};
            
            auto primitiveEntityFactory = CS::Application::Get()->GetSystem<CS::PrimitiveEntityFactory>();
            
            for (u32 x = 0; x < k_numX; ++x)
            {
                for (u32 z = 0; z < k_numZ; ++z)
                {
                    CS::Vector3 positionInGrid(k_offset + x * k_stride, 0.0f, k_offset + z * k_stride);
                    CS::Vector3 size(1.0f, CS::Random::Generate<f32>(1.0f, 3.0f), 1.0f);
                    CS::Vector3 worldPosition = positionInGrid + 0.5f * size;
                    
                    CS::EntitySPtr box = primitiveEntityFactory->CreateBox(k_boxColours[CS::Random::Generate<u32>(0, u32(k_boxColours.size()) - 1)], size);
                    box->GetTransform().SetPosition(worldPosition);
                    GetMainScene()->Add(box);
                }
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::CreateSystems()
        {
            CreateSystem<Common::TestNavigator>("Lighting");
            CreateSystem<Common::BackButtonSystem>();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::OnInit()
        {
            GetMainScene()->SetClearColour(CS::Colour::k_black);
            
            auto basicEntityFactory = CS::Application::Get()->GetSystem<Common::BasicEntityFactory>();
            
            CS::EntitySPtr room = basicEntityFactory->CreateRoom();
            room->GetTransform().SetPosition(0.0f, 10.0f, 0.0f);
            GetMainScene()->Add(room);
            
            CS::EntitySPtr camera = basicEntityFactory->CreateThirdPersonCamera(room, CS::Vector3(0.0f, -9.0f, 0.0f));
            GetMainScene()->Add(camera);
            
            CreateLights(room);
            CreateBoxes();
        }
    }
}
