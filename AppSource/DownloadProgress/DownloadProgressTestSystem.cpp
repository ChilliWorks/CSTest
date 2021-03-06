//
//  DownloadProgressTestSystem.cpp
//  CSTest
//
//  Created by Hugh McLaughlin on 30/07/2015.
//  Copyright (c) 2015 Tag Games Ltd. All rights reserved.
//

#include <DownloadProgress/DownloadProgressTestSystem.h>

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
    namespace DownloadProgress
    {
        namespace
        {
            const f32 k_downloadProgressUpdateIntervalDefault = 1.0f / 30.0f;
            
            const char k_downloadProgressCSUIPath[] = "HttpRequest/DownloadProgress.csui";
            
            //------------------------------------------------------------------------------
            /// Creates a download progress widget
            ///
            /// @author HMcLaughlin
            ///
            /// @return Widget
            //------------------------------------------------------------------------------
            CS::WidgetSPtr CreateProgressWidget()
            {
                CS::WidgetFactory* widgetFactory = CS::Application::Get()->GetWidgetFactory();
                CS_ASSERT(widgetFactory, "No widget factory");
                CS::ResourcePool* resPool = CS::Application::Get()->GetResourcePool();
                CS_ASSERT(resPool, "No resource pool");
                CS::WidgetTemplateCSPtr widgetTemplate = resPool->LoadResource<CS::WidgetTemplate>(CS::StorageLocation::k_package, k_downloadProgressCSUIPath);
                CS_ASSERT(widgetTemplate, "Widget template failed to load");
                return widgetFactory->Create(widgetTemplate);
            }
        }
        
        CS_DEFINE_NAMEDTYPE(DownloadProgressTestSystem);

        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        DownloadProgressTestSystemUPtr DownloadProgressTestSystem::Create()
        {
            return DownloadProgressTestSystemUPtr(new DownloadProgressTestSystem());
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool DownloadProgressTestSystem::IsA(CS::InterfaceIDType in_interfaceId) const
        {
            return DownloadProgressTestSystem::InterfaceID == in_interfaceId;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void DownloadProgressTestSystem::OnInit()
        {
            m_downloadProgressUpdateTimer = CS::TimerUPtr(new CS::Timer());
            
            m_downloadProgressWidget = CreateProgressWidget();
            CS_ASSERT(m_downloadProgressWidget, "DownloadProgress widget cannot be NULL");
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void DownloadProgressTestSystem::StartDownloadTest(const std::string& in_downloadURL, const DownloadCompleteDelegate& in_completeDelegate)
        {
            auto httpRequestSystem = CS::Application::Get()->GetSystem<CS::HttpRequestSystem>();
            
            auto progressBar = m_downloadProgressWidget->GetWidgetRecursive("ProgressBar");
            auto progressBarComponent = progressBar->GetComponent<CS::ProgressBarUIComponent>();
            auto progressLabel = m_downloadProgressWidget->GetWidgetRecursive("ProgressText");
            auto progressTextComponent = progressLabel->GetComponent<CS::TextUIComponent>();
            
            GetState()->GetUICanvas()->AddWidget(m_downloadProgressWidget);
            
            auto request = httpRequestSystem->MakeGetRequest(in_downloadURL, [=](const CS::HttpRequest* in_request, const CS::HttpResponse& in_response)
            {
                if(in_response.GetResult() != CS::HttpResponse::Result::k_flushed)
                {
                    m_downloadProgressUpdateTimer->Stop();
                    
                    if(m_downloadProgressWidget->GetParent())
                    {
                        m_downloadProgressWidget->RemoveFromParent();
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
                if(request->GetExpectedSize() > 0)
                {
                    f32 progress = (f32)request->GetDownloadedBytes() / (f32)request->GetExpectedSize();
                    progressBarComponent->SetProgress(progress);
                    
                    std::string percentageProgress = CS::ToString((u32)(progress * 100)) + "%";
                    progressTextComponent->SetText("Total Size - " + CS::ToString(request->GetExpectedSize()) + "\nDownloaded - " + CS::ToString(request->GetDownloadedBytes()) + "\nProgress - " + percentageProgress);
                }
            });
            
            m_downloadProgressUpdateTimer->Start();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void DownloadProgressTestSystem::OnDestroy()
        {
            m_downloadProgressUpdateTimer->Stop();
            
            if(m_downloadProgressWidget->GetParent())
            {
                m_downloadProgressWidget->RemoveFromParent();
                m_downloadProgressWidget.reset();
            }
        }
    }
}