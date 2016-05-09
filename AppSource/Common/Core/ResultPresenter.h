//
//  ResultPresenter.h
//  CSTest
//  Created by Ian Copland on 29/07/2015.
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

#ifndef _COMMON_CORE_RESULTPRESENTER_H_
#define _COMMON_CORE_RESULTPRESENTER_H_

#include <CSTest.h>

#include <ChilliSource/Core/System.h>

namespace CSTest
{
    namespace Common
    {
        //------------------------------------------------------------------------------
        /// A system for presenting the user with a result. This simply displays the
        /// Given text in a system dialogue box for now, but this might be improved in
        /// the future.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        class ResultPresenter final : public CS::StateSystem
        {
        public:
            CS_DECLARE_NAMEDTYPE(ResultPresenter);
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
            /// Presents the results of a test. For now this simply presents a dialogue box.
            ///
            /// @author Ian Copland
            ///
            /// @param in_result - A string describing the result of the test.
            //------------------------------------------------------------------------------
            void Present(const std::string& in_result);
            
        private:
            friend class CS::State;
            //------------------------------------------------------------------------------
            /// A factory method for creating new instances of the system.
            ///
            /// @author Ian Copland
            ///
            /// @return The new instance.
            //------------------------------------------------------------------------------
            static ResultPresenterUPtr Create();
            //------------------------------------------------------------------------------
            /// Default constructor. Declared private to ensure the system is created
            /// through State::CreateSystem<ResultPresenter>().
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            ResultPresenter() = default;
            //------------------------------------------------------------------------------
            /// Initialises the Result Presenter.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnInit() override;
            //------------------------------------------------------------------------------
            /// Destroys the Result Presenter.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnDestroy() override;
            
            CS::DialogueBoxSystem* m_dialogueBoxSystem = nullptr;
        };
    }
}

#endif
