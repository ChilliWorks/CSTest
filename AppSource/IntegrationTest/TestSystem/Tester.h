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
        /// TODO
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
            /// TODO
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            using ProgressUpdateDelegate = std::function<void(const TestDesc& in_testDesc, u32 in_testIndex, u32 in_numTests) noexcept>;
            //------------------------------------------------------------------------------
            /// TODO
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            using CompletionDelegate = std::function<void(const Report& in_report) noexcept>;
            //------------------------------------------------------------------------------
            /// TODO
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            Tester() = default;
            //------------------------------------------------------------------------------
            /// TODO
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            Tester(const std::vector<TestDesc>& in_tests, const ProgressUpdateDelegate& in_progressUpdateDelegate, const CompletionDelegate& in_completionDelegate) noexcept;
        private:
            //------------------------------------------------------------------------------
            /// TODO
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void TryStartTest() noexcept;
            
            const ProgressUpdateDelegate m_progressUpdateDelegate;
            const CompletionDelegate m_completionDelegate;
            const u32 m_numTests;
            
            std::queue<TestDesc> m_testQueue;
            TestSPtr m_activeTest;
            u32 m_activeTestIndex = 0;
            std::vector<Report::FailedTest> m_failedTests;
        };
    }
}

#endif
