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

#include <WebView/State.h>

#include <Common/Core/ResultPresenter.h>
#include <Common/Core/SmokeTester.h>
#include <Common/Core/SmokeTestSet.h>
#include <Common/Core/StateNavigator.h>

#include <HttpRequest/State.h>

#include <IntegrationTest/State.h>

#include <ChilliSource/Core/DialogueBox.h>
#include <ChilliSource/Core/Scene.h>
#include <ChilliSource/Web/Base.h>

namespace CSTest
{
    namespace WebView
    {
        namespace
        {
            using NextState = HttpRequest::State;
        }
        
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::CreateSystems()
        {
            CreateSystem<Common::StateNavigator<IntegrationTest::State>>();
            m_smokeTester = CreateSystem<Common::SmokeTester>();
            m_resultPresenter = CreateSystem<Common::ResultPresenter>();
            m_webView = CreateSystem<CSWeb::WebView>();
            CreateSystem<Common::StateNavigator<NextState>>();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::OnInit()
        {
            GetScene()->SetClearColour(CSCore::Colour(0.9f, 0.9f, 0.9f, 1.0f));
            
            Common::SmokeTestSet testSet("WebView");
            
            testSet.AddTest("From Disk", [=]()
            {
                m_webView->PresentFromFile(CSCore::StorageLocation::k_package, "WebView/ExampleWebView.html", CSCore::UnifiedVector2(1.0f, 1.0f, 0.0f, 0.0f), 0.1f, [=]()
                {
                    m_resultPresenter->Present("WebView dismissed.");
                });
            });
            
            testSet.AddTest("From Web", [=]()
            {
                m_webView->Present("http://google.com", CSCore::UnifiedVector2(0.8f, 0.8f, 0.0f, 0.0f), 0.1f, [=]()
                {
                    m_resultPresenter->Present("WebView dismissed.");
                });
            });
            
            testSet.AddTest("External Browser", [=]()
            {
                m_webView->PresentInExternalBrowser("http://google.com");
            });
            
            m_smokeTester->Present(testSet);
        }
    }
}
