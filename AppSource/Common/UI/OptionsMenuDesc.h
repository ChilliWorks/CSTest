//
//  OptionsMenuDesc.h
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

#ifndef _COMMON_CORE_OPTIONSMENUDESC_H_
#define _COMMON_CORE_OPTIONSMENUDESC_H_

#include <CSTest.h>

#include <functional>
#include <vector>

namespace CSTest
{
    namespace Common
    {
        //------------------------------------------------------------------------------
        /// A description for an options menu. This consists of a series of named
        /// buttons, each with an action delegate.
        ///
        /// This is not thread safe.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        class OptionsMenuDesc final
        {
        public:
            //------------------------------------------------------------------------------
            /// The delegate called each time the associated button is pressed.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            using Action = std::function<void() noexcept>;
            //------------------------------------------------------------------------------
            /// A container for information on a single button.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            struct Button final
            {
                std::string m_name;
                Action m_action;
            };
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param in_name - The name of the button
            /// @param in_action - The action to perform when the test is selected.
            //------------------------------------------------------------------------------
            void AddButton(const std::string& in_name, const Action& in_action) noexcept;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The list of tests within the set.
            //------------------------------------------------------------------------------
            const std::vector<Button>& GetButtons() const noexcept;
            
        private:
            std::vector<Button> m_buttons;
        };
    }
}

#endif
