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

#include <IntegrationTest/TestSystem/Report.h>

#include <reporters/catch_reporter_bases.hpp>

namespace CSTest
{
    namespace IntegrationTest
    {
        //------------------------------------------------------------------------------
        /// Receives events from Catch to generate a report.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        class CSReporter final : public Catch::StreamingReporterBase
        {
        public:
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A description of the the reporter.
            //------------------------------------------------------------------------------
            static std::string getDescription();
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The report generated during the last test run.
            //------------------------------------------------------------------------------
            static const Report& getReport();
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param in_config - The configuration. Note that CSReport only supports a
            /// subset of available configs. It will assert if the config isn't supported.
            //------------------------------------------------------------------------------
            CSReporter(const Catch::ReporterConfig& in_config);
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The reporter preferences.
            //------------------------------------------------------------------------------
            Catch::ReporterPreferences getPreferences() const override;
            //------------------------------------------------------------------------------
            /// Called when an assertion begins.
            ///
            /// @author Ian Copland
            ///
            /// @param in_assertionInfo - The assertion info.
            //------------------------------------------------------------------------------
            void assertionStarting(const Catch::AssertionInfo& in_assertionInfo) override {};
            //------------------------------------------------------------------------------
            /// Called when an assertion ends, with stats.
            ///
            /// @author Ian Copland
            ///
            /// @param in_assertionStats - The stats on the given assertion.
            //------------------------------------------------------------------------------
            bool assertionEnded(const Catch::AssertionStats& in_assertionStats) override;
            //------------------------------------------------------------------------------
            /// Called when an assertion ends, with stats.
            ///
            /// @author Ian Copland
            ///
            /// @param in_testCaseStats - The stats on how the test case went.
            //------------------------------------------------------------------------------
            void testCaseEnded(const Catch::TestCaseStats& in_testCaseStats) override;
            //------------------------------------------------------------------------------
            /// Called when the test run ends, with stats.
            ///
            /// @author Ian Copland
            ///
            /// @param in_testRunStats - in_testRunStats
            //------------------------------------------------------------------------------
            void testRunEnded(const Catch::TestRunStats& in_testRunStats) override;
            
        private:
            std::vector<FailedAssertion> m_currentFailedAssertions;
            std::vector<TestCase> m_currentFailedTestCases;
            
            static Report s_report; //needs to be accessible from TestSystem
        };
    }
}

#endif
