//
//  State.cpp
//  CSTest
//  Created by Ian Copland on 15/03/2016.
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

#include <IntegrationTest/State.h>

#include <Common/Core/StateNavigator.h>
#include <IntegrationTest/TestSystem/Tester.h>
#include <IntegrationTest/TestSystem/TestRegistry.h>
#include <Lighting/State.h>

#include <ChilliSource/Core/Scene.h>

namespace CSTest
{
    namespace IntegrationTest
    {
        namespace
        {
            using NextState = Lighting::State;
            
            CSIT_TESTCASE(TestCase01)
            {
                CSIT_TEST(Test01)
                {
                    CSIT_ASSERT(true, "Example assertion failure message.");
                    
                    CSIT_PASS();
                }
                
                CSIT_TEST(Test02)
                {
                    CSIT_ASSERT(false, "Example assertion failure message.");
                    CSIT_ASSERT(false, "Example assertion failure message.");
                    CSIT_ASSERT(false, "Example assertion failure message.");
                    CSIT_ASSERT(false, "Example assertion failure message.");
                    
                    CSIT_PASS();
                }
                
                CSIT_TEST(Test03)
                {
                    CSIT_ASSERT(true, "Example assertion failure message.");
                    
                    CSIT_PASS();
                }
            };
            
            CSIT_TESTCASE(TestCase02)
            {
                CSIT_TEST(Test01)
                {
                    CSIT_ASSERT(true, "Example assertion failure message.");
                    
                    CSIT_PASS();
                }
                
                CSIT_TEST(Test02)
                {
                    CSIT_ASSERT(false, "Example assertion failure message.");
                    CSIT_ASSERT(false, "Example assertion failure message.");
                    CSIT_ASSERT(false, "Example assertion failure message.");
                    CSIT_ASSERT(false, "Example assertion failure message.");
                    
                    CSIT_PASS();
                }
                
                CSIT_TEST(Test03)
                {
                    CSIT_ASSERT(true, "Example assertion failure message.");
                    
                    CSIT_PASS();
                }
            };
        }
        
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::CreateSystems()
        {
            CreateSystem<Common::StateNavigator<NextState>>();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::OnInit()
        {
            GetScene()->SetClearColour(CSCore::Colour(0.9f, 0.9f, 0.9f, 1.0f));
            
            auto progressUpdateDelegate = [](const TestDesc& in_testDesc, u32 in_testIndex, u32 in_numTests)
            {
                CS_LOG_VERBOSE(CSCore::ToString(in_testIndex) + "/" + CSCore::ToString(in_numTests) + ": [" + in_testDesc.GetTestCaseName() + "] " + in_testDesc.GetTestName());
            };
            
            auto completionDelegate = []()
            {
                CS_LOG_VERBOSE("Testing complete!");
            };
            
            m_tester = TesterUPtr(new Tester(TestRegistry::Get().GetTests(), progressUpdateDelegate, completionDelegate));
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::OnUpdate(f32 in_deltaTime)
        {
        }
    }
}
