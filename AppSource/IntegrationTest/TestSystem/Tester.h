//
//  Tester.h
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

#ifndef _INTEGRATIONTEST_TESTSYSTEM_TESTER_H_
#define _INTEGRATIONTEST_TESTSYSTEM_TESTER_H_

#include <CSTest.h>

#include <IntegrationTest/TestSystem/Report.h>
#include <IntegrationTest/TestSystem/TestCase.h>

#include <queue>
#include <vector>

namespace CSTest
{
    namespace IntegrationTest
    {
        //------------------------------------------------------------------------------
        /// Provides the means to run the integration tests and receive a report on
        /// how many passed or failed.
        ///
        /// This is not thread-safe.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        class Tester final
        {
        public:
            CS_DECLARE_NOCOPY(Tester);
            //------------------------------------------------------------------------------
            /// A delegate which is called as each test is run which can be used to display
            /// test progress onscreen.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            using ProgressUpdateDelegate = std::function<void(const TestDesc& in_testDesc, u32 in_testIndex, u32 in_numTests) noexcept>;
            //------------------------------------------------------------------------------
            /// A delegate which is called when the tests have completed, providing a report.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            using CompletionDelegate = std::function<void(const Report& in_report) noexcept>;
            //------------------------------------------------------------------------------
            /// Constructs a new instance of the tester and begins the integration tests.
            ///
            /// @author Ian Copland
            ///
            /// @param in_progressUpdateDelegate - The update progress delegate.
            /// @param in_completionDelegate - The completion delegate.
            //------------------------------------------------------------------------------
            Tester(const ProgressUpdateDelegate& in_progressUpdateDelegate, const CompletionDelegate& in_completionDelegate) noexcept;
        private:
            //------------------------------------------------------------------------------
            /// Attempts to start the next test. If there are no tests left to run the the
            /// completion delegate is called.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void TryStartTest() noexcept;
            
            ProgressUpdateDelegate m_progressUpdateDelegate;
            CompletionDelegate m_completionDelegate;
            u32 m_numTests;
            
            std::queue<TestDesc> m_testQueue;
            TestSPtr m_activeTest;
            u32 m_activeTestIndex = 0;
            std::vector<Report::FailedTest> m_failedTests;
        };
    }
}

#endif
