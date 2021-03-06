//
//  OptionsMenuPresenter.cpp
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

#include <Common/UI/OptionsMenuPresenter.h>

#include <Common/UI/OptionsMenuDesc.h>
#include <Common/UI/BasicWidgetFactory.h>

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
            constexpr f32 k_relativeSpacing = 0.065f;
            constexpr f32 k_listMargins = 0.2f;
            constexpr u32 k_numItemsPerColumn = 5;
            constexpr u32 k_numColumns = 2;
            constexpr u32 k_maxItemsPerPage = k_numItemsPerColumn * k_numColumns;
        }
        
        CS_DEFINE_NAMEDTYPE(OptionsMenuPresenter);
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        OptionsMenuPresenterUPtr OptionsMenuPresenter::Create() noexcept
        {
            return OptionsMenuPresenterUPtr(new OptionsMenuPresenter());
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool OptionsMenuPresenter::IsA(CS::InterfaceIDType in_interfaceId) const noexcept
        {
            return (OptionsMenuPresenter::InterfaceID == in_interfaceId);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void OptionsMenuPresenter::Present(const OptionsMenuDesc& in_desc) noexcept
        {
            m_desc = in_desc;
            m_numPages = (u32(m_desc.GetButtons().size() + k_maxItemsPerPage - 1) / k_maxItemsPerPage);
            if (m_numPages > 0)
            {
                SetPage(0);
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void OptionsMenuPresenter::SetPage(u32 in_page) noexcept
        {
            CS_ASSERT((in_page < m_numPages && in_page >= 0), "Error: Page does not exist.");
            m_currentPage = in_page;
            
            Dismiss();

            if (m_desc.GetButtons().size() < k_numItemsPerColumn)
            {
                auto layoutComponent = m_buttonContainer->GetComponent<CS::LayoutUIComponent>();
                layoutComponent->ApplyLayoutDef(CS::UILayoutDefSPtr(new CS::VListUILayoutDef(k_numItemsPerColumn, CS::Vector4(0.0f, k_listMargins, 0.0f, k_listMargins), CS::Vector4::k_zero, k_relativeSpacing, 0.0f)));
            }
            else
            {
                auto layoutComponent = m_buttonContainer->GetComponent<CS::LayoutUIComponent>();
                layoutComponent->ApplyLayoutDef(CS::UILayoutDefSPtr(new CS::GridUILayoutDef(CS::GridUILayout::CellOrder::k_rowMajor, k_numItemsPerColumn, k_numColumns, CS::Vector4::k_zero, CS::Vector4::k_zero, k_relativeSpacing, 0.0f, k_relativeSpacing, 0.0f)));
            }

            auto basicWidgetFactory = CS::Application::Get()->GetSystem<Common::BasicWidgetFactory>();

            auto iterBegin = m_desc.GetButtons().begin() + (in_page * k_maxItemsPerPage);
            if (iterBegin > m_desc.GetButtons().end())
            {
                iterBegin = m_desc.GetButtons().end();
            }

            auto iterEnd = iterBegin + std::min(k_maxItemsPerPage, u32(m_desc.GetButtons().size() - k_maxItemsPerPage * m_currentPage));
            if (iterEnd > m_desc.GetButtons().end())
            {
                iterEnd = m_desc.GetButtons().end();
            }

            for (auto& buttonDesc = iterBegin; buttonDesc != iterEnd; ++buttonDesc)
            {
                CS::WidgetSPtr button = basicWidgetFactory->CreateStretchableButton(CS::Vector2::k_one, buttonDesc->m_name, CS::AlignmentAnchor::k_middleCentre, CS::Colour(0.92f, 0.95f, 0.98f, 1.0f));
                m_buttonContainer->AddWidget(button);

                u32 callbackIndex = u32(buttonDesc - m_desc.GetButtons().begin()); //index required as dereferencing iterator does not work in callbacks

                auto connection = button->GetReleasedInsideEvent().OpenConnection([=](CS::Widget* in_widget, const CS::Pointer& in_pointer, CS::Pointer::InputType in_inputType)
                {
                    m_desc.GetButtons().at(callbackIndex).m_action();
                });

                m_buttons.push_back(button);
                m_connectionContainer.push_back(std::move(connection));
            }

            CreatePageNavigationButtons();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void OptionsMenuPresenter::CreatePageNavigationButtons() noexcept
        {
            auto basicWidgetFactory = CS::Application::Get()->GetSystem<Common::BasicWidgetFactory>();

            if (m_currentPage > 0)
            {
                CS::WidgetSPtr button = basicWidgetFactory->CreateStretchableButton(CS::Vector2::k_one, "Prev", CS::AlignmentAnchor::k_middleCentre, CS::Colour(0.92f, 0.95f, 0.98f, 1.0f));
                m_paginationButtonContainer->AddWidget(button);

                auto connection = button->GetReleasedInsideEvent().OpenConnection([=](CS::Widget* in_widget, const CS::Pointer& in_pointer, CS::Pointer::InputType in_inputType)
                {
                    SetPage(m_currentPage - 1);
                });

                m_paginationButtons.push_back(button);
                m_connectionContainer.push_back(std::move(connection));
            }
            else
            {
                CS::WidgetSPtr dummy = basicWidgetFactory->CreateBlank(CS::Vector2::k_one); //create dummy widget to fill out blank space.
                m_paginationButtonContainer->AddWidget(dummy);
                m_paginationButtons.push_back(dummy);
            }

            if (m_currentPage < m_numPages - 1)
            {
                CS::WidgetSPtr button = basicWidgetFactory->CreateStretchableButton(CS::Vector2::k_one, "Next", CS::AlignmentAnchor::k_middleCentre, CS::Colour(0.92f, 0.95f, 0.98f, 1.0f));
                m_paginationButtonContainer->AddWidget(button);

                auto connection = button->GetReleasedInsideEvent().OpenConnection([=](CS::Widget* in_widget, const CS::Pointer& in_pointer, CS::Pointer::InputType in_inputType)
                {
                    SetPage(m_currentPage + 1);
                });

                m_paginationButtons.push_back(button);
                m_connectionContainer.push_back(std::move(connection));
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void OptionsMenuPresenter::Dismiss() noexcept
        {
            m_connectionContainer.clear();
        
            for (const auto& button : m_buttons)
            {
                button->RemoveFromParent();
            }

            for (const auto& button : m_paginationButtons)
            {
                button->RemoveFromParent();
            }

            m_buttons.clear();
            m_paginationButtons.clear();
          
            auto layoutComponent = m_buttonContainer->GetComponent<CS::LayoutUIComponent>();
            layoutComponent->ApplyLayoutDef(CS::UILayoutDefSPtr(new CS::VListUILayoutDef(k_numItemsPerColumn, CS::Vector4::k_zero, CS::Vector4::k_zero, k_relativeSpacing, 0.0f)));
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void OptionsMenuPresenter::OnInit() noexcept
        {
            auto resourcePool = CS::Application::Get()->GetResourcePool();
            auto mediumFont = resourcePool->LoadResource<CS::Font>(CS::StorageLocation::k_package, "Fonts/ArialMed.csfont");
            
            auto widgetFactory = CS::Application::Get()->GetWidgetFactory();
            m_buttonContainer = widgetFactory->CreateLayout();
            m_buttonContainer->SetParentalAnchor(CS::AlignmentAnchor::k_topCentre);
            m_buttonContainer->SetOriginAnchor(CS::AlignmentAnchor::k_topCentre);
            m_buttonContainer->SetRelativePosition(CS::Vector2(0.0f, -0.15f));
            m_buttonContainer->SetRelativeSize(CS::Vector2(0.75f, 0.65f));

            m_paginationButtonContainer = widgetFactory->CreateLayout();
            m_paginationButtonContainer->SetParentalAnchor(CS::AlignmentAnchor::k_bottomRight);
            m_paginationButtonContainer->SetRelativePosition(CS::Vector2(-0.25f, 0.10f));
            m_paginationButtonContainer->SetRelativeSize(CS::Vector2(0.25f, 0.10f));
            auto paginationLayout = m_paginationButtonContainer->GetComponent<CS::LayoutUIComponent>();
            paginationLayout->ApplyLayoutDef(CS::UILayoutDefSPtr(new CS::GridUILayoutDef(CS::GridUILayout::CellOrder::k_rowMajor, 1, 2, CS::Vector4::k_zero, CS::Vector4::k_zero, k_relativeSpacing, 0.0f, k_relativeSpacing, 0.0f)));
            
            m_numPages = 0;
            m_currentPage = 0;

            Dismiss();
            
            GetState()->GetUICanvas()->AddWidget(m_buttonContainer);
            GetState()->GetUICanvas()->AddWidget(m_paginationButtonContainer);         
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void OptionsMenuPresenter::OnDestroy() noexcept
        {
            Dismiss();

            m_buttonContainer->RemoveFromParent();
            m_paginationButtonContainer->RemoveFromParent();
            m_buttonContainer.reset();
            m_paginationButtonContainer.reset();
        }
    }
}
