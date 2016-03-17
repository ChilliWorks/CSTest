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
        /// TODO
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
            /// TODO
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            using PassDelegate = std::function<void() noexcept>;
            //------------------------------------------------------------------------------
            /// TODO
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            using FailDelegate = std::function<void(const std::string& in_message) noexcept>;
            //------------------------------------------------------------------------------
            /// TODO
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            static TestSPtr Create(const TestDesc& in_desc, const PassDelegate& in_passDelegate, const FailDelegate& in_failDelegate);
            //------------------------------------------------------------------------------
            /// TODO
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            const TestDesc& GetDesc() const noexcept;
            //------------------------------------------------------------------------------
            /// TODO
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void Pass() noexcept;
            //------------------------------------------------------------------------------
            /// TODO
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void Fail(const std::string& in_message) noexcept;
            //------------------------------------------------------------------------------
            /// TODO
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void Assert(bool in_condition, const std::string& in_failureMessage = "") noexcept;
            
        private:
            //------------------------------------------------------------------------------
            /// TODO
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            Test(const TestDesc& in_desc, const PassDelegate& in_passDelegate, const FailDelegate& in_failDelegate) noexcept;
            
            const TestDesc m_desc;
            const PassDelegate m_passDelegate;
            const FailDelegate m_failDelegate;
            
            CSCore::TaskScheduler* m_taskScheduler = nullptr;
            bool m_active = true;
            std::mutex m_mutex;
            CSCore::Timer m_timer;
            CSCore::EventConnectionUPtr m_timerEventConnection;
        };
    }
}

#endif
