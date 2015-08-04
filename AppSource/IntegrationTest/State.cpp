//
//  State.cpp
//  CSTest
//  Created by Ian Copland on 13/07/2015.
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

#include <IntegrationTest/State.h>

#include <Common/Core/StateNavigator.h>
#include <IntegrationTest/TestSystem/ReportPresenter.h>
#include <IntegrationTest/TestSystem/TestSystem.h>
#include <Lighting/State.h>

#include <ChilliSource/Core/Scene.h>

namespace CSTest
{
    namespace IntegrationTest
    {
        namespace
        {
            using NextState = Lighting::State;
            
            const f32 k_timeBeforeTests = 0.5f;
        }
        
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::CreateSystems()
        {
            m_testSystem = CreateSystem<TestSystem>();
            m_reportPresenter = CreateSystem<ReportPresenter>();
            CreateSystem<Common::StateNavigator<NextState>>();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::OnInit()
        {
            GetScene()->SetClearColour(CSCore::Colour(0.9f, 0.9f, 0.9f, 1.0f));
            
            GetSystem<Common::StateNavigator<NextState>>()->SetNextButtonVisible(false);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::OnUpdate(f32 in_deltaTime)
        {
            if (m_testsPerformed == false)
            {
                m_timer += in_deltaTime;
                
                if (m_timer > k_timeBeforeTests)
                {
                    m_timer = 0.0f;
                    m_testsPerformed = true;
                    
                    auto report = m_testSystem->PerformTests();
                    m_reportPresenter->PresentReport(report);
                    GetSystem<Common::StateNavigator<NextState>>()->SetNextButtonVisible(true);
                }
            }
        }
    }
}
