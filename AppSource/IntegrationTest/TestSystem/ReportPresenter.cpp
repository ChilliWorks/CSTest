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
            const char k_defaultText[] = "Running tests...";
            const char k_successText[] = "All tests successful!";
            const char k_failureText[] = "Tests failed!";
            const u32 k_maxTestCasesToDisplay = 3;
            
            //------------------------------------------------------------------------------
            /// Creates a new text widget with the given properties.
            ///
            /// @author Ian Copland
            ///
            /// @param in_position - The relative position of the text widget.
            /// @param in_size - The relative size of the text widget.
            /// @param in_font - The font the text will be rendered with.
            /// @param in_alignment - [Optional] The alignment of the text. Defaults to
            /// middle centre.
            /// @param iin_horizontalTextJustification - [Optional] The horizontal
            /// justification of the text. Defaults to centre.
            /// @param in_verticalTextJustification - [Optional] The vertical justification
            /// of the text. Defaults to centre.
            //------------------------------------------------------------------------------
            CSUI::WidgetUPtr CreateTextWidget(const CSCore::Vector2& in_position, const CSCore::Vector2& in_size, const CSRendering::FontCSPtr& in_font,
                                              CSRendering::AlignmentAnchor in_alignment = CSRendering::AlignmentAnchor::k_middleCentre,
                                              CSRendering::HorizontalTextJustification in_horizontalTextJustification = CSRendering::HorizontalTextJustification::k_centre,
                                              CSRendering::VerticalTextJustification in_verticalTextJustification = CSRendering::VerticalTextJustification::k_centre)
            {
                auto widgetFactory = CSCore::Application::Get()->GetWidgetFactory();
                
                auto widget = widgetFactory->CreateLabel();
                widget->SetRelativePosition(in_position);
                widget->SetRelativeSize(in_size);
                widget->SetParentalAnchor(in_alignment);
                widget->SetOriginAnchor(in_alignment);
                
                auto textComponent = widget->GetComponent<CSUI::TextComponent>();
                textComponent->SetFont(in_font);
                textComponent->SetTextColour(CSCore::Colour::k_black);
                textComponent->SetHorizontalJustification(in_horizontalTextJustification);
                textComponent->SetVerticalJustification(in_verticalTextJustification);
                
                return widget;
            }
            //------------------------------------------------------------------------------
            /// Prints details on a failed test report to console.
            ///
            /// @author Ian Copland
            ///
            /// @param in_report - The report. Must be failed report.
            //------------------------------------------------------------------------------
            void PrintDetailedReport(const Report& in_report)
            {
                CS_ASSERT(in_report.DidAllTestsPass() == false, "Must be failed report");
                
                CS_LOG_ERROR("==========================================");
                
                CS_LOG_ERROR("Tests Failed!");
                CS_LOG_ERROR(" ");
                CS_LOG_ERROR(CSCore::ToString(in_report.GetNumFailedTestCases()) + " test cases failed out of " + CSCore::ToString(in_report.GetNumTestCases()));
                CS_LOG_ERROR(CSCore::ToString(in_report.GetNumFailedAssertions()) + " assertions failed out of " + CSCore::ToString(in_report.GetNumAssertions()));
                
                for (const auto& testCase : in_report.GetFailedTestCases())
                {
                    CS_LOG_ERROR(" ");
                    CS_LOG_ERROR("'" + testCase.GetName() + "' failed " + CSCore::ToString(testCase.GetNumFailedAssertions()) + " out of " + CSCore::ToString(testCase.GetNumAssertions()) + " assertions: ");
                    
                    for (const auto& assertion : testCase.GetFailedAssertions())
                    {
                        CS_LOG_ERROR("  " + assertion.GetFilePath() + ": " + CSCore::ToString(assertion.GetLine()));
                        CS_LOG_ERROR("    " + assertion.GetErrorMessage());
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
            if (in_report.DidAllTestsPass() == true)
            {
                std::string textBody = CSCore::ToString(in_report.GetNumTestCases()) + " test cases successful.";
                textBody += "\n" + CSCore::ToString(in_report.GetNumAssertions()) + " assertions successful.";
                
                SetText(k_successText, textBody);
            }
            else
            {
                std::string textBody = CSCore::ToString(in_report.GetNumFailedTestCases()) + " test cases failed out of " + CSCore::ToString(in_report.GetNumTestCases());
                textBody += "\n" + CSCore::ToString(in_report.GetNumFailedAssertions()) + " assertions failed out of " + CSCore::ToString(in_report.GetNumAssertions());
                
                textBody += "\n \nThe following tests cases failed:";
                
                int count = 0;
                for (const auto& testCase : in_report.GetFailedTestCases())
                {
                    textBody += "\n - " + testCase.GetName();
                    
                    if (++count >= k_maxTestCasesToDisplay)
                    {
                        textBody += "\n - ...";
                        break;
                    }
                }
                
                textBody += "\n \nPlease check the console for further information.";
                
                SetText(k_failureText, textBody);
                
                PrintDetailedReport(in_report);
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void ReportPresenter::SetText(const std::string& in_text)
        {
            CS_ASSERT(m_centreText, "Cannot set the text before the text widgets are created.");
            CS_ASSERT(m_headerText, "Cannot set the text before the text widgets are created.");
            CS_ASSERT(m_bodyText, "Cannot set the text before the text widgets are created.");
            
            auto textComponent = m_centreText->GetComponent<CSUI::TextComponent>();
            textComponent->SetText(in_text);
            
            textComponent = m_headerText->GetComponent<CSUI::TextComponent>();
            textComponent->SetText("");
            
            textComponent = m_bodyText->GetComponent<CSUI::TextComponent>();
            textComponent->SetText("");
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void ReportPresenter::SetText(const std::string& in_header, const std::string& in_body)
        {
            CS_ASSERT(m_centreText, "Cannot set the text before the text widgets are created.");
            CS_ASSERT(m_headerText, "Cannot set the text before the text widgets are created.");
            CS_ASSERT(m_bodyText, "Cannot set the text before the text widgets are created.");
            
            auto textComponent = m_centreText->GetComponent<CSUI::TextComponent>();
            textComponent->SetText("");
            
            textComponent = m_headerText->GetComponent<CSUI::TextComponent>();
            textComponent->SetText(in_header);
            
            textComponent = m_bodyText->GetComponent<CSUI::TextComponent>();
            textComponent->SetText(in_body);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void ReportPresenter::OnInit()
        {
            auto resourcePool = CSCore::Application::Get()->GetResourcePool();
            auto smallFont = resourcePool->LoadResource<CSRendering::Font>(CSCore::StorageLocation::k_package, "Fonts/ArialSmall.csfont");
            auto mediumFont = resourcePool->LoadResource<CSRendering::Font>(CSCore::StorageLocation::k_package, "Fonts/ArialMed.csfont");

            m_centreText = CreateTextWidget(CSCore::Vector2::k_zero, CSCore::Vector2(0.9f, 1.0f), mediumFont);
            m_headerText = CreateTextWidget(CSCore::Vector2::k_zero, CSCore::Vector2(0.9f, 0.15f), mediumFont, CSRendering::AlignmentAnchor::k_topCentre);
            m_bodyText = CreateTextWidget(CSCore::Vector2(0.0f, -0.15f), CSCore::Vector2(0.9f, 0.65f), smallFont, CSRendering::AlignmentAnchor::k_topCentre, CSRendering::HorizontalTextJustification::k_left,
                                            CSRendering::VerticalTextJustification::k_top);

            auto widgetFactory = CSCore::Application::Get()->GetWidgetFactory();
            m_presentationUI = widgetFactory->CreateWidget();
            m_presentationUI->AddWidget(m_centreText);
            m_presentationUI->AddWidget(m_headerText);
            m_presentationUI->AddWidget(m_bodyText);
            
            GetState()->GetUICanvas()->AddWidget(m_presentationUI);
            
            SetText(k_defaultText);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void ReportPresenter::OnDestroy()
        {
            m_centreText.reset();
            m_headerText.reset();
            m_bodyText.reset();
            
            m_presentationUI->RemoveFromParent();
            m_presentationUI.reset();
        }
    }
}
