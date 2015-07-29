//
//  SmokeTestSet.cpp
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

#include <Common/Core/SmokeTestSet.h>

namespace CSTest
{
    namespace Common
    {
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        SmokeTestSet::SmokeTestSet(const std::string& in_setName)
            : m_name(in_setName)
        {
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void SmokeTestSet::AddTest(const std::string& in_testName, const std::function<void()>& in_action)
        {
            Test test;
            test.m_name = in_testName;
            test.m_action = in_action;
            
            m_tests.push_back(test);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        const std::string& SmokeTestSet::GetName() const
        {
            return m_name;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        const std::vector<SmokeTestSet::Test>& SmokeTestSet::GetTests() const
        {
            return m_tests;
        }
    }
}
