//
//  Test.h
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

#ifndef _INTEGRATIONTEST_TESTSYSTEM_TEST_H_
#define _INTEGRATIONTEST_TESTSYSTEM_TEST_H_

#include <CSTest.h>

#include <IntegrationTest/TestSystem/TestDesc.h>

#include <ChilliSource/Core/Time.h>

#include <mutex>

namespace CSTest
{
    namespace IntegrationTest
    {
        //------------------------------------------------------------------------------
        /// Runs a single integration test which can be multi-threaded. Provides the
        /// means for the test to pass, fail and assert safely from any thread. Tests
        /// which take too long will timeout.
        ///
        /// Integration tests are declared using the macros declared in TestCase.h.
        ///
        /// This is thread-safe, though it should be created on the main thread.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        class Test final
        {
        public:
            CS_DECLARE_NOCOPY(Test);
            //------------------------------------------------------------------------------
            /// A callback which will be called if the test passes.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            using PassDelegate = std::function<void() noexcept>;
            //------------------------------------------------------------------------------
            /// A callback which will be called if the test fails. An error message will be
            /// provided.
            ///
            /// @author Ian Copland
            ///
            /// @param in_message - The error message providing details about why the test
            /// failed.
            //------------------------------------------------------------------------------
            using FailDelegate = std::function<void(const std::string& in_message) noexcept>;
            //------------------------------------------------------------------------------
            /// Creates a new instance of the test with the given description. This factory
            /// method must be used rather than direct allocation.
            ///
            /// @author Ian Copland
            ///
            /// @param in_desc - The test description.
            /// @param in_passDelegate - A delegate which will be called if the test passes.
            /// @param in_failDelegate - A delegate which will be called if the test fails.
            ///
            /// @return The new test instance.
            //------------------------------------------------------------------------------
            static TestSPtr Create(const TestDesc& in_desc, const PassDelegate& in_passDelegate, const FailDelegate& in_failDelegate);
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The test description.
            //------------------------------------------------------------------------------
            const TestDesc& GetDesc() const noexcept;
            //------------------------------------------------------------------------------
            /// This should be called by the test code to signify that the test was
            /// successful.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void Pass() noexcept;
            //------------------------------------------------------------------------------
            /// This should be called by the test code to signify that the test was
            /// unsuccessful, providing a reason as to what went wrong.
            ///
            /// @author Ian Copland
            ///
            /// @param in_message - An error message detailing why the test failed.
            //------------------------------------------------------------------------------
            void Fail(const std::string& in_message) noexcept;
            //------------------------------------------------------------------------------
            /// Test code can use this to assert that the given condition is indeed true.
            /// Failure will call Fail() with the given failure message.
            ///
            /// @author Ian Copland
            ///
            /// @param in_condition - The condition to check.
            /// @param in_failureMessage - The message which will be passed to fail if the
            /// condition is not true.
            //------------------------------------------------------------------------------
            void Assert(bool in_condition, const std::string& in_failureMessage = "") noexcept;
            
        private:
            //------------------------------------------------------------------------------
            /// Creates a new instance of the test. Declared private to force use of the
            /// factory method.
            ///
            /// @author Ian Copland
            ///
            /// @param in_desc - The test description.
            /// @param in_passDelegate - A delegate which will be called if the test passes.
            /// @param in_failDelegate - A delegate which will be called if the test fails.
            //------------------------------------------------------------------------------
            Test(const TestDesc& in_desc, const PassDelegate& in_passDelegate, const FailDelegate& in_failDelegate) noexcept;
            
            const TestDesc m_desc;
            const PassDelegate m_passDelegate;
            const FailDelegate m_failDelegate;
            
            CS::TaskScheduler* m_taskScheduler = nullptr;
            bool m_active = true;
            std::mutex m_mutex;
            CS::Timer m_timer;
            CS::EventConnectionUPtr m_timerEventConnection;
        };
    }
}

#endif
