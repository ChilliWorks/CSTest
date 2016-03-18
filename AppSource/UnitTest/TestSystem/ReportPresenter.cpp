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

#include <UnitTest/TestSystem/ReportPresenter.h>

#include <Common/UI/BasicWidgetFactory.h>
#include <UnitTest/TestSystem/CSReporter.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Core/State.h>
#include <ChilliSource/Rendering/Font.h>
#include <ChilliSource/UI/Base.h>
#include <ChilliSource/UI/Text.h>

namespace CSTest
{
    namespace UnitTest
    {
        namespace
        {
            constexpr char k_defaultText[] = "Running tests...";
            constexpr char k_noTestsText[] = "No tests to run.";
            constexpr u32 k_maxTestCasesToDisplay = 3;
            
            //------------------------------------------------------------------------------
            /// Prints details on a failed test report to console.
            ///
            /// @author Ian Copland
            ///
            /// @param in_report - The report. Must be failed report.
            //------------------------------------------------------------------------------
            void PrintDetailedReport(const Report& in_report)
            {
                CS_ASSERT(in_report.GetNumFailedTestCases()  > 0, "Must be failed report");
                
                CS_LOG_ERROR("==========================================");
                CS_LOG_ERROR("Unit tests failed!");
                CS_LOG_ERROR(" ");
                
                CS_LOG_ERROR(CSCore::ToString(in_report.GetNumFailedTestCases()) + " test cases failed out of " + CSCore::ToString(in_report.GetNumTestCases()));
                CS_LOG_ERROR(CSCore::ToString(in_report.GetNumFailedSections()) + " test sections failed out of " + CSCore::ToString(in_report.GetNumSections()));
                CS_LOG_ERROR(CSCore::ToString(in_report.GetNumFailedAssertions()) + " test assertions failed out of " + CSCore::ToString(in_report.GetNumAssertions()));
                
                for (const auto& testCase : in_report.GetFailedTestCases())
                {
                    CS_LOG_ERROR(" ");
                    CS_LOG_ERROR("** Test case '" + testCase.GetName() + "' **");
                    
                    for (const auto& section : testCase.GetFailedSections())
                    {
                        CS_LOG_ERROR("Section '" + section.GetName() + "' failed " + CSCore::ToString(section.GetNumFailedAssertions()) + " out of " + CSCore::ToString(section.GetNumAssertions()) + " assertions: ");
                        
                        for (const auto& assertion : section.GetFailedAssertions())
                        {
                            std::string baseName, directoryPath;
                            CSCore::StringUtils::SplitFilename(assertion.GetFilePath(), baseName, directoryPath);
                            
                            CS_LOG_ERROR("  " + baseName + ", ln " + CSCore::ToString(assertion.GetLine()) + ": " + assertion.GetErrorMessage());
                        }
                    }
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
        void ReportPresenter::PresentReport(const Report& in_report)
        {
            if (in_report.GetNumTestCases() == 0)
            {
                SetCentreText(k_noTestsText);
            }
            else if (in_report.GetNumFailedTestCases() == 0)
            {
                SetCentreText("All " + CSCore::ToString(in_report.GetNumSections()) + " test sections passed!");
            }
            else
            {
                std::string textBody = CSCore::ToString(in_report.GetNumFailedTestCases()) + " test cases failed out of " + CSCore::ToString(in_report.GetNumTestCases());
                textBody += "\n" + CSCore::ToString(in_report.GetNumFailedSections()) + " test sections failed out of " + CSCore::ToString(in_report.GetNumSections());
                textBody += "\n" + CSCore::ToString(in_report.GetNumFailedAssertions()) + " test assertions failed out of " + CSCore::ToString(in_report.GetNumAssertions());
                
                textBody += "\n \nThe following test cases failed:";
                
                int count = 0;
                for (const auto& testCase : in_report.GetFailedTestCases())
                {
                    ++count;
                    
                    if (count > k_maxTestCasesToDisplay)
                    {
                        textBody += "\n - ...";
                        break;
                    }
                    else
                    {
                        textBody += "\n - " + testCase.GetName();
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
            
            auto textComponent = m_centreText->GetComponent<CSUI::TextComponent>();
            textComponent->SetText(in_text);
            
            textComponent = m_bodyText->GetComponent<CSUI::TextComponent>();
            textComponent->SetText("");
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void ReportPresenter::SetBodyText(const std::string& in_text)
        {
            CS_ASSERT(m_centreText, "Cannot set the text before the text widgets are created.");
            CS_ASSERT(m_bodyText, "Cannot set the text before the text widgets are created.");
            
            auto textComponent = m_centreText->GetComponent<CSUI::TextComponent>();
            textComponent->SetText("");
            
            textComponent = m_bodyText->GetComponent<CSUI::TextComponent>();
            textComponent->SetText(in_text);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void ReportPresenter::OnInit()
        {
            auto resourcePool = CSCore::Application::Get()->GetResourcePool();
            auto smallFont = resourcePool->LoadResource<CSRendering::Font>(CSCore::StorageLocation::k_package, "Fonts/ArialSmall.csfont");
            auto mediumFont = resourcePool->LoadResource<CSRendering::Font>(CSCore::StorageLocation::k_package, "Fonts/ArialMed.csfont");

            auto basicWidgetFactory = CSCore::Application::Get()->GetSystem<Common::BasicWidgetFactory>();
            m_centreText = basicWidgetFactory->CreateLabel(CSCore::Vector2(0.9f, 1.0f), mediumFont, "");
            m_centreText->SetRelativePosition(CSCore::Vector2(0.0f, 0.05f));
            
            m_bodyText = basicWidgetFactory->CreateLabel(CSCore::Vector2(0.9f, 0.65f), smallFont, "", CSRendering::AlignmentAnchor::k_topCentre, CSRendering::HorizontalTextJustification::k_left, CSRendering::VerticalTextJustification::k_top);
            m_bodyText->SetRelativePosition(CSCore::Vector2(0.0f, -0.15f));

            auto widgetFactory = CSCore::Application::Get()->GetWidgetFactory();
            m_presentationUI = widgetFactory->CreateWidget();
            m_presentationUI->AddWidget(m_centreText);
            m_presentationUI->AddWidget(m_bodyText);
            
            GetState()->GetUICanvas()->AddWidget(m_presentationUI);
            
            SetCentreText(k_defaultText);
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
