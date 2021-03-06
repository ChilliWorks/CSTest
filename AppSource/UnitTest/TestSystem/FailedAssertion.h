//
//  FailedAssertion.h
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

#ifndef _UNITTEST_TESTSYSTEM_FAILEDASSERTION_H_
#define _UNITTEST_TESTSYSTEM_FAILEDASSERTION_H_

#include <CSTest.h>

namespace CSTest
{
    namespace UnitTest
    {
        //------------------------------------------------------------------------------
        /// A container for all information pertaining to a single failed test assertion.
        ///
        /// This is immutable after construction.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        class FailedAssertion final
        {
        public:
            //------------------------------------------------------------------------------
            /// Constructor. Creates the assertion with failure information.
            ///
            /// @author Ian Copland
            ///
            /// @param in_filePath - The file the failed assertion is contained in.
            /// @param in_line - The line the failed assertion is at within the file.
            /// @param in_errorMessage - The error message associated with the assertion.
            //------------------------------------------------------------------------------
            FailedAssertion(const std::string& in_filePath, u32 in_line, const std::string& in_errorMessage) noexcept;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The file the failed assertion is contained in.
            //------------------------------------------------------------------------------
            const std::string& GetFilePath() const noexcept;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The line the failed assertion is at within the file.
            //------------------------------------------------------------------------------
            u32 GetLine() const noexcept;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The error message associated with the assertion.
            //------------------------------------------------------------------------------
            const std::string& GetErrorMessage() const noexcept;
            
        private:
            std::string m_filePath;
            u32 m_line;
            std::string m_errorMessage;
        };
    }
}

#endif
