//
//  State.cpp
//  CSTest
//  Created by Ian Copland on 14/03/2016.
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

#include <CricketAudio/State.h>

#include <Common/Core/SmokeTester.h>
#include <Common/Core/SmokeTestSet.h>
#include <Common/Core/StateNavigator.h>
#include <HTTPRequest/State.h>

#include <ChilliSource/Audio/CricketAudio.h>
#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Scene.h>

namespace CSTest
{
    namespace CricketAudio
    {
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::CreateSystems()
        {
            CreateSystem<Common::StateNavigator<HttpRequest::State>>();
            m_smokeTester = CreateSystem<Common::SmokeTester>();

#ifndef CS_TARGETPLATFORM_WINDOWS
            m_audioPlayer = CreateSystem<CSAudio::CkAudioPlayer>();
#endif
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::OnInit()
        {
            GetScene()->SetClearColour(CSCore::Colour(0.9f, 0.9f, 0.9f, 1.0f));
            
			Common::SmokeTestSet testSet("Cricket Audio");

#ifndef CS_TARGETPLATFORM_WINDOWS
            auto resourcePool = CSCore::Application::Get()->GetResourcePool();
            auto bank = resourcePool->LoadResource<CSAudio::CkBank>(CSCore::StorageLocation::k_package, "SFX/SFX.ckb");
            
            testSet.AddTest("Play Music", [=]()
            {
                m_audioPlayer->PlayMusic(CSCore::StorageLocation::k_package, "Music/Music01.cks");
            });
            
            testSet.AddTest("Stop Music", [=]()
            {
                m_audioPlayer->StopMusic();
            });
            
            testSet.AddTest("Play SFX 1", [=]()
            {
                m_audioPlayer->PlayEffect(bank, "Beep01");
            });
            
            testSet.AddTest("Play SFX 2", [=]()
            {
                m_audioPlayer->PlayEffect(bank, "Beep02");
            });
#endif
            
            m_smokeTester->Present(testSet);
        }
    }
}
