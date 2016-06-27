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
            GetScene()->SetClearColour(CS::Colour(0.9f, 0.9f, 0.9f, 1.0f));

            Common::OptionsMenuDesc optionsMenuDesc;

            m_videoDismissConnection = CS::MakeConnectableDelegate(this, &State::OnVideoDismissed).OpenConnection();
            m_videoSubtitledDismissConnection = CS::MakeConnectableDelegate(this, &State::OnVideoDismissed).OpenConnection();

            optionsMenuDesc.AddButton("Play Video", [=]()
            {
                m_videoPlayer->Present(CS::StorageLocation::k_package, "Video/testVideo.mp4", std::move(m_videoDismissConnection));
            });
                    
            auto resourcePool = CS::Application::Get()->GetResourcePool();
            m_videoSubtitles = resourcePool->LoadResource<CS::Subtitles>(CS::StorageLocation::k_package, "Video/testSubtitles.cssubtitles");
            
            optionsMenuDesc.AddButton("Play Video Subtitled", [=]()
            {
                m_videoPlayer->PresentWithSubtitles(CS::StorageLocation::k_package, "Video/testVideo.mp4", m_videoSubtitles, std::move(m_videoSubtitledDismissConnection), true);
            });

            m_optionsMenuPresenter->Present(optionsMenuDesc);
        }

        //------------------------------------------------------------------------------
        void State::OnVideoDismissed() noexcept
        {
            // Pop up box?
        }
    }
}