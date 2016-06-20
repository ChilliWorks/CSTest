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

#include <TextEntry/TextEntryPresenter.h>

#include <Common/UI/BasicWidgetFactory.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Core/State.h>
#include <ChilliSource/Input/TextEntry.h>
#include <ChilliSource/Rendering/Font.h>
#include <ChilliSource/UI/Base.h>
#include <ChilliSource/UI/Text.h>

namespace CSTest
{
    namespace TextEntry
    {
        
        CS_DEFINE_NAMEDTYPE(TextEntryPresenter);

        //------------------------------------------------------------------------------
        TextEntryPresenterUPtr TextEntryPresenter::Create()
        {
            return TextEntryPresenterUPtr(new TextEntryPresenter());
        }

        //------------------------------------------------------------------------------
        bool TextEntryPresenter::IsA(CS::InterfaceIDType in_interfaceId) const
        {
            return (TextEntryPresenter::InterfaceID == in_interfaceId);
        }

        //------------------------------------------------------------------------------
        void TextEntryPresenter::InitUI() noexcept
        {
            auto resourcePool = CS::Application::Get()->GetResourcePool();
            auto smallFont = resourcePool->LoadResource<CS::Font>(CS::StorageLocation::k_package, "Fonts/ArialSmall.csfont");
            
            auto basicWidgetFactory = CS::Application::Get()->GetSystem<Common::BasicWidgetFactory>();

            auto textBox = basicWidgetFactory->CreateTextBox(CS::Vector2(0.35f, 0.10f), smallFont, "Enter Text Here", CS::AlignmentAnchor::k_topLeft,
                                                                          CS::HorizontalTextJustification::k_centre, CS::VerticalTextJustification::k_centre);

            textBox->SetRelativePosition(CS::Vector2(0.05f, -0.10f));

            m_textUIComponent = textBox->GetWidget("TextBoxText")->GetComponent<CS::TextUIComponent>();

            auto uiCanvas = GetState()->GetUICanvas();
            uiCanvas->AddWidget(std::move(textBox));
        }

        //------------------------------------------------------------------------------
        void TextEntryPresenter::OnInit() noexcept
        {
            InitUI();
            CreateTextEntryEventHandlers();
        }

        //------------------------------------------------------------------------------
        void TextEntryPresenter::OnUpdate(f32 in_deltaTime) noexcept
        {
            auto textEntrySystem = CS::Application::Get()->GetSystem<CS::TextEntry>();

            m_textUIComponent->SetText(textEntrySystem->GetTextBuffer());
        }

        //------------------------------------------------------------------------------
        void TextEntryPresenter::CreateTextEntryEventHandlers() noexcept
        {
            ActivateTextBox();            
        }

        //------------------------------------------------------------------------------
        void TextEntryPresenter::ActivateTextBox() noexcept
        {
            auto textEntrySystem = CS::Application::Get()->GetSystem<CS::TextEntry>();

            auto textChangedDelegate = [=](const std::string& text) -> bool
            {
                return(text.size() < k_maxTextLength);
            };

            auto textEntryDeactivatedDelegate = [=]()
            {
                // Do Nothing
            };

            textEntrySystem->Activate("Enter Text Here", CS::TextEntry::Type::k_text, CS::TextEntry::Capitalisation::k_sentences, textChangedDelegate, textEntryDeactivatedDelegate);
        }

        //------------------------------------------------------------------------------
        void TextEntryPresenter::OnDestroy() noexcept
        {
            m_eventConnections.clear();
        }
    }
}
