//
//  StateNavigator.cpp
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

#ifndef _COMMON_CORE_TESTNAVIGATORIMPL_H_
#define _COMMON_CORE_TESTNAVIGATORIMPL_H_

#include <Common/Core/StateNavigator.h>
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
        CS_DEFINE_NAMEDTYPETEMPLATED(StateNavigator, TNextState);
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TNextState> std::unique_ptr<StateNavigator<TNextState>> StateNavigator<TNextState>::Create()
        {
            return std::unique_ptr<StateNavigator<TNextState>>(new StateNavigator<TNextState>());
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TNextState> bool StateNavigator<TNextState>::IsA(CSCore::InterfaceIDType in_interfaceId) const
        {
            return (StateNavigator::InterfaceID == in_interfaceId);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TNextState> bool StateNavigator<TNextState>::IsNextButtonVisible() const
        {
            CS_ASSERT(m_nextButton != nullptr, "Cannot test next button visibility prior to next button being created.");
            
            return m_nextButton->IsVisible();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TNextState> void StateNavigator<TNextState>::SetNextButtonVisible(bool in_visible)
        {
            CS_ASSERT(m_nextButton != nullptr, "Cannot change next button visibility prior to next button being created.");
            CS_ASSERT(m_nextButton->IsVisible() != in_visible, "Cannot change to the current visibility.");
            
            m_nextButton->SetVisible(in_visible);
            m_nextButton->SetInputEnabled(in_visible);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TNextState> void StateNavigator<TNextState>::OnInit()
        {
            auto basicWidgetFactory = CSCore::Application::Get()->GetSystem<Common::BasicWidgetFactory>();
            m_nextButton = basicWidgetFactory->CreateButton(CSCore::Vector2(0.0f, 0.1f), "Next", CSRendering::AlignmentAnchor::k_bottomRight, CSCore::Colour::k_green);
            m_nextButton->SetRelativePosition(CSCore::Vector2(-0.05f, 0.05f));
            
            auto widgetFactory = CSCore::Application::Get()->GetWidgetFactory();
            m_ui = widgetFactory->CreateWidget();
            m_ui->AddWidget(m_nextButton);
            
            GetState()->GetUICanvas()->AddWidget(m_ui);
            
            m_nextPressedConnection = m_nextButton->GetReleasedInsideEvent().OpenConnection([](CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer, CSInput::Pointer::InputType in_inputType)
            {
                CSCore::Application::Get()->GetStateManager()->Change(std::make_shared<TNextState>());
            });
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TNextState> void StateNavigator<TNextState>::OnDestroy()
        {
            m_nextPressedConnection.reset();
            m_nextButton.reset();
            
            m_ui->RemoveFromParent();
            m_ui.reset();
        }
    }
}

#endif
