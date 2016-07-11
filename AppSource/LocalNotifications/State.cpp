//  The MIT License (MIT)
//
//  Copyright � 2016 Tag Games. All rights reserved.
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

#include <LocalNotifications/State.h>

#include <Common/Core/ResultPresenter.h>
#include <Common/Core/TestNavigator.h>
#include <Common/Input/BackButtonSystem.h>
#include <Common/UI/OptionsMenuPresenter.h>
#include <Common/UI/OptionsMenuDesc.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Scene.h>
#include <ChilliSource/Core/DialogueBox.h>
#include <ChilliSource/Core/Notification.h>
#include <ChilliSource/UI/Base.h>

#include <functional>

namespace CSTest
{
    namespace LocalNotifications
    {
        //------------------------------------------------------------------------------
        void State::CreateSystems() noexcept
        {
            CreateSystem<Common::TestNavigator>("Local Notifications");
            m_optionsMenuPresenter = CreateSystem<Common::OptionsMenuPresenter>();
            CreateSystem<Common::BackButtonSystem>();
        }

        //------------------------------------------------------------------------------
        void State::OnInit() noexcept
        {
            GetScene()->SetClearColour(CS::Colour(0.9f, 0.9f, 0.9f, 1.0f));
 
            m_notificationManager = CS::Application::Get()->GetSystem<CS::NotificationManager>();

            CS_ASSERT(m_notificationManager, "Could not get NotificationManager.");
            
            Common::OptionsMenuDesc optionsMenuDesc;
            CS::ParamDictionary notificationParams;

#ifdef CS_TARGETPLATFORM_ANDROID
            notificationParams.SetValue("Title", "ChilliSource Notification");
            notificationParams.SetValue("Body", "Test Successful.");
#endif

#ifdef CS_TARGETPLATFORM_IOS
            notificationParams.SetValue("Body", "ChilliSource Notification: Test Successful.");
#endif

            optionsMenuDesc.AddButton("Queue Notification", [=]()
            {
                m_notificationManager->ScheduleLocalNotificationAfterTime(u32(1), notificationParams, TimeIntervalSecs(10));
            });

            optionsMenuDesc.AddButton("Queue High-Priority Notification", [=]()
            {
                m_notificationManager->ScheduleLocalNotificationAfterTime(u32(2), notificationParams, TimeIntervalSecs(10), CS::Notification::Priority::k_high);
            });
            
            optionsMenuDesc.AddButton("Get Scheduled Notifications", [=]()
            {
                m_notificationManager->GetScheduledNotifications([=](const std::vector<ChilliSource::NotificationCSPtr>& notificationList)
                {
                    auto dialogueSystem = CS::Application::Get()->GetSystem<CS::DialogueBoxSystem>();
                    dialogueSystem->ShowSystemDialogue(0, nullptr, "Scheduled Notifications", "There are " + CS::ToString(notificationList.size()) + " scheduled notifications.", "OK");
                });
            });

            optionsMenuDesc.AddButton("Cancel all Notifications", [=]()
            {
                m_notificationManager->CancelAll();
            });

            auto dialogueSystem = CS::Application::Get()->GetSystem<CS::DialogueBoxSystem>();
            
            m_eventConnections.push_back(m_notificationManager->GetReceivedEvent().OpenConnection([=](CS::NotificationManager* notificationManager, const CS::NotificationCSPtr& notification)
            {
                dialogueSystem->ShowSystemDialogue(0, nullptr, "Notification Received", notification->m_params.GetValueOrDefault("Body", "No Message"), "OK");
            }));

            m_optionsMenuPresenter->Present(optionsMenuDesc);
        }

        //--------------------------------------------------------------------------------
        void State::OnDestroy() noexcept
        {
            m_eventConnections.clear();
        }
    }
}
