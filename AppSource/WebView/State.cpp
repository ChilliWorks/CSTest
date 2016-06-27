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
#include <Common/Input/BackButton.h>
#include <Common/UI/OptionsMenuPresenter.h>
#include <Common/UI/OptionsMenuDesc.h>
#include <Common/Core/TestNavigator.h>
#include <EmailComposer/State.h>

#include <ChilliSource/Core/Scene.h>
#include <ChilliSource/Web/Base.h>

namespace CSTest
{
    namespace WebView
    {
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::CreateSystems()
        {
            CreateSystem<Common::TestNavigator>("Web View");
            m_optionsMenuPresenter = CreateSystem<Common::OptionsMenuPresenter>();
            m_resultPresenter = CreateSystem<Common::ResultPresenter>();
            m_webView = CreateSystem<CS::WebView>();
            m_backButtonSystem = CreateSystem<Common::BackButtonSystem>();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::OnInit()
        {
            GetScene()->SetClearColour(CS::Colour(0.9f, 0.9f, 0.9f, 1.0f));
            
            Common::OptionsMenuDesc optionsMenuDesc;
            
            if (m_webView != nullptr)
            {
                optionsMenuDesc.AddButton("From Disk", [=]()
                {
                    m_webView->PresentFromFile(CS::StorageLocation::k_package, "WebView/ExampleWebView.html", CS::UnifiedVector2(1.0f, 1.0f, 0.0f, 0.0f), 0.1f, [=]()
                    {
                        m_resultPresenter->Present("WebView dismissed.");
                    });
                });
                
                optionsMenuDesc.AddButton("From Web", [=]()
                {
                    m_webView->Present("https://google.com", CS::UnifiedVector2(0.8f, 0.8f, 0.0f, 0.0f), 0.1f, [=]()
                    {
                        m_resultPresenter->Present("WebView dismissed.");
                    });
                });
                
                optionsMenuDesc.AddButton("External Browser", [=]()
                {
                    m_webView->PresentInExternalBrowser("https://google.com");
                });
            }
            
            m_optionsMenuPresenter->Present(optionsMenuDesc);
        }
    }
}
