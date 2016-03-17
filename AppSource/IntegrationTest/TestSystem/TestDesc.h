//
//  TestDesc.h
//  CSTest
//  Created by Ian Copland on 16/03/2016.
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

#ifndef _INTEGRATIONTEST_TESTSYSTEM_TESTDESC_H_
#define _INTEGRATIONTEST_TESTSYSTEM_TESTDESC_H_

#include <CSTest.h>

#include <vector>

namespace CSTest
{
    namespace IntegrationTest
    {
        //------------------------------------------------------------------------------
        /// An immutable description of a test.
        ///
        /// This is immutable and therefore thread-safe.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        class TestDesc final
        {
        public:
            static constexpr f32 k_defaultTimeoutSeconds = 5.0f;
            //------------------------------------------------------------------------------
            /// A delegate describing the test method.
            ///
            /// @author Ian Copland
            ///
            /// @param in_test - The test instance.
            //------------------------------------------------------------------------------
            using TestDelegate = std::function<void(const TestSPtr& in_test) noexcept>;
            //------------------------------------------------------------------------------
            /// The constructor.
            ///
            /// @author Ian Copland
            ///
            /// @param in_testCaseName - The name of the test case.
            /// @param in_testName - The name of the test.
            /// @param in_testDelegate - The test delegate.
            /// @param in_timoutSeconds - [Optional] The time before the test fails.
            //------------------------------------------------------------------------------
            TestDesc(const std::string& in_testCaseName, const std::string& in_testName, const TestDelegate& in_testDelegate, f32 in_timeoutSeconds = k_defaultTimeoutSeconds) noexcept;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The name of the test case.
            //------------------------------------------------------------------------------
            const std::string& GetTestCaseName() const noexcept;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The name of the test.
            //------------------------------------------------------------------------------
            const std::string& GetTestName() const noexcept;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The test delegate.
            //------------------------------------------------------------------------------
            const TestDelegate& GetTestDelegate() const noexcept;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The time before the test will fail.
            //------------------------------------------------------------------------------
            f32 GetTimeoutSeconds() const noexcept;
            
        private:
            std::string m_testCaseName;
            std::string m_testName;
            TestDelegate m_testDelegate;
            f32 m_timeoutSeconds;
        };
    }
}

#endif
