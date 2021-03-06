//
//  TestSystem.cpp
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

#include <UnitTest/TestSystem/TestSystem.h>

#include <UnitTest/TestSystem/CSReporter.h>

#define CATCH_CONFIG_RUNNER
#include <catch.hpp>

namespace CSTest
{
    namespace UnitTest
    {
        CS_DEFINE_NAMEDTYPE(TestSystem);
        
        Catch::Session TestSystem::s_session;
        
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        TestSystemUPtr TestSystem::Create()
        {
            return TestSystemUPtr(new TestSystem());
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool TestSystem::IsA(CS::InterfaceIDType in_interfaceId) const
        {
            return (TestSystem::InterfaceID == in_interfaceId);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        Report TestSystem::PerformTests()
        {
            s_session.configData().reporterNames = { "cs" };

            s_session.run();
            
            return CSReporter::getReport();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void TestSystem::OnInit()
        {
        }
    }
}
