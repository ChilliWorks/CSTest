//
//  State.cpp
//  CSTest
//  Created by HMcLaughlin on 13/07/2015.
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

#include <DownloadProgress/State.h>

#include <Common/Core/ResultPresenter.h>
#include <Common/UI/OptionsMenuPresenter.h>
#include <Common/UI/OptionsMenuDesc.h>
#include <Common/Core/TestNavigator.h>
#include <DownloadProgress/DownloadProgressTestSystem.h>
#include <WebView/State.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Scene.h>
#include <ChilliSource/Networking/Http.h>
#include <ChilliSource/UI/Base.h>

namespace CSTest
{
    namespace DownloadProgress
    {
        namespace
        {
            //------------------------------------------------------------------------------
            /// @author HMcLaughlin
            ///
            /// @param in_result - The HTTP result which should be converted to a string.
            ///
            /// @return The string representation of the given HTTP result.
            //------------------------------------------------------------------------------
            std::string ToString(CS::HttpResponse::Result in_result)
            {
                switch (in_result)
                {
                    case CS::HttpResponse::Result::k_completed:
                        return "Completed";
                    case CS::HttpResponse::Result::k_failed:
                        return "Failed";
                    case CS::HttpResponse::Result::k_flushed:
                        return "Flushed";
                    case CS::HttpResponse::Result::k_timeout:
                        return "Timeout";
                    default:
                        return "Unknown result type";
                }
            }
        }
        
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::CreateSystems()
        {
            CreateSystem<Common::TestNavigator>("Download Progress");
            m_optionsMenuPresenter = CreateSystem<Common::OptionsMenuPresenter>();
            m_resultPresenter = CreateSystem<Common::ResultPresenter>();
            m_downloadProgressTestSystem = CreateSystem<DownloadProgressTestSystem>();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::OnInit()
        {
            GetScene()->SetClearColour(CS::Colour(0.9f, 0.9f, 0.9f, 1.0f));
            
            m_httpRequestSystem = CS::Application::Get()->GetSystem<CS::HttpRequestSystem>();
            CS_ASSERT(m_httpRequestSystem, "Cannot complete HttpRequest smoke test as the system could not be created!");
            
            const u32 k_downloadBufferSize = 100 * 1024;
            m_httpRequestSystem->SetMaxBufferSize(k_downloadBufferSize);
            
            Common::OptionsMenuDesc optionsMenuDesc;

            optionsMenuDesc.AddButton("Test download progress", [=]()
            {
                //TODO: This doesn't work on iOS as HTTP requests aren't allowed. We need to find a file that can be downloaded over HTTPS for testing.
                m_downloadProgressTestSystem->StartDownloadTest("http://download.thinkbroadband.com/5MB.zip", [=](const CS::HttpResponse& in_completeResponse)
                {
                    PresentHttpResponse(in_completeResponse);
                });
            });
            
            m_optionsMenuPresenter->Present(optionsMenuDesc);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::PresentHttpResponse(const CS::HttpResponse& in_response)
        {
            //Ignore flush responses
            if(in_response.GetResult() != CS::HttpResponse::Result::k_flushed)
            {
                m_resultPresenter->Present("Request Status: " + ToString(in_response.GetResult()) + "\nResponse Code: " + CS::ToString(in_response.GetCode()));
            }
        }
    }
}
