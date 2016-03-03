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

#include <HttpRequest/State.h>

#include <Common/Core/ResultPresenter.h>
#include <Common/Core/SmokeTester.h>
#include <Common/Core/SmokeTestSet.h>
#include <Common/Core/StateNavigator.h>
#include <HttpRequest/DownloadProgressTestSystem.h>
#include <WebView/State.h>

#include <ChilliSource/Core/DialogueBox.h>
#include <ChilliSource/Core/Scene.h>
#include <ChilliSource/Core/Time.h>
#include <ChilliSource/Networking/Http.h>
#include <ChilliSource/UI/Base.h>
#include <ChilliSource/UI/ProgressBar.h>
#include <ChilliSource/Web/Base.h>

namespace CSTest
{
    namespace HttpRequest
    {
        namespace
        {
            //------------------------------------------------------------------------------
            /// Converts a HTTP::Result to string
            ///
            /// @author HMcLaughlin
            //------------------------------------------------------------------------------
            std::string ToString(CSNetworking::HttpResponse::Result in_result)
            {
                switch (in_result)
                {
                    case ChilliSource::Networking::HttpResponse::Result::k_completed:
                        return "Completed";
                    case ChilliSource::Networking::HttpResponse::Result::k_failed:
                        return "Failed";
                    case ChilliSource::Networking::HttpResponse::Result::k_flushed:
                        return "Flushed";
                    case ChilliSource::Networking::HttpResponse::Result::k_timeout:
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
            CreateSystem<Common::StateNavigator<WebView::State>>();
            m_smokeTester = CreateSystem<Common::SmokeTester>();
            m_resultPresenter = CreateSystem<Common::ResultPresenter>();
            m_downloadProgressTestSystem = CreateSystem<DownloadProgressTestSystem>();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::OnInit()
        {
            m_httpRequestSystem = CSCore::Application::Get()->GetSystem<CSNetworking::HttpRequestSystem>();
            CS_ASSERT(m_httpRequestSystem, "Cannot complete HttpRequest smoke test as the system could not be created!");
            
            const u32 k_downloadBufferSize = 100 * 1024;//100KB
            
            //Setup the http system with a buffer size
            m_httpRequestSystem->SetMaxBufferSize(k_downloadBufferSize);
            
            GetScene()->SetClearColour(CSCore::Colour(0.9f, 0.9f, 0.9f, 1.0f));
            
            Common::SmokeTestSet testSet("HttpRequest");
            
            testSet.AddTest("Get Request", [=]()
            {
                m_httpRequestSystem->MakeGetRequest("http://www.google.com", [=](const CSNetworking::HttpRequest* in_request, const CSNetworking::HttpResponse& in_response)
                {
                    PresentHttpResponse(in_response);
                });
            });
            
            testSet.AddTest("Get Request, w-headers", [=]()
            {
                CSCore::ParamDictionary headersTest;
                headersTest["Accept-Language"] = "en-us,en;q=0.5";
                
                m_httpRequestSystem->MakeGetRequest("http://www.google.com", headersTest, [=](const CSNetworking::HttpRequest* in_request, const CSNetworking::HttpResponse& in_response)
                {
                    PresentHttpResponse(in_response);
                });
            });
            
            testSet.AddTest("Post Request", [=]()
            {
                m_httpRequestSystem->MakePostRequest("http://postcatcher.in/catchers/55b9fc6f24937d0300000292", "SomeData", [=](const CSNetworking::HttpRequest* in_request, const CSNetworking::HttpResponse& in_response)
                {
                    PresentHttpResponse(in_response);
                });
            });
            
            testSet.AddTest("Post Request, w-headers", [=]()
            {
                CSCore::ParamDictionary headersTest;
                headersTest["Accept-Language"] = "en-us,en;q=0.5";
                
                m_httpRequestSystem->MakePostRequest("http://postcatcher.in/catchers/55b9fc6f24937d0300000292", "SomeData", headersTest, [=](const CSNetworking::HttpRequest* in_request, const CSNetworking::HttpResponse& in_response)
                {
                    PresentHttpResponse(in_response);
                });
            });
            
            testSet.AddTest("Download progress", [=]()
            {
                m_downloadProgressTestSystem->StartDownloadTest("http://download.thinkbroadband.com/5MB.zip", [=](const CSNetworking::HttpResponse& in_completeResponse)
                {
                    PresentHttpResponse(in_completeResponse);
                });
            });
            
            testSet.AddTest("Check Reachability", [=]()
            {
                bool reachable = m_httpRequestSystem->CheckReachability();
                
                m_resultPresenter->Present("Rechability status - " + CSCore::ToString(reachable));
            });
            
            m_smokeTester->Present(testSet);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::PresentHttpResponse(const CSNetworking::HttpResponse& in_response)
        {
            //Ignore flush responses
            if(in_response.GetResult() != CSNetworking::HttpResponse::Result::k_flushed)
            {
                m_resultPresenter->Present("Request returned:\nStatus - " + ToString(in_response.GetResult()) + "\nResponse Code - " + CSCore::ToString(in_response.GetCode()));
            }
        }
    }
}
