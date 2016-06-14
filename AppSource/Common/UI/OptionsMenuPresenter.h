//
//  OptionsMenuPresenter.h
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

#ifndef _COMMON_CORE_OPTIONSMENUPRESENTER_H_
#define _COMMON_CORE_OPTIONSMENUPRESENTER_H_

#include <CSTest.h>

#include <ChilliSource/Core/System.h>

namespace CSTest
{
    namespace Common
    {
        //------------------------------------------------------------------------------
        /// A system for presenting the user with a series of options. For each option
        /// a named button is displayed which a defined action when the button is
        /// pressed.
        ///
        /// This is not thread safe.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        class OptionsMenuPresenter final : public CS::StateSystem
        {
        public:
            CS_DECLARE_NAMEDTYPE(OptionsMenuPresenter);
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
            bool IsA(CS::InterfaceIDType in_interfaceId) const noexcept override;
            //------------------------------------------------------------------------------
            /// Presents a series of buttons, each with an associated action.
            ///
            /// @author Ian Copland / Jordan Brown
            ///
            //------------------------------------------------------------------------------
            void Present(const OptionsMenuDesc& in_desc) noexcept;
            //------------------------------------------------------------------------------
            /// Changes to the specified page, presenting new buttons to the user.
            ///
            /// @author Jordan Brown
            ///
            /// @param in_page - The desired page number, starting index 0.
            //------------------------------------------------------------------------------
            void ChangePage(const u32& in_pagek, const OptionsMenuDesc& in_desc) noexcept;
            //------------------------------------------------------------------------------
            //------------------------------------------------------------------------------
            /// Cleans up all existing buttons, and resets the system back into a reusable
            /// state.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void Dismiss() noexcept;
            
        private:
            friend class CS::State;
            //------------------------------------------------------------------------------
            /// A factory method for creating new instances of the system.
            ///
            /// @author Ian Copland
            ///
            /// @return The new instance.
            //------------------------------------------------------------------------------
            static OptionsMenuPresenterUPtr Create() noexcept;
            //------------------------------------------------------------------------------
            /// Default constructor. Declared private to ensure the system is created
            /// through State::CreateSystem<OptionsMenuPresenter>().
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            OptionsMenuPresenter() = default;
            //------------------------------------------------------------------------------
            /// Initialises the system.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnInit() noexcept override;
            //------------------------------------------------------------------------------
            /// Destroys the system.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnDestroy() noexcept override;
            
            CS::WidgetSPtr m_buttonContainer;
            CS::WidgetSPtr m_paginationButtonContainer;
            
            std::vector<CS::WidgetSPtr> m_buttons;
            std::vector<CS::WidgetSPtr> m_paginationButtons;
            std::vector<CS::EventConnectionUPtr> m_connectionContainer;
            
            u32 m_numPages;
            u32 m_currentPage;

        };
    }
}

#endif
