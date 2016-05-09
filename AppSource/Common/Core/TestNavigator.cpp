//
//  TestNavigator.cpp
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

#include <Common/Core/TestNavigator.h>
#include <Common/UI/BasicWidgetFactory.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Core/State.h>
#include <ChilliSource/Rendering/Font.h>
#include <ChilliSource/UI/Base.h>
#include <ChilliSource/UI/Text.h>

namespace CSTest
{
    namespace Common
    {
        CS_DEFINE_NAMEDTYPE(TestNavigator);
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        TestNavigatorUPtr TestNavigator::Create(const std::string& in_title) noexcept
        {
            return std::unique_ptr<TestNavigator>(new TestNavigator(in_title));
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        TestNavigator::TestNavigator(const std::string& in_title) noexcept
            : m_title(in_title)
        {
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool TestNavigator::IsA(CS::InterfaceIDType in_interfaceId) const noexcept
        {
            return (TestNavigator::InterfaceID == in_interfaceId);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool TestNavigator::IsBackButtonVisible() const noexcept
        {
            CS_ASSERT(m_backButton != nullptr, "Cannot test next button visibility prior to next button being created.");
            
            return m_backButton->IsVisible();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void TestNavigator::SetBackButtonVisible(bool in_visible) noexcept
        {
            CS_ASSERT(m_backButton != nullptr, "Cannot change next button visibility prior to next button being created.");
            CS_ASSERT(m_backButton->IsVisible() != in_visible, "Cannot change to the current visibility.");
            
            m_backButton->SetVisible(in_visible);
            m_backButton->SetInputEnabled(in_visible);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void TestNavigator::OnInit() noexcept
        {
            auto resourcePool = CS::Application::Get()->GetResourcePool();
            auto mediumFont = resourcePool->LoadResource<CS::Font>(CS::StorageLocation::k_package, "Fonts/ArialMed.csfont");
            
            auto basicWidgetFactory = CS::Application::Get()->GetSystem<Common::BasicWidgetFactory>();
            m_titleLabel = basicWidgetFactory->CreateLabel(CS::Vector2(0.9f, 0.15f), mediumFont, m_title, CS::AlignmentAnchor::k_topCentre);

            m_backButton = basicWidgetFactory->CreateButton(CS::Vector2(0.0f, 0.1f), "Back", CS::AlignmentAnchor::k_bottomLeft, CS::Colour::k_cornflowerBlue);
            m_backButton->SetRelativePosition(CS::Vector2(0.05f, 0.05f));
            
            auto widgetFactory = CS::Application::Get()->GetWidgetFactory();
            m_ui = widgetFactory->CreateWidget();
            m_ui->AddWidget(m_titleLabel);
            m_ui->AddWidget(m_backButton);
            
            GetState()->GetUICanvas()->AddWidget(m_ui);
            
            m_backPressedConnection = m_backButton->GetReleasedInsideEvent().OpenConnection([](CS::Widget* in_widget, const CS::Pointer& in_pointer, CS::Pointer::InputType in_inputType)
            {
                CS::Application::Get()->GetStateManager()->Pop();
            });
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void TestNavigator::OnDestroy() noexcept
        {
            m_backPressedConnection.reset();
            m_backButton.reset();
            
            m_ui->RemoveFromParent();
            m_ui.reset();
        }
    }
}
