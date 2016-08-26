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

#include <VideoPlayer/State.h>


#include <Common/UI/OptionsMenuPresenter.h>
#include <Common/UI/OptionsMenuDesc.h>
#include <Common/Core/TestNavigator.h>
#include <CricketAudio/State.h>


#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Core/Scene.h>
#include <ChilliSource/Core/Delegate.h>
#include <ChilliSource/Core/DialogueBox.h>
#include <ChilliSource/Video/Base.h>
#include <ChilliSource/Video/ForwardDeclarations.h>

namespace CSTest
{
    namespace VideoPlayer
    {
        //------------------------------------------------------------------------------
        void State::CreateSystems() noexcept
        {
            CreateSystem<Common::TestNavigator>("Video Player");
            m_videoPlayer = CreateSystem<CS::VideoPlayer>();
            m_optionsMenuPresenter = CreateSystem<Common::OptionsMenuPresenter>();
        }

        //------------------------------------------------------------------------------
        void State::OnInit() noexcept
        {
            GetMainScene()->SetClearColour(CS::Colour(0.9f, 0.9f, 0.9f, 1.0f));

            Common::OptionsMenuDesc optionsMenuDesc;

            m_videoDismissDelegate = CS::MakeConnectableDelegate(this, &State::OnVideoDismissed);

            optionsMenuDesc.AddButton("Play Video", [=]()
            {
                auto videoDismissConnection = m_videoDismissDelegate.OpenConnection();

                m_videoPlayer->Present(CS::StorageLocation::k_package, "Video/ChilliSourceLogo01.mp4", std::move(videoDismissConnection));
            });
                    
            
            optionsMenuDesc.AddButton("Play Video Subtitled", [=]()
            {
                auto resourcePool = CS::Application::Get()->GetResourcePool();
                auto videoSubtitles = resourcePool->LoadResource<CS::Subtitles>(CS::StorageLocation::k_package, "Video/testSubtitles.cssubtitles");
                auto videoDismissConnection = m_videoDismissDelegate.OpenConnection();

                m_videoPlayer->PresentWithSubtitles(CS::StorageLocation::k_package, "Video/ChilliSourceLogo01.mp4", videoSubtitles, std::move(videoDismissConnection), true);
            });

            m_optionsMenuPresenter->Present(optionsMenuDesc);
        }

        //------------------------------------------------------------------------------
        void State::OnVideoDismissed() noexcept
        {
            auto dialogueBoxSystem = CS::Application::Get()->GetSystem<CS::DialogueBoxSystem>();
            CS_ASSERT(dialogueBoxSystem, "No dialogue box system.");

            dialogueBoxSystem->ShowSystemDialogue(0, nullptr, "Video Player Test", "Video Dismissed.", "OK");
        }
    }
}