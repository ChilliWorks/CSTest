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

#include <HTTPRequest/State.h>

#include <Common/Core/ResultPresenter.h>
#include <Common/Core/SmokeTester.h>
#include <Common/Core/SmokeTestSet.h>
#include <Common/Core/StateNavigator.h>
#include <IntegrationTest/State.h>

#include <ChilliSource/Core/DialogueBox.h>
#include <ChilliSource/Core/Scene.h>
#include <ChilliSource/Core/Time.h>
#include <ChilliSource/Networking/Http.h>
#include <ChilliSource/UI/Base.h>
#include <ChilliSource/UI/ProgressBar.h>
#include <ChilliSource/Web/Base.h>

namespace CSTest
{
    namespace HTTPRequest
    {
        namespace
        {
            const u32 k_downloadBufferSize = 256;
            
            const f32 k_downloadProgressUpdateIntervalDefault = 1.0f / 30.0f;
            
            const char k_downloadProgressCSUIPath[] = "HttpRequest/DownloadProgress.csui";
            
            //------------------------------------------------------------------------------
            /// Converts a HTTP::Result to string
            ///
            /// @author HMcLaughlin
            //------------------------------------------------------------------------------
            std::string ConvertResultToString(CSNetworking::HttpResponse::Result in_result)
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
            CreateSystem<Common::StateNavigator<IntegrationTest::State>>();
            m_smokeTester = CreateSystem<Common::SmokeTester>();
            m_resultPresenter = CreateSystem<Common::ResultPresenter>();
            
            m_downloadProgressUpdateTimer = CSCore::TimerSPtr(new CSCore::Timer());
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::OnInit()
        {
            m_httpRequestSystem = CSCore::Application::Get()->GetSystem<CSNetworking::HttpRequestSystem>();
            CS_ASSERT(m_httpRequestSystem, "Cannot complete HttpRequest smoke test as the system could not be created!");
            
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
            
            testSet.AddTest("Get Request, with headers", [=]()
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
            
            testSet.AddTest("Post Request, with headers", [=]()
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
                auto downloadProgressWidget = CreateProgressWidget();
                CS_ASSERT(downloadProgressWidget, "DownloadProgress widget cannot be NULL");
                
                auto progressBar = downloadProgressWidget->GetWidgetRecursive("ProgressBar");
                auto progressBarComponent = progressBar->GetComponent<CSUI::ProgressBarComponent>();
                auto progressLabel = downloadProgressWidget->GetWidgetRecursive("ProgressText");
                auto progressTextComponent = progressLabel->GetComponent<CSUI::TextComponent>();
                
                GetUICanvas()->AddWidget(downloadProgressWidget);
                
                auto request = m_httpRequestSystem->MakeGetRequest("http://c758482.r82.cf2.rackcdn.com/Sublime%20Text%20Build%203083.dmg", [=](const CSNetworking::HttpRequest* in_request, const CSNetworking::HttpResponse& in_response)
                {
                    if(in_response.GetResult() != CSNetworking::HttpResponse::Result::k_flushed)
                    {
                        m_downloadProgressUpdateTimer->Stop();
                        
                        if(downloadProgressWidget->GetParent())
                        {
                            downloadProgressWidget->RemoveFromParent();
                        }
                        
                        PresentHttpResponse(in_response);
                    }
                });
                
                //Reset the timer
                m_downloadProgressUpdateTimer->Reset();
                m_downloadProgressEventConnection = m_downloadProgressUpdateTimer->OpenConnection(k_downloadProgressUpdateIntervalDefault, [=]()
                {
                    if(request->GetExpectedTotalSize() > 0)
                    {
                        f32 progress = (f32)request->GetCurrentSize() / (f32)request->GetExpectedTotalSize();
                        progressBarComponent->SetProgress(progress);
                        
                        std::string percentageProgress = CSCore::ToString((u32)(progress * 100)) + "%";
                        progressTextComponent->SetText("Total Size - " + CSCore::ToString(request->GetExpectedTotalSize()) + "\nDownloaded - " + CSCore::ToString(request->GetCurrentSize()) + "\nProgress - " + percentageProgress);
                    }
                });
                
                m_downloadProgressUpdateTimer->Start();
            });
            
            testSet.AddTest("Check Reachability", [=]()
            {
                CSCore::ParamDictionary headersTest;
                headersTest["Accept-Language"] = "en-us,en;q=0.5";
                
                bool reachable = m_httpRequestSystem->CheckReachability();
                
                m_resultPresenter->Present("Rechability status - " + CSCore::ToString(reachable));
            });
            
            m_smokeTester->Present(testSet);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        CSUI::WidgetSPtr State::CreateProgressWidget()
        {
            CSUI::WidgetFactory* widgetFactory = CSCore::Application::Get()->GetWidgetFactory();
            CS_ASSERT(widgetFactory, "No widget factory");
            CSCore::ResourcePool* resPool = CSCore::Application::Get()->GetResourcePool();
            CS_ASSERT(resPool, "No resource pool");
            CSUI::WidgetTemplateCSPtr widgetTemplate = resPool->LoadResource<CSUI::WidgetTemplate>(CSCore::StorageLocation::k_package, k_downloadProgressCSUIPath);
            CS_ASSERT(widgetTemplate, "Widget template failed to load");
            return widgetFactory->Create(widgetTemplate);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::PresentHttpResponse(const CSNetworking::HttpResponse& in_response)
        {
            //Ignore flush responses
            if(in_response.GetResult() != CSNetworking::HttpResponse::Result::k_flushed)
            {
                m_resultPresenter->Present("Request returned:\n Status - " + ConvertResultToString(in_response.GetResult()) + "\nResponse Code - " + CSCore::ToString(in_response.GetCode()));
            }
        }
    }
}
