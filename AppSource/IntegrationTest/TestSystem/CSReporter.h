//
//  CSReporter.h
//  CSTest
//  Created by Ian Copland on 15/07/2015.
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

#ifndef _INTEGRATIONTEST_TESTSYSTEM_CSREPORTER_H_
#define _INTEGRATIONTEST_TESTSYSTEM_CSREPORTER_H_

#include <CSTest.h>

#include <reporters/catch_reporter_bases.hpp>

namespace CSTest
{
    namespace IntegrationTest
    {
        //------------------------------------------------------------------------------
        /// //TODO: !?
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        class CSLogReporter final : public Catch::StreamingReporterBase
        {
        public:
            //------------------------------------------------------------------------------
            /// //TODO: !?
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            static std::string getDescription();
            //------------------------------------------------------------------------------
            /// //TODO: !?
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSLogReporter(const Catch::ReporterConfig& in_config);
            //------------------------------------------------------------------------------
            /// //TODO: !?
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            Catch::ReporterPreferences getPreferences() const override;
            //------------------------------------------------------------------------------
            /// //TODO: !?
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
//            void noMatchingTestCases(const std::string& in_spec) override;
            //------------------------------------------------------------------------------
            /// //TODO: !?
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void assertionStarting(const Catch::AssertionInfo& in_assertionInfo) override;
            //------------------------------------------------------------------------------
            /// //TODO: !?
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            bool assertionEnded(const Catch::AssertionStats& in_assertionStats) override;
            //------------------------------------------------------------------------------
            /// //TODO: !?
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
//            void sectionStarting(const Catch::SectionInfo& in_sectionInfo) override;
            //------------------------------------------------------------------------------
            /// //TODO: !?
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
//            void sectionEnded(const Catch::SectionStats& in_sectionStats) override;
            //------------------------------------------------------------------------------
            /// //TODO: !?
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
//            void testCaseEnded(const Catch::TestCaseStats& in_testCaseStats) override;
            //------------------------------------------------------------------------------
            /// //TODO: !?
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
//            void testGroupEnded(const Catch::TestGroupStats& in_testGroupStats) override;
            //------------------------------------------------------------------------------
            /// //TODO: !?
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void testRunEnded(const Catch::TestRunStats& in_testRunStats) override;
            //------------------------------------------------------------------------------
            /// //TODO: !?
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            ~CSLogReporter();
        };
    }
}

#endif
