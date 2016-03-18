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
                
                CS_LOG_ERROR(CSCore::ToString(in_report.GetNumFailedTests()) + "/" + CSCore::ToString(in_report.GetNumTests()) + " tests failed!");
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
        bool ReportPresenter::IsA(CSCore::InterfaceIDType in_interfaceId) const
        {
            return (ReportPresenter::InterfaceID == in_interfaceId);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void ReportPresenter::PresentProgress(const TestDesc& in_testDesc, u32 in_testIndex, u32 in_numTests)
        {
            SetText("Running test " + CSCore::ToString(in_testIndex) + " out of " + CSCore::ToString(in_numTests) + ": " + in_testDesc.GetTestName());
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void ReportPresenter::PresentReport(const Report& in_report)
        {
            if (in_report.GetNumTests() == 0)
            {
                SetText(k_noTestsText);
            }
            else if (in_report.GetNumFailedTests() == 0)
            {
                SetText("All " + CSCore::ToString(in_report.GetNumTests()) + " tests passed!");
            }
            else
            {
                std::string textBody = CSCore::ToString(in_report.GetNumFailedTests()) + " tests failed out of " + CSCore::ToString(in_report.GetNumTests()) + ":";
                
                int count = 0;
                for (const auto& failedTest : in_report.GetFailedTests())
                {
                    ++count;
                    
                    if (count > k_maxTestsToDisplay)
                    {
                        textBody += "\n...";
                        break;
                    }
                    else
                    {
                        textBody += "\n" + failedTest.GetDesc().GetTestName();
                    }
                }
                
                textBody += "\n \nPlease check the console for further information.";
                
                SetText(textBody);
                
                PrintDetailedReport(in_report);
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
            auto resourcePool = CSCore::Application::Get()->GetResourcePool();
            auto mediumFont = resourcePool->LoadResource<CSRendering::Font>(CSCore::StorageLocation::k_package, "Fonts/ArialMed.csfont");

            auto basicWidgetFactory = CSCore::Application::Get()->GetSystem<Common::BasicWidgetFactory>();
            m_text = basicWidgetFactory->CreateLabel(CSCore::Vector2(0.9f, 1.0f), mediumFont, "");
            m_text->SetRelativePosition(CSCore::Vector2(0.0f, 0.05f));
            
            GetState()->GetUICanvas()->AddWidget(m_text);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void ReportPresenter::OnDestroy()
        {
            m_text->RemoveFromParent();
            m_text.reset();
        }
    }
}
