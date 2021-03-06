//
//  App.cpp
//  CSTest
//  Created by Ian Copland on 13/07/2015.
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

#include <App.h>

#include <Common/Core/BasicEntityFactory.h>
#include <Common/UI/BasicWidgetFactory.h>
#include <MainMenu/State.h>

#include <ChilliSource/Audio/CricketAudio.h>
#include <ChilliSource/Core/Entity.h>
#include <ChilliSource/Input/Accelerometer.h>
#include <ChilliSource/Input/Gyroscope.h>
#include <ChilliSource/Core/Notification.h>
#include <ChilliSource/Input/Keyboard.h>
#include <ChilliSource/Rendering/Model.h>
#include <ChilliSource/Networking/Http.h>
#include <ChilliSource/Video/Base.h>


//------------------------------------------------------------------------------
/// Creates the application instance for this app.
///
/// @author Ian Copland
///
/// @return The new Application instance.
//------------------------------------------------------------------------------
CS::Application* CreateApplication(CS::SystemInfoCUPtr systemInfo) noexcept
{
    return new CSTest::App(std::move(systemInfo));
}

#if defined(CS_TARGETPLATFORM_ANDROID) && defined(CS_ANDROIDFLAVOUR_GOOGLEPLAY)

//------------------------------------------------------------------------------
/// Returns the LVL Public key used by Google Play Android builds for a number
/// of systems, including IAP and APK expansion downloading.
///
/// @author Ian Copland
///
/// @return The Google Play LVL key.
//------------------------------------------------------------------------------
std::string GetGooglePlayLvlPublicKey()
{
    //Enter your Google Play LVL public key here if you are building for Google Play on Android
    return "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAgnZYTbeRc01ZxEDhLFQd49PWtoCWak7Ia5oDSmQY/dQH5rx1X+tAI+mF8uUjIgcizlH6CJT7a6wtoaNWgKBRPmjvbqjAwMzLs0c27atVGw0peX9zyEO9JEgl6UWzPMVoUPJtkn82/Z6fYHjp92DPtiEypnJs0Fyna2ZAwH+cUwS9TUQIkFVCWa5h/VY/MXuoCyYt7SlveNAScpv1ozEzFwbtBOFYrQCgeYUGhOPyagZ1VpTZjarepCGuz+nfrti+5SvyFeS18jzU9ytsmxM7i8DtBX6VygfWelPl09wHuEOV9obmw932FCebuel57w6nFMmKw3eUSYWn6iFlh0fShwIDAQAB";
}

#endif

namespace CSTest
{
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    App::App(CS::SystemInfoCUPtr systemInfo) noexcept
        : Application(std::move(systemInfo))
    { 
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void App::CreateSystems() noexcept
    {
        CreateSystem<CS::CSModelProvider>();
        CreateSystem<CS::CSAnimProvider>();
        
#ifndef CS_TARGETPLATFORM_RPI
        CreateSystem<CS::CricketAudioSystem>();
		CreateSystem<CS::CkBankProvider>();
#endif

        CreateSystem<CS::PrimitiveEntityFactory>();
        CreateSystem<CS::HttpRequestSystem>();
        CreateSystem<CS::PrimitiveModelFactory>();

#if defined CS_TARGETPLATFORM_IOS || defined CS_TARGETPLATFORM_ANDROID
        CreateSystem<CS::AppNotificationSystem>();
        CreateSystem<CS::LocalNotificationSystem>();
        CreateSystem<CS::RemoteNotificationSystem>();
        CreateSystem<CS::NotificationManager>();
		CreateSystem<CS::Accelerometer>();
        CreateSystem<CS::Gyroscope>();
#endif
        
        CreateSystem<CS::CSSubtitlesProvider>();

        CreateSystem<Common::BasicEntityFactory>();
        CreateSystem<Common::BasicWidgetFactory>();
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void App::OnInit() noexcept
    {
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void App::PushInitialState() noexcept
    {
        GetStateManager()->Push(CS::StateSPtr(new MainMenu::State()));
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void App::OnDestroy() noexcept
    {
    }
}

