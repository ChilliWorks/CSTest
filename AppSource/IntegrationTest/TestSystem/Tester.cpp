//
//  Tester.cpp
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

#include <IntegrationTest/TestSystem/Tester.h>

#include <IntegrationTest/TestSystem/TestRegistry.h>

namespace CSTest
{
    namespace IntegrationTest
    {
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        Tester::Tester(const ProgressUpdateDelegate& in_progressUpdateDelegate, const CompletionDelegate& in_completionDelegate) noexcept
            : m_progressUpdateDelegate(in_progressUpdateDelegate), m_completionDelegate(in_completionDelegate)
        {
            auto tests = TestRegistry::Get().GetTests();
            m_numTests = tests.size();
            
            for (const auto& test : tests)
            {
                m_testQueue.push(test);
            }
            
            TryStartTest();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void Tester::TryStartTest() noexcept
        {
            if (m_testQueue.empty())
            {
                m_completionDelegate(Report(m_numTests, m_failedTests));
                return;
            }
            
            ++m_activeTestIndex;
            TestDesc testDesc = m_testQueue.front();
            m_testQueue.pop();
            
            m_progressUpdateDelegate(testDesc, m_activeTestIndex, m_numTests);
            
            auto passDelegate = [=]()
            {
                CS_ASSERT(testDesc.GetTestCaseName() == m_activeTest->GetDesc().GetTestCaseName(), "Received callback for test which is no longer active.");
                CS_ASSERT(testDesc.GetTestName() == m_activeTest->GetDesc().GetTestName(), "Received callback for test which is no longer active.");
                
                TryStartTest();
            };
            
            auto failDelegate = [=](const std::string& in_message)
            {
                CS_ASSERT(testDesc.GetTestCaseName() == m_activeTest->GetDesc().GetTestCaseName(), "Received callback for test which is no longer active.");
                CS_ASSERT(testDesc.GetTestName() == m_activeTest->GetDesc().GetTestName(), "Received callback for test which is no longer active.");
                
                m_failedTests.push_back(Report::FailedTest(testDesc, in_message));
                
                TryStartTest();
            };
            
            m_activeTest = Test::Create(testDesc, passDelegate, failDelegate);
        }
    }
}
