//
//  CSTest.h
//  CSTest
//  Created by Ian Copland on 15/07/2015.
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

#ifndef _CSTEST_H_
#define _CSTEST_H_

#include <ChilliSource/ChilliSource.h>

namespace CSTest
{
    CS_FORWARDDECLARE_CLASS(App);

    namespace Accelerometer
    {
        CS_FORWARDDECLARE_CLASS(AccelerometerPresenter);
    }
    
    namespace AnimatedModel
    {
        CS_FORWARDDECLARE_CLASS(State);
    }
    
    namespace Common
    {
        CS_FORWARDDECLARE_CLASS(BasicEntityFactory);
        CS_FORWARDDECLARE_CLASS(BasicWidgetFactory);
        CS_FORWARDDECLARE_CLASS(FollowerComponent);
        CS_FORWARDDECLARE_CLASS(OptionsMenuPresenter);
        CS_FORWARDDECLARE_CLASS(OptionsMenuDesc);
        CS_FORWARDDECLARE_CLASS(OrbiterComponent);
        CS_FORWARDDECLARE_CLASS(ResultPresenter);
        CS_FORWARDDECLARE_CLASS(TestNavigator);
        CS_FORWARDDECLARE_CLASS(BackButtonSystem);
    }
    
    namespace CricketAudio
    {
        CS_FORWARDDECLARE_CLASS(State);
    }
    
    namespace CustomShader
    {
        CS_FORWARDDECLARE_CLASS(State);
    }
    
    namespace Device
    {
        CS_FORWARDDECLARE_CLASS(DevicePresenter);
        CS_FORWARDDECLARE_CLASS(State);
    }

    namespace DownloadProgress
    {
        CS_FORWARDDECLARE_CLASS(State);
        CS_FORWARDDECLARE_CLASS(DownloadProgressTestSystem);
    }
    
    namespace Gamepad
    {
        CS_FORWARDDECLARE_CLASS(GamepadPresenter);
    }

    namespace Gesture
    {
        CS_FORWARDDECLARE_CLASS(EventInfo);
        CS_FORWARDDECLARE_CLASS(GesturePresenter);
        CS_FORWARDDECLARE_CLASS(State);
    }
    
    namespace Gyroscope
    {
        CS_FORWARDDECLARE_CLASS(GyroscopePresenter);
        CS_FORWARDDECLARE_CLASS(State);
    }
    
    namespace IntegrationTest
    {
        CS_FORWARDDECLARE_CLASS(AutoRegister);
        CS_FORWARDDECLARE_CLASS(Report);
        CS_FORWARDDECLARE_CLASS(ReportPresenter);
        CS_FORWARDDECLARE_CLASS(State);
        CS_FORWARDDECLARE_CLASS(Test);
        CS_FORWARDDECLARE_CLASS(TestDesc);
        CS_FORWARDDECLARE_CLASS(Tester);
        CS_FORWARDDECLARE_CLASS(TestRegistry);
    }

    namespace Keyboard
    {
        CS_FORWARDDECLARE_CLASS(KeyboardPresenter);
    }
    
    namespace Lighting
    {
        CS_FORWARDDECLARE_CLASS(State);
    }
    
    namespace MainMenu
    {
        CS_FORWARDDECLARE_CLASS(State);
    }
    
    namespace Particle
    {
        CS_FORWARDDECLARE_CLASS(State);
    }

    namespace RemoteNotifications
    {
        CS_FORWARDDECLARE_CLASS(State);
        CS_FORWARDDECLARE_CLASS(RemoteNotificationsPresenter);
    }

    namespace Screen
    {
        CS_FORWARDDECLARE_CLASS(ScreenPresenter);
        CS_FORWARDDECLARE_CLASS(State);
    }
    
    namespace SmokeTest
    {
        CS_FORWARDDECLARE_CLASS(State);
    }

    namespace TextEntry
    {
        CS_FORWARDDECLARE_CLASS(TextEntryPresenter);
        CS_FORWARDDECLARE_CLASS(State);
    }
    
    namespace Sprite
    {
        CS_FORWARDDECLARE_CLASS(State);
    }
    
    namespace UnitTest
    {
        CS_FORWARDDECLARE_CLASS(CSReporter);
        CS_FORWARDDECLARE_CLASS(FailedAssertion);
        CS_FORWARDDECLARE_CLASS(FailedSection);
        CS_FORWARDDECLARE_CLASS(FailedTestCase);
        CS_FORWARDDECLARE_CLASS(Report);
        CS_FORWARDDECLARE_CLASS(ReportPresenter);
        CS_FORWARDDECLARE_CLASS(State);
        CS_FORWARDDECLARE_CLASS(TestSystem);
    }

    namespace VideoPlayer
    {
        CS_FORWARDDECLARE_CLASS(State);
    }
    
    namespace WebView
    {
        CS_FORWARDDECLARE_CLASS(State);
    }
}

#endif
