//
//  BasicWidgetFactory.h
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

#ifndef _COMMON_UI_BASICWIDGETFACTORY_H_
#define _COMMON_UI_BASICWIDGETFACTORY_H_

#include <CSTest.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Rendering/Base.h>
#include <ChilliSource/UI/Base.h>

namespace CSTest
{
    namespace Common
    {
        //------------------------------------------------------------------------------
        /// A factory for creation of common UI types.
        ///
        /// This is not thread-safe and must only be called from the main thread.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        class BasicWidgetFactory final : public CS::AppSystem
        {
        public:
            CS_DECLARE_NAMEDTYPE(BasicWidgetFactory);
            //------------------------------------------------------------------------------
            /// Allows querying of whether or not this system implements the interface
            /// described by the given interface Id. Typically this is not called directly
            /// as the templated equivalent IsA<Interface>() is preferred.
            ///
            /// @author Ian Copland
            ///
            /// @param in_interfaceId - The interface Id.
            ///
            /// @return Whether or not the interface is implemented.
            //------------------------------------------------------------------------------
            bool IsA(CS::InterfaceIDType in_interfaceId) const override;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param in_size - The size of the button.
            /// @param in_text - The text on the button.
            /// @param in_alignment - [Optional] Both the origin and parental alignment.
            /// Defaults to middle centre.
            /// @param in_colour - [Optional] The colour of the button. Note that this does
            /// not affect the colour of the text. Defaults to white.
            /// @param in_sizePolicy - [Optional] The size policy for the button. Defaults
            /// to UseHeightMaintainingAspect.
            //------------------------------------------------------------------------------
            CS::WidgetUPtr CreateButton(const CS::Vector2& in_size, const std::string& in_text, CS::AlignmentAnchor in_alignment = CS::AlignmentAnchor::k_middleCentre,
                                          const CS::Colour& in_colour = CS::Colour::k_white, CS::SizePolicy in_sizePolicy = CS::SizePolicy::k_useHeightMaintainingAspect);
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param in_size - The size of the button.
            /// @param in_text - The text on the button.
            /// @param in_alignment - [Optional] Both the origin and parental alignment.
            /// Defaults to middle centre.
            /// @param in_colour - [Optional] The colour of the button. Note that this does
            /// not affect the colour of the text. Defaults to white.
            //------------------------------------------------------------------------------
            CS::WidgetUPtr CreateStretchableButton(const CS::Vector2& in_size, const std::string& in_text, CS::AlignmentAnchor in_alignment = CS::AlignmentAnchor::k_middleCentre,
                                                     const CS::Colour& in_colour = CS::Colour::k_white);
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param in_size - The relative size of the text widget.
            /// @param in_font - The font the text will be rendered with.
            /// @param in_text - The text which should be rendered.
            /// @param in_alignment - [Optional] Both the origin and parental alignment.
            /// Defaults to middle centre.
            /// @param in_horizontalTextJustification - [Optional] The horizontal
            /// justification of the text. Defaults to centre.
            /// @param in_verticalTextJustification - [Optional] The vertical justification
            /// of the text. Defaults to centre.
            //------------------------------------------------------------------------------
            CS::WidgetUPtr CreateLabel(const CS::Vector2& in_size, const CS::FontCSPtr& in_font, const std::string& in_text, CS::AlignmentAnchor in_alignment = CS::AlignmentAnchor::k_middleCentre,
                                         CS::HorizontalTextJustification in_horizontalTextJustification = CS::HorizontalTextJustification::k_centre,
                                         CS::VerticalTextJustification in_verticalTextJustification = CS::VerticalTextJustification::k_centre);
            //------------------------------------------------------------------------------
            /// @author Jordan Brown
            ///
            /// @param in_size - The size of the dummy widget.
            /// @param in_alignment - [Optional] Both the origin and parental alignment.
            /// Defaults to middle centre.
            //------------------------------------------------------------------------------
            CS::WidgetUPtr CreateDummy(const CS::Vector2& in_size, CS::AlignmentAnchor in_alignment = CS::AlignmentAnchor::k_middleCentre);
            
        private:
            friend class CS::Application;
            //------------------------------------------------------------------------------
            /// A factory method for creating new instances of the system.
            ///
            /// @author Ian Copland
            ///
            /// @return The new instance.
            //------------------------------------------------------------------------------
            static BasicWidgetFactoryUPtr Create();
            //------------------------------------------------------------------------------
            /// Default constructor. Declared private to ensure the system is created
            /// through State::CreateSystem<>().
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            BasicWidgetFactory() = default;
        };
    }
}

#endif
