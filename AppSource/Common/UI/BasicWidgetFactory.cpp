//
//  BasicWidgetFactory.cpp
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

#include <Common/UI/BasicWidgetFactory.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Core/Threading.h>
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
        CS_DEFINE_NAMEDTYPE(BasicWidgetFactory);
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        BasicWidgetFactoryUPtr BasicWidgetFactory::Create()
        {
            return BasicWidgetFactoryUPtr(new BasicWidgetFactory());
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool BasicWidgetFactory::IsA(CS::InterfaceIDType in_interfaceId) const
        {
            return (BasicWidgetFactory::InterfaceID == in_interfaceId);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        CS::WidgetUPtr BasicWidgetFactory::CreateButton(const CS::Vector2& in_size, const std::string& in_text, CS::AlignmentAnchor in_alignment, const CS::Colour& in_colour, CS::SizePolicy in_sizePolicy)
        {
            CS_ASSERT(CS::Application::Get()->GetTaskScheduler()->IsMainThread(), "Cannot create widgets on background threads.");
            
            auto resourcePool = CS::Application::Get()->GetResourcePool();
            auto font = resourcePool->LoadResource<CS::Font>(CS::StorageLocation::k_package, "Fonts/ArialSmall.csfont");
            auto atlas = resourcePool->LoadResource<CS::TextureAtlas>(CS::StorageLocation::k_package, "TextureAtlases/UI/UI.csatlas");
            auto texture = resourcePool->LoadResource<CS::Texture>(CS::StorageLocation::k_package, "TextureAtlases/UI/UI.csimage");
            
            auto widgetFactory = CS::Application::Get()->GetWidgetFactory();
            
            CS::WidgetSPtr text = CreateLabel(CS::Vector2::k_one, font, in_text);
            
            auto button = widgetFactory->CreateHighlightButton();
            button->SetParentalAnchor(in_alignment);
            button->SetOriginAnchor(in_alignment);
            button->SetSizePolicy(in_sizePolicy);
            button->SetRelativeSize(in_size);
            
            auto buttonComponent = button->GetComponent<CS::HighlightComponent>();
            buttonComponent->SetHighlightColour(CS::Colour::k_lightGrey);
            buttonComponent->SetNormalDrawableDef(CS::UIDrawableDefCSPtr(new CS::StandardUIDrawableDef(texture, atlas, "Button01", in_colour)));
            
            button->AddWidget(text);
            
            return button;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        CS::WidgetUPtr BasicWidgetFactory::CreateStretchableButton(const CS::Vector2& in_size, const std::string& in_text, CS::AlignmentAnchor in_alignment, const CS::Colour& in_colour)
        {
            CS_ASSERT(CS::Application::Get()->GetTaskScheduler()->IsMainThread(), "Cannot create widgets on background threads.");
            
            auto resourcePool = CS::Application::Get()->GetResourcePool();
            auto font = resourcePool->LoadResource<CS::Font>(CS::StorageLocation::k_package, "Fonts/ArialSmall.csfont");
            auto atlas = resourcePool->LoadResource<CS::TextureAtlas>(CS::StorageLocation::k_package, "TextureAtlases/UI/UI.csatlas");
            auto texture = resourcePool->LoadResource<CS::Texture>(CS::StorageLocation::k_package, "TextureAtlases/UI/UI.csimage");
            
            auto widgetFactory = CS::Application::Get()->GetWidgetFactory();
            
            CS::WidgetSPtr text = CreateLabel(CS::Vector2::k_one, font, in_text);
            
            auto button = widgetFactory->CreateHighlightButton();
            button->SetParentalAnchor(in_alignment);
            button->SetOriginAnchor(in_alignment);
            button->SetRelativeSize(in_size);
            
            auto buttonComponent = button->GetComponent<CS::HighlightComponent>();
            buttonComponent->SetHighlightColour(CS::Colour::k_lightGrey);
            buttonComponent->SetNormalDrawableDef(CS::UIDrawableDefCSPtr(new CS::NinePatchUIDrawableDef(texture, atlas, "Button01", CS::Vector4(0.15f, 0.15f, 0.15f, 0.15f), in_colour)));
            
            button->AddWidget(text);
            
            return button;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        CS::WidgetUPtr BasicWidgetFactory::CreateLabel(const CS::Vector2& in_size, const CS::FontCSPtr& in_font, const std::string& in_text, CS::AlignmentAnchor in_alignment,
                                     CS::HorizontalTextJustification in_horizontalTextJustification, CS::VerticalTextJustification in_verticalTextJustification)
        {
            CS_ASSERT(CS::Application::Get()->GetTaskScheduler()->IsMainThread(), "Cannot create widgets on background threads.");
            
            auto widgetFactory = CS::Application::Get()->GetWidgetFactory();
            
            auto text = widgetFactory->CreateLabel();
            text->SetParentalAnchor(in_alignment);
            text->SetOriginAnchor(in_alignment);
            text->SetRelativeSize(in_size);
            
            auto textComponent = text->GetComponent<CS::TextComponent>();
            textComponent->SetFont(in_font);
            textComponent->SetText(in_text);
            textComponent->SetTextColour(CS::Colour::k_black);
            textComponent->SetHorizontalJustification(in_horizontalTextJustification);
            textComponent->SetVerticalJustification(in_verticalTextJustification);
            
            return text;
        }
    }
}
