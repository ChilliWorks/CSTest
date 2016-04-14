//
//  AutoRegister.h
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

#ifndef _INTEGRATIONTEST_TESTSYSTEM_AUTOREGISTER_H_
#define _INTEGRATIONTEST_TESTSYSTEM_AUTOREGISTER_H_

#include <CSTest.h>

#include <IntegrationTest/TestSystem/TestRegistry.h>

namespace CSTest
{
    namespace IntegrationTest
    {
        //------------------------------------------------------------------------------
        /// Instances of this class created during static initialisation are used by the
        /// test case macros to register tests with the test registry.
        ///
        /// This is not thread-safe and should only be created on the main thread.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        class AutoRegister final
        {
        public:
            CS_DECLARE_NOCOPY(AutoRegister);
            //------------------------------------------------------------------------------
            /// The default constructor.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            AutoRegister() = default;
            //------------------------------------------------------------------------------
            /// Creates a new instances of this, adding the given test description to the
            /// test registry.
            ///
            /// @author Ian Copland
            ///
            /// @param in_testDesc - The test description.
            //------------------------------------------------------------------------------
            AutoRegister(const TestDesc& in_testDesc) noexcept
            {
                TestRegistry::Get().RegisterTest(in_testDesc);
            }
        };
    }
}

#endif