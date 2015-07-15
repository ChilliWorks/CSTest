//
//  TestReporter.cpp
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

#include <IntegrationTest/CSLogReporter.h>

#include <internal/catch_reporter_registrars.hpp>

namespace CSTest
{
    namespace IntegrationTest
    {
        INTERNAL_CATCH_REGISTER_REPORTER("cs", CSLogReporter);
        
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        std::string CSLogReporter::getDescription()
        {
            return "Reports using ChilliSource's logging functionality.";
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        CSLogReporter::CSLogReporter(const Catch::ReporterConfig& in_config)
            : StreamingReporterBase(in_config)
        {
            CS_ASSERT(m_config->includeSuccessfulResults() == false, "CSLogReporter doesn't support reporting successful results.");
            CS_ASSERT(m_config->warnAboutMissingAssertions() == false, "CSLogReporter doesn't support warning about missing assertions.");
            CS_ASSERT(m_config->showInvisibles() == false, "CSLogReporter doesn't support showing invisibles.");
            CS_ASSERT(m_config->showDurations() == false, "CSLogReporter doesn't support showing durations.");
            CS_ASSERT(m_config->forceColour() == false, "CSLogReporter doesn't support coloured output.");
            CS_ASSERT(m_config->shouldDebugBreak() == false, "CSLogReporter doesn't support debug break.");
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        Catch::ReporterPreferences CSLogReporter::getPreferences() const
        {
            Catch::ReporterPreferences prefs;
            prefs.shouldRedirectStdOut = true;
            return prefs;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
//        void CSLogReporter::noMatchingTestCases(const std::string& in_spec)
//        {
//            StreamingReporterBase::noMatchingTestCases(in_spec);
//        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void CSLogReporter::assertionStarting(const Catch::AssertionInfo& in_assertionInfo)
        {
            //StreamingReporterBase::assertionStarting(in_assertionInfo);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool CSLogReporter::assertionEnded(const Catch::AssertionStats& in_assertionStats)
        {
//            if (StreamingReporterBase::assertionEnded(in_assertionStats) == false)
//                return false;
            
            const Catch::AssertionResult& assertionResult = in_assertionStats.assertionResult;
            
            // Print any info messages in <Info> tags.
            if(assertionResult.getResultType() != Catch::ResultWas::Ok)
            {
                for(auto it = in_assertionStats.infoMessages.begin(), itEnd = in_assertionStats.infoMessages.end(); it != itEnd; ++it)
                {
                    if(it->type == Catch::ResultWas::Info)
                    {
                        CS_LOG_VERBOSE("Info: " + it->message);
                    }
                    else if (it->type == Catch::ResultWas::Warning)
                    {
                        CS_LOG_VERBOSE("Warning: " + it->message);
                    }
                }
            }
            
            // Drop out if result was successful but we're not printing them.
            if(m_config->includeSuccessfulResults() == false && isOk(assertionResult.getResultType()) == true)
            {
                return true;
            }
            
            // Print the expression if there is one.
            if(assertionResult.hasExpression() == true)
            {
                CS_LOG_VERBOSE("Expression: success=" + CSCore::ToString(assertionResult.succeeded()) + " type=" + assertionResult.getTestMacroName() +
                               " filename=" + assertionResult.getSourceInfo().file + " line=" + CSCore::ToString(assertionResult.getSourceInfo().line));
                
                CS_LOG_VERBOSE("Original: " + assertionResult.getExpression());
                CS_LOG_VERBOSE("Expanded: " + assertionResult.getExpandedExpression());
            }
            
            // And... Print a result applicable to each result type.
            switch( assertionResult.getResultType() ) {
                case Catch::ResultWas::ThrewException:
                    CS_LOG_VERBOSE("Exception: " + assertionResult.getMessage());
                    break;
                case Catch::ResultWas::FatalErrorCondition:
                    CS_LOG_VERBOSE("Fatal Error Condition: " + assertionResult.getMessage());
                    break;
                case Catch::ResultWas::Info:
                    CS_LOG_VERBOSE("Info: " + assertionResult.getMessage());
                    break;
                case Catch::ResultWas::Warning:
                    // Warning will already have been written
                    break;
                case Catch::ResultWas::ExplicitFailure:
                    CS_LOG_VERBOSE("Failure: " + assertionResult.getMessage());
                    break;
                default:
                    break;
            }
            
            return true;
        }
//        //------------------------------------------------------------------------------
//        //------------------------------------------------------------------------------
//        void CSLogReporter::sectionStarting(const Catch::SectionInfo& in_sectionInfo)
//        {
//            StreamingReporterBase::sectionStarting(in_sectionInfo);
//            
//            CS_LOG_VERBOSE("Starting section '" + in_sectionInfo.name + "': " + in_sectionInfo.description);
//        }
//        //------------------------------------------------------------------------------
//        //------------------------------------------------------------------------------
//        void CSLogReporter::sectionEnded(const Catch::SectionStats& in_sectionStats)
//        {
//            CS_LOG_VERBOSE("Ending section '" + in_sectionStats.sectionInfo.name + "' with " + CSCore::ToString(in_sectionStats.assertions.failed) + " errors.");
//        }
//        //------------------------------------------------------------------------------
//        //------------------------------------------------------------------------------
//        void CSLogReporter::testCaseEnded(const Catch::TestCaseStats& in_testCaseStats)
//        {
//            
//        }
//        //------------------------------------------------------------------------------
//        //------------------------------------------------------------------------------
//        void CSLogReporter::testGroupEnded(const Catch::TestGroupStats& in_testGroupStats)
//        {
//            
//        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void CSLogReporter::testRunEnded(const Catch::TestRunStats& in_testRunStats)
        {
            StreamingReporterBase::testRunEnded(in_testRunStats);
            
            if (in_testRunStats.totals.assertions.allPassed() == 0)
            {
                CS_LOG_VERBOSE("All tests passed!");
            }
            else
            {
                std::size_t failed = in_testRunStats.totals.assertions.failed;
                std::size_t total = in_testRunStats.totals.assertions.total();
                CS_LOG_VERBOSE("OverallResults: " + CSCore::ToString(failed) + " out of " + CSCore::ToString(total) + " tests failed.");
            }
            
            
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        CSLogReporter::~CSLogReporter()
        {
            
        }
    }
}

