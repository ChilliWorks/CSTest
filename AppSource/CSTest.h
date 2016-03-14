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
    
    namespace Common
    {
        CS_FORWARDDECLARE_CLASS(BasicEntityFactory);
        CS_FORWARDDECLARE_CLASS(BasicWidgetFactory);
        CS_FORWARDDECLARE_CLASS(OrbiterComponent);
        CS_FORWARDDECLARE_CLASS(ResultPresenter);
        CS_FORWARDDECLARE_CLASS(SmokeTester);
        CS_FORWARDDECLARE_CLASS(SmokeTestSet);
        CS_FORWARDDECLARE_CLASS(FollowerComponent);
    }
    
    namespace CricketAudio
    {
        CS_FORWARDDECLARE_CLASS(State);
    }
    
    namespace IntegrationTest
    {
        CS_FORWARDDECLARE_CLASS(CSReporter);
        CS_FORWARDDECLARE_CLASS(FailedAssertion);
        CS_FORWARDDECLARE_CLASS(Report);
        CS_FORWARDDECLARE_CLASS(ReportPresenter);
        CS_FORWARDDECLARE_CLASS(State);
        CS_FORWARDDECLARE_CLASS(TestCase);
        CS_FORWARDDECLARE_CLASS(TestSystem);
    }
    
    namespace Gesture
    {
        CS_FORWARDDECLARE_CLASS(EventInfo);
        CS_FORWARDDECLARE_CLASS(GesturePresenter);
        CS_FORWARDDECLARE_CLASS(State);
    }
    
    namespace HttpRequest
    {
        CS_FORWARDDECLARE_CLASS(State);
        CS_FORWARDDECLARE_CLASS(DownloadProgressTestSystem);
    }
    
    namespace Lighting
    {
        CS_FORWARDDECLARE_CLASS(State);
    }
    
    namespace Particle
    {
        CS_FORWARDDECLARE_CLASS(State);
    }
    
    namespace WebView
    {
        CS_FORWARDDECLARE_CLASS(State);
    }
}

#endif
