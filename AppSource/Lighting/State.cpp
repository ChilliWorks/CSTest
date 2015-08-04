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

#include <Common/Core/StateNavigator.h>
#include <Common/Core/BasicEntityFactory.h>
#include <Common/Rendering/ThirdPersonCameraComponent.h>
#include <WebView/State.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Scene.h>

namespace CSTest
{
    namespace Lighting
    {
        namespace
        {
            std::array<CSCore::Colour, 4> k_boxColours =
            {
                CSCore::Colour(1.0f, 0.1f, 0.1f, 1.0f),
                CSCore::Colour(0.1f, 1.0f, 0.1f, 1.0f),
                CSCore::Colour(0.1f, 0.1f, 1.0f, 1.0f),
                CSCore::Colour(1.0f, 1.0f, 1.0f, 1.0f)
            };
        }
        
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::CreateSystems()
        {
            CreateSystem<Common::StateNavigator<WebView::State>>();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::OnInit()
        {
            GetScene()->SetClearColour(CSCore::Colour::k_black);
            
            auto basicEntityFactory = CSCore::Application::Get()->GetSystem<Common::BasicEntityFactory>();
            
            CSCore::EntitySPtr room = basicEntityFactory->CreateRoom();
            room->GetTransform().SetPosition(0.0f, 10.0f, 0.0f);
            GetScene()->Add(room);
            
            CSCore::EntitySPtr ambientLight = basicEntityFactory->CreateAmbientLight(CSCore::Colour(0.2f, 0.2f, 0.2f, 1.0f));
            GetScene()->Add(ambientLight);
            
            CSCore::EntitySPtr directionalLight = basicEntityFactory->CreateDirectionalLight(CSCore::Colour(0.8f, 0.5f, 0.4f, 1.0f));
            directionalLight->GetTransform().SetLookAt(CSCore::Vector3(-9.0f, 9.0f, 9.0f), CSCore::Vector3::k_zero, CSCore::Vector3::k_unitPositiveY);
            GetScene()->Add(directionalLight);
            
            CSCore::EntitySPtr camera = basicEntityFactory->CreateThirdPersonCamera(room, CSCore::Vector3(0.0f, -9.0f, 0.0f));
            GetScene()->Add(camera);
            
            const u32 k_numX = 5;
            const u32 k_numZ = 5;
            const f32 k_offset = -7.0f;
            const f32 k_stride = 3.0f;
            for (u32 x = 0; x < k_numX; ++x)
            {
                for (u32 z = 0; z < k_numZ; ++z)
                {
                    CSCore::Vector3 positionInGrid(k_offset + x * k_stride, 0.0f, k_offset + z * k_stride);
                    CSCore::Vector3 size(1.0f, CSCore::Random::Generate<f32>(1.0f, 3.0f), 1.0f);
                    CSCore::Vector3 worldPosition = positionInGrid + 0.5f * size;
                    
                    CSCore::EntitySPtr box = basicEntityFactory->CreateBox(k_boxColours[CSCore::Random::Generate<u32>(0, 3)], size);
                    box->GetTransform().SetPosition(worldPosition);
                    GetScene()->Add(box);
                }
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::OnUpdate(f32 in_deltaTime)
        {
        }
    }
}
