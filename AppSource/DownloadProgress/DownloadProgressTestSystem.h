//
//  DownloadProgressTestSystem.h
//  CSTest
//
//  Created by HMcLaughlin on 30/07/2015.
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

#ifndef _DOWNLOADPROGRESS_DOWNLOADPROGRESSTESTSYSTEM_H_
#define _DOWNLOADPROGRESS_DOWNLOADPROGRESSTESTSYSTEM_H_

#include <CSTest.h>

#include <ChilliSource/Core/System.h>
#include <ChilliSource/Core/Time.h>

#include <functional>

namespace CSTest
{
    namespace DownloadProgress
    {
        //------------------------------------------------------------------------------
        /// System used to test download functionality with UI feedback
        ///
        /// @author Hugh McLaughlin
        //------------------------------------------------------------------------------
        class DownloadProgressTestSystem final : public CS::StateSystem
        {
        public:
            CS_DECLARE_NAMEDTYPE(DownloadProgressTestSystem);
            
            //------------------------------------------------------------------------------
            /// Delegate called when a download is complete
            ///
            /// @author Hugh McLaughlin
            ///
            /// @param in_response - Http Response
            //------------------------------------------------------------------------------
            using DownloadCompleteDelegate = std::function<void(const CS::HttpResponse& in_response)>;
            
            //------------------------------------------------------------------------------
            /// Allows querying of whether or not the component implements the
            /// interface associated with the given interface Id.
            ///
            /// @author Hugh McLaughlin
            ///
            /// @param in_interfaceId - The interface Id.
            ///
            /// @return Whether the object implements the given interface.
            //------------------------------------------------------------------------------
            bool IsA(CS::InterfaceIDType in_interfaceId) const override;
            //------------------------------------------------------------------------------
            /// Start the download test
            ///
            /// @author Hugh McLaughlin
            ///
            /// @param in_downloadURL
            /// @param in_completeDelegate
            //------------------------------------------------------------------------------
            void StartDownloadTest(const std::string& in_downloadURL, const DownloadCompleteDelegate& in_completeDelegate);
            
        private:
            
            friend class CS::State;
            
            //------------------------------------------------------------------------------
            /// Creates a new instance of the system.
            ///
            /// @author Hugh McLaughlin
            ///
            /// @return The new instance.
            //------------------------------------------------------------------------------
            static DownloadProgressTestSystemUPtr Create();
            //------------------------------------------------------------------------------
            /// Constructor
            ///
            /// @author Hugh McLaughlin
            //------------------------------------------------------------------------------
            DownloadProgressTestSystem() = default;
            //------------------------------------------------------------------------------
            /// OnInit
            ///
            /// @author Hugh McLaughlin
            //------------------------------------------------------------------------------
            void OnInit() override;
            //------------------------------------------------------------------------------
            /// OnDestroy
            ///
            /// @author Hugh McLaughlin
            //------------------------------------------------------------------------------
            void OnDestroy() override;
            
        private:
            
            CS::WidgetSPtr m_downloadProgressWidget;
            
            CS::TimerUPtr m_downloadProgressUpdateTimer;
            CS::EventConnectionUPtr m_downloadProgressEventConnection;
        };
    }
}

#endif
