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

#include <RemoteNotifications/State.h>

#include <Common/UI/OptionsMenuPresenter.h>
#include <Common/UI/OptionsMenuDesc.h>
#include <Common/Core/TestNavigator.h>

#include <RemoteNotifications/RemoteNotificationsPresenter.h>

#include <ChilliSource/Core/Scene.h>

namespace CSTest
{
    namespace RemoteNotifications
    {
        //------------------------------------------------------------------------------
        void State::CreateSystems()
        {
            CreateSystem<Common::TestNavigator>("Remote Notifications");
            CreateSystem<RemoteNotificationsPresenter>();
        }

        //------------------------------------------------------------------------------
        void State::OnInit()
        {
            GetScene()->SetClearColour(CS::Colour(0.9f, 0.9f, 0.9f, 1.0f));
        }
    }
}
