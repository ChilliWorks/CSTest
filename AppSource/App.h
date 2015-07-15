//
//  App.h
//  CSTest
//  Created by Ian Copland on 13/07/2015.
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

#ifndef _APP_H_
#define _APP_H_

#include <CSTest.h>

#include <ChilliSource/Core/Base.h>

namespace CSTest
{
    //------------------------------------------------------------------------------
    /// The main application class, providing the entry point to the a CS app and
    /// events for creating systems and pushing the first state.
    ///
    /// @author Ian Copland
    //------------------------------------------------------------------------------
    class App final : public CSCore::Application
    {
        //------------------------------------------------------------------------------
        /// The life-cycle event for creating all application systems.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        void CreateSystems() override;
        //------------------------------------------------------------------------------
        /// Initialisation event called immediately after create systems and before
        /// push initial state.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        void OnInit() override;
        //------------------------------------------------------------------------------
        /// Creates the first state that will be added to the state stack.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        void PushInitialState() override;
        //------------------------------------------------------------------------------
        /// Called when the application is about to be destroyed.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        void OnDestroy() override;
    };
}

#endif
