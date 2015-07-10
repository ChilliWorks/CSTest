//
//  App.cpp
//  CSTest
//  Created by Ian Copland on 15/10/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#include <State.h>

CSCore::Application* CreateApplication()
{
    return new CSTest::App();
}

#if defined(CS_TARGETPLATFORM_ANDROID) && defined(CS_ANDROIDFLAVOUR_GOOGLEPLAY)

std::string GetGooglePlayLvlPublicKey()
{
    //Enter your Google Play LVL public key here if you are building for Google Play on Android
    return "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAgnZYTbeRc01ZxEDhLFQd49PWtoCWak7Ia5oDSmQY/dQH5rx1X+tAI+mF8uUjIgcizlH6CJT7a6wtoaNWgKBRPmjvbqjAwMzLs0c27atVGw0peX9zyEO9JEgl6UWzPMVoUPJtkn82/Z6fYHjp92DPtiEypnJs0Fyna2ZAwH+cUwS9TUQIkFVCWa5h/VY/MXuoCyYt7SlveNAScpv1ozEzFwbtBOFYrQCgeYUGhOPyagZ1VpTZjarepCGuz+nfrti+5SvyFeS18jzU9ytsmxM7i8DtBX6VygfWelPl09wHuEOV9obmw932FCebuel57w6nFMmKw3eUSYWn6iFlh0fShwIDAQAB";
}

#endif

namespace CSTest
{
    void App::CreateSystems()
    {
        //Create systems here.
    }

    void App::OnInit()
    {
        //initialisation stuff here.
    }

    void App::PushInitialState()
    {
        GetStateManager()->Push(CSCore::StateSPtr(new State()));
    }

    void App::OnDestroy()
    {
        //destruction stuff here.
    }
}

