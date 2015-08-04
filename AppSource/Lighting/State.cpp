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
#include <WebView/State.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Scene.h>

namespace CSTest
{
    namespace Lighting
    {
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
            
            CSCore::EntitySPtr camera = basicEntityFactory->CreateThirdPersonCamera(room);
            GetScene()->Add(camera);
        }
    }
}
