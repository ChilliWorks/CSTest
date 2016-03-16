//
//  FailedAssertion.cpp
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

#include <UnitTest/TestSystem/FailedAssertion.h>

namespace CSTest
{
    namespace UnitTest
    {
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        FailedAssertion::FailedAssertion(const std::string& in_filePath, u32 in_line, const std::string& in_errorMessage)
            : m_filePath(in_filePath), m_line(in_line), m_errorMessage(in_errorMessage)
        {
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        const std::string& FailedAssertion::GetFilePath() const
        {
            return m_filePath;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        u32 FailedAssertion::GetLine() const
        {
            return m_line;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        const std::string& FailedAssertion::GetErrorMessage() const
        {
            return m_errorMessage;
        }
    }
}
