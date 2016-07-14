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

#include <RemoteNotifications/RemoteNotificationsPresenter.h>

#include <Common/UI/BasicWidgetFactory.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Cryptographic/BaseEncoding.h>
#include <ChilliSource/Core/DialogueBox.h>
#include <ChilliSource/Core/Notification.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Core/State.h>
#include <ChilliSource/Rendering/Font.h>
#include <ChilliSource/UI/Base.h>
#include <ChilliSource/UI/Text.h>

namespace CSTest
{
    namespace RemoteNotifications
    {
        CS_DEFINE_NAMEDTYPE(RemoteNotificationsPresenter);

        //------------------------------------------------------------------------------
        RemoteNotificationsPresenterUPtr RemoteNotificationsPresenter::Create() noexcept
        {
            return RemoteNotificationsPresenterUPtr(new RemoteNotificationsPresenter());
        }

        //------------------------------------------------------------------------------
        bool RemoteNotificationsPresenter::IsA(CS::InterfaceIDType interfaceId) const noexcept
        {
            return (RemoteNotificationsPresenter::InterfaceID == interfaceId);
        }

        //------------------------------------------------------------------------------
        void RemoteNotificationsPresenter::InitUI() noexcept
        {
            auto resourcePool = CS::Application::Get()->GetResourcePool();
            auto smallFont = resourcePool->LoadResource<CS::Font>(CS::StorageLocation::k_package, "Fonts/ArialSmall.csfont");
            
            auto basicWidgetFactory = CS::Application::Get()->GetSystem<Common::BasicWidgetFactory>();
            auto label = basicWidgetFactory->CreateLabel(CS::Vector2(0.9f, 0.8f), smallFont, "", CS::AlignmentAnchor::k_middleCentre,
                                                         CS::HorizontalTextJustification::k_left, CS::VerticalTextJustification::k_top);
            m_textComponent = label->GetComponent<CS::TextUIComponent>();

            auto uiCanvas = GetState()->GetUICanvas();
            uiCanvas->AddWidget(std::move(label));

            m_textComponent->SetText("No connection. Token not generated.");
        }

        //------------------------------------------------------------------------------
        void RemoteNotificationsPresenter::AddEventHandler() noexcept
        {   
            CS_ASSERT(!m_remoteNotificationSystem, "Cannot add event handler more than once!");

            m_remoteNotificationSystem = CS::Application::Get()->GetSystem<CS::RemoteNotificationSystem>();

            m_eventConnection = m_remoteNotificationSystem->GetReceivedEvent().OpenConnection([=](const CS::NotificationCSPtr& notification)
            {
                    CS::Application::Get()->GetSystem<CS::DialogueBoxSystem>()->ShowSystemDialogue(0, nullptr, "Remote Notification Received", notification->m_params.GetValueOrDefault("Body", "No Message"), "OK");
            });

            m_remoteNotificationSystem->RequestRemoteToken([=](const std::string& token)
            {
                CS_LOG_WARNING("Base64 Device Token: " + token);
                m_textComponent->SetText("Device token can be retrieved from device log.");
            });

            m_remoteNotificationSystem->SetEnabled(true);
        }

        //------------------------------------------------------------------------------
        void RemoteNotificationsPresenter::OnInit() noexcept
        {
            InitUI();
            AddEventHandler();
        }

        //------------------------------------------------------------------------------
        void RemoteNotificationsPresenter::OnDestroy() noexcept
        {
            m_eventConnection.reset();
        }
    }
}
