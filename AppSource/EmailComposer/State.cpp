//
//  State.cpp
//  CSTest
//  Created by Ian Copland on 08/03/2016.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2016 Tag Games Limited
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

#include <EmailComposer/State.h>

#include <Common/Core/ResultPresenter.h>
#include <Common/Core/SmokeTester.h>
#include <Common/Core/SmokeTestSet.h>
#include <Common/Core/StateNavigator.h>

#include <IntegrationTest/State.h>

#include <ChilliSource/Core/DialogueBox.h>
#include <ChilliSource/Core/Scene.h>
#include <ChilliSource/Social/Communications.h>

namespace CSTest
{
    namespace EmailComposer
    {
        namespace
        {
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param in_result - The send result enum.
            ///
            /// @return The result represented as a string.
            //------------------------------------------------------------------------------
            std::string ToString(CSSocial::EmailComposer::SendResult in_result)
            {
                switch (in_result)
                {
                    case CSSocial::EmailComposer::SendResult::k_succeed:
                        return "Success";
                    case CSSocial::EmailComposer::SendResult::k_failed:
                        return "Failed";
                    case CSSocial::EmailComposer::SendResult::k_cancelled:
                        return "Cancelled";
                    default:
                        CS_LOG_FATAL("Invalid result.");
                }
            }
        }
        
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::CreateSystems()
        {
            CreateSystem<Common::StateNavigator<IntegrationTest::State>>();
            m_smokeTester = CreateSystem<Common::SmokeTester>();
            m_resultPresenter = CreateSystem<Common::ResultPresenter>();
            m_emailComposer = CreateSystem<CSSocial::EmailComposer>();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::OnInit()
        {
            GetScene()->SetClearColour(CSCore::Colour(0.9f, 0.9f, 0.9f, 1.0f));
            
            Common::SmokeTestSet testSet("Email Composer");
            
            if (m_emailComposer != nullptr)
            {
                testSet.AddTest("Text Email", [=]()
                {
                    std::vector<std::string> addresses;
                    addresses.push_back("test.01@test.com");
                    addresses.push_back("test.02@test.com");
                    
                    std::string subject = "Example email subject.";
                    std::string contents = "This is an example email contents.";
                    
                    m_emailComposer->Present(addresses, subject, contents, CSSocial::EmailComposer::ContentFormat::k_plainText, [=](CSSocial::EmailComposer::SendResult in_result)
                    {
                        m_resultPresenter->Present("Email composer dismissed with result: " + ToString(in_result));
                    });
                });
                
                testSet.AddTest("HTML Email", [=]()
                {
                    std::vector<std::string> addresses;
                    addresses.push_back("test.01@test.com");
                    addresses.push_back("test.02@test.com");

                    std::string subject = "Example email subject.";
                    std::string contents = "<ul><li>This</li><li>Is</li><li>Some</li><li>HTML</li></ul>";

                    m_emailComposer->Present(addresses, subject, contents, CSSocial::EmailComposer::ContentFormat::k_html, [=](CSSocial::EmailComposer::SendResult in_result)
                    {
                        m_resultPresenter->Present("Email composer dismissed with result: " + ToString(in_result));
                    });
                });
                
                testSet.AddTest("Email With Attachment", [=]()
                {
                    std::vector<std::string> addresses;
                    addresses.push_back("test.01@test.com");

                    std::string subject = "Example email subject.";
                    std::string contents = "This is an example email contents.";

                    CSSocial::EmailComposer::Attachment attachment;
                    attachment.m_filename = "EmailComposer/Attachment.zip";
                    attachment.m_storageLocation = CSCore::StorageLocation::k_chilliSource;
                    attachment.m_mimeType = "application/zip";

                    m_emailComposer->PresentWithAttachment(addresses, subject, contents, CSSocial::EmailComposer::ContentFormat::k_plainText, attachment, [=](CSSocial::EmailComposer::SendResult in_result)
                    {
                        m_resultPresenter->Present("Email composer dismissed with result: " + ToString(in_result));
                    });
                });
            }
            
            m_smokeTester->Present(testSet);
        }
    }
}
