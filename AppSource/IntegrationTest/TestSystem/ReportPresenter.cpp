//
//  ReportPresenter.cpp
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

#include <IntegrationTest/TestSystem/ReportPresenter.h>

#include <IntegrationTest/TestSystem/CSReporter.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/State.h>
#include <ChilliSource/UI/Base.h>
#include <ChilliSource/UI/Text.h>

namespace CSTest
{
    namespace IntegrationTest
    {
        namespace
        {
            const char k_defaultText[] = "Running tests...";
            const char k_successText[] = "All tests succeeded!";
            const u32 k_maxTestCasesToDisplay = 4;
        }
        
        CS_DEFINE_NAMEDTYPE(ReportPresenter);
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        ReportPresenterUPtr ReportPresenter::Create()
        {
            return ReportPresenterUPtr(new ReportPresenter());
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool ReportPresenter::IsA(CSCore::InterfaceIDType in_interfaceId) const
        {
            return (ReportPresenter::InterfaceID == in_interfaceId);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void ReportPresenter::PresentReport(const Report& in_report)
        {
            if (in_report.DidAllTestsPass() == true)
            {
                SetText(k_successText);
            }
            else
            {
                std::string failureText = "Tests failed!";
                failureText += "\n\n" + CSCore::ToString(in_report.GetNumFailedTestCases()) + " test cases failed out of " + CSCore::ToString(in_report.GetNumTestCases());
                failureText += "\n" + CSCore::ToString(in_report.GetNumFailedAssertions()) + " assertions failed out of " + CSCore::ToString(in_report.GetNumAssertions());
                
                failureText += "\n\nThe following tests cases failed:";
                
                int count = 0;
                for (auto testCase : in_report.GetFailedTestCases())
                {
                    failureText += "\n" + testCase.GetName();
                    
                    if (count++ >= k_maxTestCasesToDisplay)
                    {
                        failureText += "\n...";
                    }
                }
                
                SetText(failureText);
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void ReportPresenter::SetText(const std::string& in_text)
        {
            CS_ASSERT(m_text, "Cannot set the text before the text widget is created.");
            
            auto textComponent = m_text->GetComponent<CSUI::TextComponent>();
            textComponent->SetText(in_text);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void ReportPresenter::OnInit()
        {
            auto widgetFactory = CSCore::Application::Get()->GetWidgetFactory();
            
            m_text = widgetFactory->CreateLabel();
            m_text->SetAbsoluteSize(CSCore::Vector2(0.8f, 0.8f));
            
            auto textComponent = m_text->GetComponent<CSUI::TextComponent>();
            textComponent->SetTextColour(CSCore::Colour::k_black);
            textComponent->SetVerticalJustification(CSRendering::VerticalTextJustification::k_centre);
            textComponent->SetHorizontalJustification(CSRendering::HorizontalTextJustification::k_centre);
            
            m_presentationUI = widgetFactory->CreateWidget();
            m_presentationUI->AddWidget(m_text);
            
            GetState()->GetUICanvas()->AddWidget(m_presentationUI);
            
            SetText(k_defaultText);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void ReportPresenter::OnDestroy()
        {
            m_text.reset();
            m_presentationUI->RemoveFromParent();
            m_presentationUI.reset();
        }
    }
}
