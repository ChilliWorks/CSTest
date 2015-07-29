//
//  UIFactory.cpp
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

#include <Common/UI/UIFactory.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Rendering/Font.h>
#include <ChilliSource/Rendering/Texture.h>
#include <ChilliSource/UI/Base.h>
#include <ChilliSource/UI/Button.h>
#include <ChilliSource/UI/Drawable.h>
#include <ChilliSource/UI/Text.h>

namespace CSTest
{
    namespace Common
    {
        namespace UIFactory
        {
            //------------------------------------------------------------------------------
            //------------------------------------------------------------------------------
            CSUI::WidgetUPtr CreateButton(const CSCore::Vector2& in_size, const std::string& in_text, CSRendering::AlignmentAnchor in_alignment, const CSCore::Colour& in_colour, CSUI::SizePolicy in_sizePolicy)
            {
                auto resourcePool = CSCore::Application::Get()->GetResourcePool();
                auto font = resourcePool->LoadResource<CSRendering::Font>(CSCore::StorageLocation::k_package, "Fonts/ArialSmall.csfont");
                auto atlas = resourcePool->LoadResource<CSRendering::TextureAtlas>(CSCore::StorageLocation::k_package, "TextureAtlases/UI/UI.csatlas");
                auto texture = resourcePool->LoadResource<CSRendering::Texture>(CSCore::StorageLocation::k_package, "TextureAtlases/UI/UI.csimage");
                
                auto widgetFactory = CSCore::Application::Get()->GetWidgetFactory();
                
                CSUI::WidgetSPtr text = CreateLabel(CSCore::Vector2::k_one, font, in_text);
                
                auto button = widgetFactory->CreateHighlightButton();
                button->SetParentalAnchor(in_alignment);
                button->SetOriginAnchor(in_alignment);
                button->SetSizePolicy(in_sizePolicy);
                button->SetRelativeSize(in_size);
                
                auto buttonComponent = button->GetComponent<CSUI::HighlightComponent>();
                buttonComponent->SetHighlightColour(CSCore::Colour::k_lightGrey);
                buttonComponent->SetNormalDrawableDef(CSUI::DrawableDefCSPtr(new CSUI::StandardDrawableDef(texture, atlas, "Button01", in_colour)));
                
                button->AddWidget(text);
                
                return button;
            }
            //------------------------------------------------------------------------------
            //------------------------------------------------------------------------------
            CSUI::WidgetUPtr CreateStretchableButton(const CSCore::Vector2& in_size, const std::string& in_text, CSRendering::AlignmentAnchor in_alignment, const CSCore::Colour& in_colour)
            {
                auto resourcePool = CSCore::Application::Get()->GetResourcePool();
                auto font = resourcePool->LoadResource<CSRendering::Font>(CSCore::StorageLocation::k_package, "Fonts/ArialSmall.csfont");
                auto atlas = resourcePool->LoadResource<CSRendering::TextureAtlas>(CSCore::StorageLocation::k_package, "TextureAtlases/UI/UI.csatlas");
                auto texture = resourcePool->LoadResource<CSRendering::Texture>(CSCore::StorageLocation::k_package, "TextureAtlases/UI/UI.csimage");
                
                auto widgetFactory = CSCore::Application::Get()->GetWidgetFactory();
                
                CSUI::WidgetSPtr text = CreateLabel(CSCore::Vector2::k_one, font, in_text);
                
                auto button = widgetFactory->CreateHighlightButton();
                button->SetParentalAnchor(in_alignment);
                button->SetOriginAnchor(in_alignment);
                button->SetRelativeSize(in_size);
                
                auto buttonComponent = button->GetComponent<CSUI::HighlightComponent>();
                buttonComponent->SetHighlightColour(CSCore::Colour::k_lightGrey);
                buttonComponent->SetNormalDrawableDef(CSUI::DrawableDefCSPtr(new CSUI::NinePatchDrawableDef(texture, atlas, "Button01", CSCore::Vector4(0.15f, 0.15f, 0.15f, 0.15f), in_colour)));
                
                button->AddWidget(text);
                
                return button;
            }
            //------------------------------------------------------------------------------
            //------------------------------------------------------------------------------
            CSUI::WidgetUPtr CreateLabel(const CSCore::Vector2& in_size, const CSRendering::FontCSPtr& in_font, const std::string& in_text, CSRendering::AlignmentAnchor in_alignment,
                                         CSRendering::HorizontalTextJustification in_horizontalTextJustification, CSRendering::VerticalTextJustification in_verticalTextJustification)
            {
                auto widgetFactory = CSCore::Application::Get()->GetWidgetFactory();
                
                auto text = widgetFactory->CreateLabel();
                text->SetParentalAnchor(in_alignment);
                text->SetOriginAnchor(in_alignment);
                text->SetRelativeSize(in_size);
                
                auto textComponent = text->GetComponent<CSUI::TextComponent>();
                textComponent->SetFont(in_font);
                textComponent->SetText(in_text);
                textComponent->SetTextColour(CSCore::Colour::k_black);
                textComponent->SetHorizontalJustification(in_horizontalTextJustification);
                textComponent->SetVerticalJustification(in_verticalTextJustification);
                
                return text;
            }
        }
    }
}
