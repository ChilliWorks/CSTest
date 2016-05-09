//
//  State.cpp
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

#include <IntegrationTest/State.h>

#include <Common/Core/TestNavigator.h>
#include <IntegrationTest/TestSystem/ReportPresenter.h>
#include <IntegrationTest/TestSystem/Tester.h>
#include <Lighting/State.h>

#include <ChilliSource/Core/Scene.h>

namespace CSTest
{
    namespace IntegrationTest
    {
        namespace
        {
            using NextState = Lighting::State;
        }
        
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::CreateSystems()
        {
            m_testNavigator = CreateSystem<Common::TestNavigator>("Integration Tests");
            m_reportPresenter = CreateSystem<ReportPresenter>();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::OnInit()
        {
            GetScene()->SetClearColour(CS::Colour(0.9f, 0.9f, 0.9f, 1.0f));
            
			m_testNavigator->SetBackButtonVisible(false);
            
            auto progressUpdateDelegate = [=](const TestDesc& in_testDesc, u32 in_testIndex, u32 in_numTests)
            {
                m_reportPresenter->PresentProgress(in_testDesc, in_testIndex, in_numTests);
            };
            
            auto completionDelegate = [=](const Report& in_report)
            {
                m_reportPresenter->PresentReport(in_report);
				m_testNavigator->SetBackButtonVisible(true);
            };
            
            m_tester = TesterUPtr(new Tester(progressUpdateDelegate, completionDelegate));
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::OnDestroy()
        {
            m_tester.reset();
        }
    }
}
