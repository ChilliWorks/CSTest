//
//  State.h
//  CSTest
//  Created by HMcLaughlin on 28/07/2015.
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

#ifndef _DOWNLOADPROGRESS_STATE_H_
#define _DOWNLOADPROGRESS_STATE_H_

#include <CSTest.h>

#include <ChilliSource/Core/State.h>

namespace CSTest
{
    namespace DownloadProgress
    {
        //------------------------------------------------------------------------------
        /// A state for testing HTTP request download progress.
        ///
        /// @author HMcLaughlin
        //------------------------------------------------------------------------------
        class State final : public CS::State
        {
        private:
            //------------------------------------------------------------------------------
            /// The life-cycle event for creating all state systems.
            ///
            /// @author HMcLaughlin
            //------------------------------------------------------------------------------
            void CreateSystems() override;
            //------------------------------------------------------------------------------
            /// Displays the results of a http response
            ///
            /// @author HMcLaughlin
            ///
            /// @param in_response - The HTTP response.
            //------------------------------------------------------------------------------
            void PresentHttpResponse(const CS::HttpResponse& in_response);
            //------------------------------------------------------------------------------
            /// Initialises the state.
            ///
            /// @author HMcLaughlin
            //------------------------------------------------------------------------------
            void OnInit() override;
            
        private:
            Common::OptionsMenuPresenter* m_optionsMenuPresenter = nullptr;
            Common::ResultPresenter* m_resultPresenter = nullptr;
            CS::HttpRequestSystem* m_httpRequestSystem = nullptr;
            DownloadProgressTestSystem* m_downloadProgressTestSystem = nullptr;
        };
    }
}

#endif
