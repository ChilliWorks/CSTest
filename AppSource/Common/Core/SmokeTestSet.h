//
//  SmokeTestSet.h
//  CSTest
//  Created by Ian Copland on 28/07/2015.
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

#ifndef _COMMON_CORE_SMOKETESTSET_H_
#define _COMMON_CORE_SMOKETESTSET_H_

#include <CSTest.h>

#include <functional>
#include <vector>

namespace CSTest
{
    namespace Common
    {
        //------------------------------------------------------------------------------
        /// A container for information which the Smoke Tester will use to present
        /// buttons. This includes the title of the smoke test set, and a name and
        /// action for each of the tests.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        class SmokeTestSet final
        {
        public:
            //------------------------------------------------------------------------------
            /// The delegate called each time the button associated with the test is pressed.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            using Action = std::function<void()>;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param in_setName - The name of the test set.
            //------------------------------------------------------------------------------
            struct Test final
            {
                std::string m_name;
                Action m_action;
            };
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param in_setName - The name of the test set.
            //------------------------------------------------------------------------------
            SmokeTestSet(const std::string& in_setName);
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param in_name - The
            //------------------------------------------------------------------------------
            void AddTest(const std::string& in_testName, const Action& in_action);
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The name of the smoke test set.
            //------------------------------------------------------------------------------
            const std::string& GetName() const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The list of tests within the set.
            //------------------------------------------------------------------------------
            const std::vector<Test>& GetTests() const;
            
        private:
            std::string m_name;
            std::vector<Test> m_tests;
        };
    }
}

#endif
