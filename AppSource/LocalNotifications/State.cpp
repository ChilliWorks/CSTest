//  The MIT License (MIT)
//
//  Copyright © 2016 Tag Games. All rights reserved.
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
#include <Common/UI/OptionsMenuPresenter.h>
#include <Common/UI/OptionsMenuDesc.h>
#include <Common/Core/TestNavigator.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Scene.h>
#include <ChilliSource/Networking/Http.h>
#include <ChilliSource/Core/Notification.h>
#include <ChilliSource/UI/Base.h>

#include <functional>

namespace CSTest
{
    namespace LocalNotifications
    {
        //------------------------------------------------------------------------------
        void State::CreateSystems()
        {
            CreateSystem<Common::TestNavigator>("Local Notifications");
            m_optionsMenuPresenter = CreateSystem<Common::OptionsMenuPresenter>();
        }
        //------------------------------------------------------------------------------
        void State::OnInit()
        {
            GetScene()->SetClearColour(CS::Colour(0.9f, 0.9f, 0.9f, 1.0f));
 
            m_notificationManager = CS::Application::Get()->GetSystem<CS::NotificationManager>();

            if (m_notificationManager == nullptr)
            {
                CS_LOG_ERROR("Could not get Notification Manager!");
            }

            Common::OptionsMenuDesc optionsMenuDesc;
            CS::ParamDictionary notificationParams;

#ifdef CS_TARGETPLATFORM_ANDROID
            notificationParams.SetValue("Title", "ChilliSource Notification");
            notificationParams.SetValue("Body", "Test Successful.");
#endif

#ifdef CS_TARGETPLATFORM_IOS
            notificationParams.SetValue("Body", "ChilliSource Notification: Test Successful.");
#endif

            optionsMenuDesc.AddButton("Queue Standard Notification for 10 seconds", [=]()
            {
                m_notificationManager->ScheduleLocalNotificationAfterTime(u32(1), notificationParams, TimeIntervalSecs(10));
            });

            optionsMenuDesc.AddButton("Queue High-Priority Notification for 10 seconds", [=]()
            {
                m_notificationManager->ScheduleLocalNotificationAfterTime(u32(1), notificationParams, TimeIntervalSecs(10), CS::Notification::Priority::k_high);
            });

            optionsMenuDesc.AddButton("Cancel all upcoming notifications", [=]()
            {
                m_notificationManager->CancelAll();
            });

            m_optionsMenuPresenter->Present(optionsMenuDesc);
        }
    }
}
