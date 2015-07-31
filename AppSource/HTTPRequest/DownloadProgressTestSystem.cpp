//
//  DownloadProgressTestSystem.cpp
//  CSTest
//
//  Created by Hugh McLaughlin on 30/07/2015.
//  Copyright (c) 2015 Tag Games Ltd. All rights reserved.
//

#include <HttpRequest/DownloadProgressTestSystem.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Core/State.h>
#include <ChilliSource/Core/Time.h>
#include <ChilliSource/Networking/Http.h>
#include <ChilliSource/UI/Base.h>
#include <ChilliSource/UI/ProgressBar.h>
#include <ChilliSource/UI/Text.h>

namespace CSTest
{
    namespace HttpRequest
    {
        CS_DEFINE_NAMEDTYPE(DownloadProgressTestSystem);

        namespace
        {
            
            const f32 k_downloadProgressUpdateIntervalDefault = 1.0f / 30.0f;
            
            const char k_downloadProgressCSUIPath[] = "HttpRequest/DownloadProgress.csui";
        }
        
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        DownloadProgressTestSystemUPtr DownloadProgressTestSystem::Create()
        {
            DownloadProgressTestSystemUPtr system(new DownloadProgressTestSystem());
            return system;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool DownloadProgressTestSystem::IsA(CSCore::InterfaceIDType in_interfaceId) const
        {
            return DownloadProgressTestSystem::InterfaceID == in_interfaceId;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void DownloadProgressTestSystem::OnInit()
        {
            m_downloadProgressUpdateTimer = CSCore::TimerSPtr(new CSCore::Timer());
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void DownloadProgressTestSystem::StartDownloadTest(const std::string& in_downloadURL, const DownloadCompleteDelegate& in_completeDelegate)
        {
            auto httpRequestSystem = CSCore::Application::Get()->GetSystem<CSNetworking::HttpRequestSystem>();
            
            auto downloadProgressWidget = CreateProgressWidget();
            CS_ASSERT(downloadProgressWidget, "DownloadProgress widget cannot be NULL");
            
            auto progressBar = downloadProgressWidget->GetWidgetRecursive("ProgressBar");
            auto progressBarComponent = progressBar->GetComponent<CSUI::ProgressBarComponent>();
            auto progressLabel = downloadProgressWidget->GetWidgetRecursive("ProgressText");
            auto progressTextComponent = progressLabel->GetComponent<CSUI::TextComponent>();
            
            GetState()->GetUICanvas()->AddWidget(downloadProgressWidget);
            
            auto request = httpRequestSystem->MakeGetRequest(in_downloadURL, [=](const CSNetworking::HttpRequest* in_request, const CSNetworking::HttpResponse& in_response)
            {
                if(in_response.GetResult() != CSNetworking::HttpResponse::Result::k_flushed)
                {
                    m_downloadProgressUpdateTimer->Stop();
                    
                    if(downloadProgressWidget->GetParent())
                    {
                        downloadProgressWidget->RemoveFromParent();
                    }
                    
                    if(in_completeDelegate)
                    {
                        in_completeDelegate(in_response);
                    }
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
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void DownloadProgressTestSystem::OnDestroy()
        {
            m_downloadProgressUpdateTimer->Stop();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        CSUI::WidgetSPtr DownloadProgressTestSystem::CreateProgressWidget()
        {
            CSUI::WidgetFactory* widgetFactory = CSCore::Application::Get()->GetWidgetFactory();
            CS_ASSERT(widgetFactory, "No widget factory");
            CSCore::ResourcePool* resPool = CSCore::Application::Get()->GetResourcePool();
            CS_ASSERT(resPool, "No resource pool");
            CSUI::WidgetTemplateCSPtr widgetTemplate = resPool->LoadResource<CSUI::WidgetTemplate>(CSCore::StorageLocation::k_package, k_downloadProgressCSUIPath);
            CS_ASSERT(widgetTemplate, "Widget template failed to load");
            return widgetFactory->Create(widgetTemplate);
        }
    }
}