//
//  SmokeTester.cpp
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

#include <Common/Core/SmokeTester.h>

#include <Common/Core/SmokeTestSet.h>
#include <Common/UI/UIFactory.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Core/State.h>
#include <ChilliSource/Rendering/Font.h>
#include <ChilliSource/UI/Base.h>
#include <ChilliSource/UI/Layout.h>
#include <ChilliSource/UI/Text.h>

namespace CSTest
{
    namespace Common
    {
        namespace
        {
            const f32 k_relativeSpacing = 0.065f;
            const f32 k_listMargins = 0.2f;
        }
        
        CS_DEFINE_NAMEDTYPE(SmokeTester);
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        SmokeTesterUPtr SmokeTester::Create()
        {
            return SmokeTesterUPtr(new SmokeTester());
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool SmokeTester::IsA(CSCore::InterfaceIDType in_interfaceId) const
        {
            return (SmokeTester::InterfaceID == in_interfaceId);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void SmokeTester::Present(const SmokeTestSet& in_testSet)
        {
            CS_ASSERT(in_testSet.GetTests().size() <= 10, "Too many tests");
            
            Dismiss();
            
            m_titleText->GetComponent<CSUI::TextComponent>()->SetText(in_testSet.GetName());
            
			if (in_testSet.GetTests().size() < 4)
			{
				auto layoutComponent = m_buttonContainer->GetComponent<CSUI::LayoutComponent>();
				layoutComponent->ApplyLayoutDef(CSUI::LayoutDefSPtr(new CSUI::VListLayoutDef(5, CSCore::Vector4(0.0f, k_listMargins, 0.0f, k_listMargins), CSCore::Vector4::k_zero, k_relativeSpacing, 0.0f)));
			}
			else
			{
				auto layoutComponent = m_buttonContainer->GetComponent<CSUI::LayoutComponent>();
				layoutComponent->ApplyLayoutDef(CSUI::LayoutDefSPtr(new CSUI::GridLayoutDef(CSUI::GridLayout::CellOrder::k_rowMajor, 5, 2, CSCore::Vector4::k_zero, CSCore::Vector4::k_zero, k_relativeSpacing, 0.0f, k_relativeSpacing, 0.0f)));
			}

            for (const auto& test : in_testSet.GetTests())
            {
                CSUI::WidgetSPtr button = UIFactory::CreateStretchableButton(CSCore::Vector2::k_one, test.m_name, CSRendering::AlignmentAnchor::k_middleCentre, CSCore::Colour(0.92f, 0.95f, 0.98f, 1.0f));
                m_buttonContainer->AddWidget(button);
                
                auto connection = button->GetReleasedInsideEvent().OpenConnection([=](CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer, CSInput::Pointer::InputType in_inputType)
                {
                    test.m_action();
                });
                
                m_buttons.push_back(button);
                m_connectionContainer.push_back(std::move(connection));
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void SmokeTester::Dismiss()
        {
            m_connectionContainer.clear();
            
            for (const auto& button : m_buttons)
            {
                button->RemoveFromParent();
            }
            
            auto layoutComponent = m_buttonContainer->GetComponent<CSUI::LayoutComponent>();
            layoutComponent->ApplyLayoutDef(CSUI::LayoutDefSPtr(new CSUI::VListLayoutDef(5, CSCore::Vector4::k_zero, CSCore::Vector4::k_zero, k_relativeSpacing, 0.0f)));
            
            m_titleText->GetComponent<CSUI::TextComponent>()->SetText("");
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void SmokeTester::OnInit()
        {
            auto resourcePool = CSCore::Application::Get()->GetResourcePool();
            auto mediumFont = resourcePool->LoadResource<CSRendering::Font>(CSCore::StorageLocation::k_package, "Fonts/ArialMed.csfont");
            
            m_titleText = UIFactory::CreateLabel(CSCore::Vector2(0.9f, 0.15f), mediumFont, "", CSRendering::AlignmentAnchor::k_topCentre);
            
            auto widgetFactory = CSCore::Application::Get()->GetWidgetFactory();
            m_buttonContainer = widgetFactory->CreateLayout();
            m_buttonContainer->SetParentalAnchor(CSRendering::AlignmentAnchor::k_topCentre);
            m_buttonContainer->SetOriginAnchor(CSRendering::AlignmentAnchor::k_topCentre);
            m_buttonContainer->SetRelativePosition(CSCore::Vector2(0.0f, -0.15f));
            m_buttonContainer->SetRelativeSize(CSCore::Vector2(0.75f, 0.65f));
            
            m_ui = widgetFactory->CreateWidget();
            m_ui->AddWidget(m_titleText);
            m_ui->AddWidget(m_buttonContainer);
            
            Dismiss();
            
            GetState()->GetUICanvas()->AddWidget(m_ui);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void SmokeTester::OnDestroy()
        {
            Dismiss();
            
            m_titleText.reset();
            m_buttonContainer.reset();
            
            m_ui->RemoveFromParent();
            m_ui.reset();
        }
    }
}
