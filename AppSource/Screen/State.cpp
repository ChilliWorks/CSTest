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

#include <Screen/State.h>

#include <Common/UI/OptionsMenuPresenter.h>
#include <Common/UI/OptionsMenuDesc.h>
#include <Common/Core/TestNavigator.h>
#include <Screen/ScreenPresenter.h>
#include <CricketAudio/State.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Scene.h>


namespace CSTest
{
    namespace Screen
    {
        //------------------------------------------------------------------------------
        void State::CreateSystems()
        {
            CreateSystem<Common::TestNavigator>("Screen");
            CreateSystem<ScreenPresenter>();
            m_optionsMenuPresenter = CreateSystem<Common::OptionsMenuPresenter>();
        }

        //------------------------------------------------------------------------------
        void State::OnInit()
        {
            GetMainScene()->SetClearColour(CS::Colour(0.9f, 0.9f, 0.9f, 1.0f));

#ifdef CS_TARGETPLATFORM_WINDOWS

            Common::OptionsMenuDesc optionsMenuDesc;

            optionsMenuDesc.AddButton("Windowed", [=]()
            {
                auto screen = CS::Application::Get()->GetSystem<CS::Screen>();
                screen->SetDisplayMode(CS::Screen::DisplayMode::k_windowed);
            });

            optionsMenuDesc.AddButton("Fullscreen", [=]()
            {
                auto screen = CS::Application::Get()->GetSystem<CS::Screen>();
                screen->SetResolution(screen->GetSupportedResolutions().at(0));
                screen->SetDisplayMode(CS::Screen::DisplayMode::k_fullscreen);
            });

            m_optionsMenuPresenter->Present(optionsMenuDesc);

#endif

        }
    }
}
