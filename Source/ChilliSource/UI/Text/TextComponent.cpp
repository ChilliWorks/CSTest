//
//  TextComponent.cpp
//  Chilli Source
//  Created by Ian Copland on 05/11/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#include <ChilliSource/UI/Text/TextComponent.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/Localisation/LocalisedText.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Rendering/Font/Font.h>

namespace ChilliSource
{
    namespace UI
    {
        namespace
        {
            const char k_fontKey[] = "Font";
            const char k_localisedTextKey[] = "LocalisedText";
            const char k_localisedTextIdKey[] = "LocalisedTextId";
            const char k_textKey[] = "Text";
            const char k_textColourKey[] = "TextColour";
            const char k_horizontalJustificationKey[] = "HorizontalJustification";
            const char k_verticalJustificationKey[] = "VerticalJustification";
            const char k_absCharSpacingOffsetKey[] = "AbsCharSpacingOffset";
            const char k_absLineSpacingOffsetKey[] = "AbsLineSpacingOffset";
            const char k_lineSpacingScaleKey[] = "LineSpacingScale";
            const char k_maxNumberOfLinesKey[] = "MaxNumberOfLines";
            const char k_textScaleKey[] = "TextScale";
            
            const std::vector<PropertyMap::PropertyDesc> k_propertyDescs =
            {
                {PropertyType::k_font, k_fontKey},
                {PropertyType::k_localisedText, k_localisedTextKey},
                {PropertyType::k_string, k_localisedTextIdKey},
                {PropertyType::k_string, k_textKey},
                {PropertyType::k_colour, k_textColourKey},
                {PropertyType::k_horizontalTextJustification, k_horizontalJustificationKey},
                {PropertyType::k_verticalTextJustification, k_verticalJustificationKey},
                {PropertyType::k_float, k_absCharSpacingOffsetKey},
                {PropertyType::k_float, k_absLineSpacingOffsetKey},
                {PropertyType::k_float, k_lineSpacingScaleKey},
                {PropertyType::k_int, k_maxNumberOfLinesKey},
                {PropertyType::k_float, k_textScaleKey}
            };
        }
        
        CS_DEFINE_NAMEDTYPE(TextComponent);
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const std::vector<PropertyMap::PropertyDesc>& TextComponent::GetPropertyDescs()
        {
            return k_propertyDescs;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        TextComponent::TextComponent(const std::string& in_componentName, const PropertyMap& in_properties)
            : Component(in_componentName)
        {
            if (in_properties.HasValue(k_fontKey) == true)
            {
                SetFont(in_properties.GetProperty<Rendering::FontCSPtr>(k_fontKey));
            }
            else
            {
                auto resourcePool = Core::Application::Get()->GetResourcePool();
                SetFont(resourcePool->LoadResource<Rendering::Font>(Core::StorageLocation::k_chilliSource, "Fonts/CarlitoMed.csfont"));
            }
            
            if (in_properties.HasValue(k_localisedTextKey) == true)
            {
                SetLocalisedText(in_properties.GetProperty<Core::LocalisedTextCSPtr>(k_localisedTextKey));
            }
            
            SetLocalisedTextId(in_properties.GetPropertyOrDefault(k_textKey, GetLocalisedTextId()));
            SetText(in_properties.GetPropertyOrDefault(k_textKey, GetText()));
            SetTextColour(in_properties.GetPropertyOrDefault(k_textColourKey, GetTextColour()));
            SetAbsoluteCharacterSpacingOffset(in_properties.GetPropertyOrDefault(k_absCharSpacingOffsetKey, GetAbsoluteCharacterSpacingOffset()));
            SetAbsoluteLineSpacingOffset(in_properties.GetPropertyOrDefault(k_absLineSpacingOffsetKey, GetAbsoluteLineSpacingOffset()));
            SetLineSpacingScale(in_properties.GetPropertyOrDefault(k_lineSpacingScaleKey, GetLineSpacingScale()));
            SetMaxNumberOfLines(in_properties.GetPropertyOrDefault(k_maxNumberOfLinesKey, (s32)GetMaxNumberOfLines()));
            SetTextScale(in_properties.GetPropertyOrDefault(k_textScaleKey, GetTextScale()));
            SetHorizontalJustification(in_properties.GetPropertyOrDefault(k_horizontalJustificationKey, GetHorizontalJustification()));
            SetVerticalJustification(in_properties.GetPropertyOrDefault(k_verticalJustificationKey, GetVerticalJustification()));
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        bool TextComponent::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return (Component::InterfaceID == in_interfaceId || TextComponent::InterfaceID == in_interfaceId);
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const Rendering::FontCSPtr& TextComponent::GetFont() const
        {
            return m_font;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const Core::LocalisedTextCSPtr& TextComponent::GetLocalisedText() const
        {
            return m_localisedText;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const std::string& TextComponent::GetLocalisedTextId() const
        {
            return m_localisedTextId;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const std::string& TextComponent::GetText() const
        {
            return m_text;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const Core::Colour& TextComponent::GetTextColour() const
        {
            return m_textColour;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        Rendering::HorizontalTextJustification TextComponent::GetHorizontalJustification() const
        {
            return m_textProperties.m_horizontalJustification;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        Rendering::VerticalTextJustification TextComponent::GetVerticalJustification() const
        {
            return m_textProperties.m_verticalJustification;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        f32 TextComponent::GetAbsoluteCharacterSpacingOffset() const
        {
            return m_textProperties.m_absCharSpacingOffset;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        f32 TextComponent::GetAbsoluteLineSpacingOffset() const
        {
            return m_textProperties.m_absLineSpacingOffset;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        f32 TextComponent::GetLineSpacingScale() const
        {
            return m_textProperties.m_lineSpacingScale;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        s32 TextComponent::GetMaxNumberOfLines() const
        {
            return m_textProperties.m_maxNumLines;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        f32 TextComponent::GetTextScale() const
        {
            return m_textProperties.m_textScale;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextComponent::SetFont(const Rendering::FontCSPtr& in_font)
        {
            CS_ASSERT(in_font != nullptr, "Cannot set a null font on a Text Drawable.");
            CS_ASSERT(in_font->GetLoadState() == Core::Resource::LoadState::k_loaded, "Cannot set an incomplete font on a Text Drawable.");
            
            m_font = in_font;
            
            m_invalidateCache = true;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextComponent::SetLocalisedText(const Core::LocalisedTextCSPtr& in_localisedText)
        {
#ifdef CS_ENABLE_DEBUG
            if (in_localisedText != nullptr)
            {
                CS_ASSERT(in_localisedText->GetLoadState() == Core::Resource::LoadState::k_loaded, "Cannot set text using an incomplete localised text.");
            }
#endif
            
            m_localisedText = in_localisedText;
            
            if (m_localisedText != nullptr && m_localisedText->Contains(m_localisedTextId) == true)
            {
                m_text = m_localisedText->GetText(m_localisedTextId);
            }
            else
            {
                m_text = "";
            }
            
            m_invalidateCache = true;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextComponent::SetLocalisedTextId(const std::string& in_localisedTextId)
        {
            m_localisedTextId = in_localisedTextId;
            
            if (m_localisedText != nullptr && m_localisedText->Contains(m_localisedTextId) == true)
            {
                m_text = m_localisedText->GetText(m_localisedTextId);
            }
            else
            {
                m_text = "";
            }
            
            m_invalidateCache = true;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextComponent::SetLocalisedTextId(const std::string& in_localisedTextId, const Core::ParamDictionary& in_params)
        {
            CS_ASSERT(m_localisedText != nullptr, "Cannot set text using a null localised text.");
            
            m_text = Core::StringUtils::InsertVariables(m_localisedText->GetText(in_localisedTextId), in_params);
            
            m_invalidateCache = true;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextComponent::SetText(const std::string& in_text)
        {
            m_text = in_text;
            
            m_invalidateCache = true;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextComponent::SetTextColour(const Core::Colour& in_textColour)
        {
            m_textColour = in_textColour;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextComponent::SetHorizontalJustification(Rendering::HorizontalTextJustification in_horizontalJustification)
        {
            m_textProperties.m_horizontalJustification = in_horizontalJustification;
            
            m_invalidateCache = true;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextComponent::SetVerticalJustification(Rendering::VerticalTextJustification in_verticalJustification)
        {
            m_textProperties.m_verticalJustification = in_verticalJustification;
            
            m_invalidateCache = true;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextComponent::SetAbsoluteCharacterSpacingOffset(f32 in_offset)
        {
            m_textProperties.m_absCharSpacingOffset = in_offset;
            
            m_invalidateCache = true;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextComponent::SetAbsoluteLineSpacingOffset(f32 in_offset)
        {
            m_textProperties.m_absLineSpacingOffset = in_offset;
            
            m_invalidateCache = true;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextComponent::SetLineSpacingScale(f32 in_scale)
        {
            m_textProperties.m_lineSpacingScale = in_scale;
            
            m_invalidateCache = true;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextComponent::SetMaxNumberOfLines(s32 in_numLines)
        {
            CS_ASSERT(in_numLines >= 0, "The maximum number of lines cannot be below 0.");
            
            m_textProperties.m_maxNumLines = in_numLines;
            
            m_invalidateCache = true;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextComponent::SetTextScale(f32 in_scale)
        {
            m_textProperties.m_textScale = in_scale;
            
            m_invalidateCache = true;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextComponent::OnRegisterProperties()
        {
            RegisterProperty<Rendering::FontCSPtr>(k_fontKey, CSCore::MakeDelegate(this, &TextComponent::GetFont), CSCore::MakeDelegate(this, &TextComponent::SetFont));
            RegisterProperty<Core::LocalisedTextCSPtr>(k_localisedTextKey, CSCore::MakeDelegate(this, &TextComponent::GetLocalisedText), CSCore::MakeDelegate(this, &TextComponent::SetLocalisedText));
            RegisterProperty<std::string>(k_localisedTextIdKey, CSCore::MakeDelegate(this, &TextComponent::GetLocalisedTextId), CSCore::MakeDelegate<TextComponent, TextComponent, void, const std::string&>(this, &TextComponent::SetLocalisedTextId));
            RegisterProperty<std::string>(k_textKey, CSCore::MakeDelegate(this, &TextComponent::GetText), CSCore::MakeDelegate<TextComponent, TextComponent, void, const std::string&>(this, &TextComponent::SetText));
            RegisterProperty<Core::Colour>(k_textColourKey, CSCore::MakeDelegate(this, &TextComponent::GetTextColour), CSCore::MakeDelegate(this, &TextComponent::SetTextColour));
            RegisterProperty<Rendering::HorizontalTextJustification>(k_horizontalJustificationKey, CSCore::MakeDelegate(this, &TextComponent::GetHorizontalJustification), CSCore::MakeDelegate(this, &TextComponent::SetHorizontalJustification));
            RegisterProperty<Rendering::VerticalTextJustification>(k_verticalJustificationKey, CSCore::MakeDelegate(this, &TextComponent::GetVerticalJustification), CSCore::MakeDelegate(this, &TextComponent::SetVerticalJustification));
            RegisterProperty<f32>(k_absCharSpacingOffsetKey, CSCore::MakeDelegate(this, &TextComponent::GetAbsoluteCharacterSpacingOffset), CSCore::MakeDelegate(this, &TextComponent::SetAbsoluteCharacterSpacingOffset));
            RegisterProperty<f32>(k_absLineSpacingOffsetKey, CSCore::MakeDelegate(this, &TextComponent::GetAbsoluteLineSpacingOffset), CSCore::MakeDelegate(this, &TextComponent::SetAbsoluteCharacterSpacingOffset));
            RegisterProperty<f32>(k_lineSpacingScaleKey, CSCore::MakeDelegate(this, &TextComponent::GetLineSpacingScale), CSCore::MakeDelegate(this, &TextComponent::SetLineSpacingScale));
            RegisterProperty<s32>(k_maxNumberOfLinesKey, CSCore::MakeDelegate(this, &TextComponent::GetMaxNumberOfLines), CSCore::MakeDelegate(this, &TextComponent::SetMaxNumberOfLines));
            RegisterProperty<f32>(k_textScaleKey, CSCore::MakeDelegate(this, &TextComponent::GetTextScale), CSCore::MakeDelegate(this, &TextComponent::SetTextScale));
        }
        //-------------------------------------------------------------------
        //--------------------------------------------------------------------
        void TextComponent::OnDraw(Rendering::CanvasRenderer* in_renderer, const Core::Matrix3& in_transform, const Core::Vector2& in_absSize, const Core::Colour& in_absColour)
        {
            if (m_cachedSize != in_absSize)
            {
                m_cachedSize = in_absSize;
                m_invalidateCache = true;
            }
            
            if (m_invalidateCache == true)
            {
                m_invalidateCache = false;
                m_cachedText = in_renderer->BuildText(m_text, m_font, in_absSize, m_textProperties);
            }
        
            in_renderer->DrawText(m_cachedText.m_characters, in_transform, m_textColour, m_font->GetTexture());
        }
    }
}