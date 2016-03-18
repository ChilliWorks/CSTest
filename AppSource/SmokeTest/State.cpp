//
//  State.cpp
//  CSTest
//  Created by Ian Copland on 18/03/2016.
//
//  The MIT License (MIT)
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

#include <SmokeTest/State.h>

#include <Common/UI/OptionsMenuPresenter.h>
#include <Common/UI/OptionsMenuDesc.h>
#include <Common/Core/TestNavigator.h>
#include <CricketAudio/State.h>
#include <DownloadProgress/State.h>
#include <EmailComposer/State.h>
#include <Gesture/State.h>
#include <Lighting/State.h>
#include <Particle/State.h>
#include <WebView/State.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Scene.h>
#include <ChilliSource/Core/State.h>

namespace CSTest
{
    namespace SmokeTest
    {
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::CreateSystems() noexcept
        {
            CreateSystem<Common::TestNavigator>("Smoke Tests");
            m_optionsMenuPresenter = CreateSystem<Common::OptionsMenuPresenter>();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::OnInit() noexcept
        {
            GetScene()->SetClearColour(CSCore::Colour(0.9f, 0.9f, 0.9f, 1.0f));
            
            Common::OptionsMenuDesc optionsMenuDesc;
            
            optionsMenuDesc.AddButton("Lighting", [=]()
            {
               CSCore::Application::Get()->GetStateManager()->Push(std::make_shared<Lighting::State>());
            });
            
            optionsMenuDesc.AddButton("Particles", [=]()
            {
                CSCore::Application::Get()->GetStateManager()->Push(std::make_shared<Particle::State>());
            });

            optionsMenuDesc.AddButton("Gestures", [=]()
            {
                CSCore::Application::Get()->GetStateManager()->Push(std::make_shared<Gesture::State>());
            });

#ifndef CS_TARGETPLATFORM_WINDOWS
            optionsMenuDesc.AddButton("Cricket Audio", [=]()
            {
                CSCore::Application::Get()->GetStateManager()->Push(std::make_shared<CricketAudio::State>());
            });
#endif

            optionsMenuDesc.AddButton("Download Progress", [=]()
            {
                CSCore::Application::Get()->GetStateManager()->Push(std::make_shared<DownloadProgress::State>());
            });

#ifndef CS_TARGETPLATFORM_WINDOWS
            optionsMenuDesc.AddButton("Web View", [=]()
            {
                CSCore::Application::Get()->GetStateManager()->Push(std::make_shared<WebView::State>());
            });

            optionsMenuDesc.AddButton("Email Composer", [=]()
            {
                CSCore::Application::Get()->GetStateManager()->Push(std::make_shared<EmailComposer::State>());
            });
#endif
            
            m_optionsMenuPresenter->Present(optionsMenuDesc);
        }
    }
}
