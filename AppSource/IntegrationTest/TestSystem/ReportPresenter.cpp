//
//  ReportPresenter.cpp
//  CSTest
//  Created by Ian Copland on 16/03/2016.
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

#include <IntegrationTest/TestSystem/ReportPresenter.h>

#include <Common/UI/BasicWidgetFactory.h>
#include <IntegrationTest/TestSystem/Report.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Core/State.h>
#include <ChilliSource/Rendering/Font.h>
#include <ChilliSource/UI/Base.h>
#include <ChilliSource/UI/Text.h>

namespace CSTest
{
    namespace IntegrationTest
    {
        namespace
        {
            constexpr char k_noTestsText[] = "No tests to run.";
            constexpr u32 k_maxTestsToDisplay = 3;
            
            //------------------------------------------------------------------------------
            /// Prints details on a failed test report to console.
            ///
            /// @author Ian Copland
            ///
            /// @param in_report - The report. Must be failed report.
            //------------------------------------------------------------------------------
            void PrintDetailedReport(const Report& in_report)
            {
                CS_ASSERT(in_report.GetNumFailedTests() > 0, "Must be failed report");
                
                CS_LOG_ERROR("==========================================");
                
                CS_LOG_ERROR(CS::ToString(in_report.GetNumFailedTests()) + "/" + CS::ToString(in_report.GetNumTests()) + " integration tests failed!");
                CS_LOG_ERROR(" ");
                
                for (const auto& failedTest : in_report.GetFailedTests())
                {
                    CS_LOG_ERROR("[" + failedTest.GetDesc().GetTestCaseName() + "] " + failedTest.GetDesc().GetTestName() + ": " + failedTest.GetErrorMessage());
                }
                
                CS_LOG_ERROR("==========================================");
            }
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
        bool ReportPresenter::IsA(CS::InterfaceIDType in_interfaceId) const
        {
            return (ReportPresenter::InterfaceID == in_interfaceId);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void ReportPresenter::PresentProgress(const TestDesc& in_testDesc, u32 in_testIndex, u32 in_numTests)
        {
            SetCentreText("Running test " + CS::ToString(in_testIndex) + " out of " + CS::ToString(in_numTests) + "\n" + in_testDesc.GetTestName());
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void ReportPresenter::PresentReport(const Report& in_report)
        {
            if (in_report.GetNumTests() == 0)
            {
                SetCentreText(k_noTestsText);
            }
            else if (in_report.GetNumFailedTests() == 0)
            {
                SetCentreText("All " + CS::ToString(in_report.GetNumTests()) + " tests passed!");
            }
            else
            {
                std::string textBody = CS::ToString(in_report.GetNumFailedTests()) + " tests failed out of " + CS::ToString(in_report.GetNumTests()) + ":";
                
                int count = 0;
                for (const auto& failedTest : in_report.GetFailedTests())
                {
                    ++count;
                    
                    if (count > k_maxTestsToDisplay)
                    {
                        textBody += "\n - ...";
                        break;
                    }
                    else
                    {
                        textBody += "\n - " + failedTest.GetDesc().GetTestCaseName() + ": " + failedTest.GetDesc().GetTestName();
                    }
                }
                
                textBody += "\n \nPlease check the console for further information.";
                
                SetBodyText(textBody);
                
                PrintDetailedReport(in_report);
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void ReportPresenter::SetCentreText(const std::string& in_text)
        {
            CS_ASSERT(m_centreText, "Cannot set the text before the text widgets are created.");
            CS_ASSERT(m_bodyText, "Cannot set the text before the text widgets are created.");
            
            auto textComponent = m_centreText->GetComponent<CS::TextUIComponent>();
            textComponent->SetText(in_text);
            
            textComponent = m_bodyText->GetComponent<CS::TextUIComponent>();
            textComponent->SetText("");
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void ReportPresenter::SetBodyText(const std::string& in_text)
        {
            CS_ASSERT(m_centreText, "Cannot set the text before the text widgets are created.");
            CS_ASSERT(m_bodyText, "Cannot set the text before the text widgets are created.");
            
            auto textComponent = m_centreText->GetComponent<CS::TextUIComponent>();
            textComponent->SetText("");
            
            textComponent = m_bodyText->GetComponent<CS::TextUIComponent>();
            textComponent->SetText(in_text);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void ReportPresenter::OnInit()
        {
            auto resourcePool = CS::Application::Get()->GetResourcePool();
            auto smallFont = resourcePool->LoadResource<CS::Font>(CS::StorageLocation::k_package, "Fonts/ArialSmall.csfont");
            auto mediumFont = resourcePool->LoadResource<CS::Font>(CS::StorageLocation::k_package, "Fonts/ArialMed.csfont");
            
            auto basicWidgetFactory = CS::Application::Get()->GetSystem<Common::BasicWidgetFactory>();
            m_centreText = basicWidgetFactory->CreateLabel(CS::Vector2(0.9f, 1.0f), mediumFont, "");
            m_centreText->SetRelativePosition(CS::Vector2(0.0f, 0.05f));
            
            m_bodyText = basicWidgetFactory->CreateLabel(CS::Vector2(0.9f, 0.65f), smallFont, "", CS::AlignmentAnchor::k_topCentre, CS::HorizontalTextJustification::k_left, CS::VerticalTextJustification::k_top);
            m_bodyText->SetRelativePosition(CS::Vector2(0.0f, -0.15f));
            
            auto widgetFactory = CS::Application::Get()->GetWidgetFactory();
            m_presentationUI = widgetFactory->CreateWidget();
            m_presentationUI->AddWidget(m_centreText);
            m_presentationUI->AddWidget(m_bodyText);
            
            GetState()->GetUICanvas()->AddWidget(m_presentationUI);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void ReportPresenter::OnDestroy()
        {
            m_centreText.reset();
            m_bodyText.reset();
            
            m_presentationUI->RemoveFromParent();
            m_presentationUI.reset();
        }
    }
}
