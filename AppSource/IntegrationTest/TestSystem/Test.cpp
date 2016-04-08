//
//  Test.cpp
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

#include <IntegrationTest/TestSystem/Test.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Threading.h>
#include <ChilliSource/Core/Time.h>

namespace CSTest
{
    namespace IntegrationTest
    {
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        TestSPtr Test::Create(const TestDesc& in_desc, const PassDelegate& in_passDelegate, const FailDelegate& in_failDelegate)
        {
            TestSPtr test(new Test(in_desc, in_passDelegate, in_failDelegate));
            
            // This is called here rather than inside the constructor to ensure the shared pointer exists.
            test->GetDesc().GetTestDelegate()(test);
            
            return test;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        Test::Test(const TestDesc& in_desc, const PassDelegate& in_passDelegate, const FailDelegate& in_failDelegate) noexcept
            : m_desc(in_desc), m_passDelegate(in_passDelegate), m_failDelegate(in_failDelegate)
        {
            CS_ASSERT(m_passDelegate, "A valid pass delegate must be supplied.");
            CS_ASSERT(m_failDelegate, "A valid fail delegate must be supplied.");
            
            m_taskScheduler = CSCore::Application::Get()->GetTaskScheduler();
            
            m_timerEventConnection = m_timer.OpenConnection(in_desc.GetTimeoutSeconds(), [=]()
            {
                std::unique_lock<std::mutex> lock(m_mutex);
                
                if (m_active)
                {
                    m_active = false;
                    m_timer.Stop();
                    m_timerEventConnection.reset();
                    
                    m_failDelegate("Timed out.");
                }
            });
            
            m_timer.Start();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        const TestDesc& Test::GetDesc() const noexcept
        {
            return m_desc;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void Test::Pass() noexcept
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            
            if (m_active)
            {
                m_active = false;
                
                m_taskScheduler->ScheduleTask(CSCore::TaskType::k_mainThread, [=](const CSCore::TaskContext&) noexcept
                {
                    m_timer.Stop();
                    m_timerEventConnection.reset();
                    
                    m_passDelegate();
                });
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void Test::Fail(const std::string& in_message) noexcept
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            
            if (m_active)
            {
                m_active = false;
                
                m_taskScheduler->ScheduleTask(CSCore::TaskType::k_mainThread, [=](const CSCore::TaskContext&) noexcept
                {
                    m_timer.Stop();
                    m_timerEventConnection.reset();
                    
                    m_failDelegate(in_message);
                });
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void Test::Assert(bool in_condition, const std::string& in_failureMessage) noexcept
        {
            if (!in_condition)
            {
                Fail(in_failureMessage);
            }
        }
    }
}
