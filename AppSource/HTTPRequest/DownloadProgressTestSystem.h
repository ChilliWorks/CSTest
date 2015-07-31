//
//  DownloadProgressTestSystem.h
//  CSTest
//
//  Created by Hugh McLaughlin on 30/07/2015.
//  Copyright (c) 2015 Tag Games Ltd. All rights reserved.
//

#ifndef _HTTPREQUEST_DOWNLOADPROGRESSTESTSYSTEM_H_
#define _HTTPREQUEST_DOWNLOADPROGRESSTESTSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/StateSystem.h>

namespace CSTest
{
    namespace HttpRequest
    {
        CS_FORWARDDECLARE_CLASS(DownloadProgressTestSystem);

        //------------------------------------------------------------------------------
        /// System used to test download functionality with UI feedback
        ///
        /// @author Hugh McLaughlin
        //------------------------------------------------------------------------------
        class DownloadProgressTestSystem final : public CSCore::StateSystem
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
            using DownloadCompleteDelegate = std::function<void(const CSNetworking::HttpResponse& in_response)>;
            
            //------------------------------------------------------------------------------
            /// Creates a new instance of the system.
            ///
            /// @author Hugh McLaughlin
            ///
            /// @return The new instance.
            //------------------------------------------------------------------------------
            static DownloadProgressTestSystemUPtr Create();
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
            bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
            //------------------------------------------------
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
            
            //------------------------------------------------------------------------------
            /// Constructor
            ///
            /// @author Hugh McLaughlin
            //------------------------------------------------------------------------------
            DownloadProgressTestSystem() = default;
            //------------------------------------------------------------------------------
            /// Creates a download progress widget
            ///
            /// @author HMcLaughlin
            ///
            /// @return Widget
            //------------------------------------------------------------------------------
            CSUI::WidgetSPtr CreateProgressWidget();
            
        private:
            
            CSCore::TimerSPtr m_downloadProgressUpdateTimer;
            CSCore::EventConnectionUPtr m_downloadProgressEventConnection;
        };
    }
}

#endif
