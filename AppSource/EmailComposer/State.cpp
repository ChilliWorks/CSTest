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
#include <Common/Input/BackButton.h>
#include <Common/UI/OptionsMenuPresenter.h>
#include <Common/UI/OptionsMenuDesc.h>
#include <Common/Core/TestNavigator.h>

#include <UnitTest/State.h>

#include <ChilliSource/Core/DialogueBox.h>
#include <ChilliSource/Core/File.h>
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
            std::string ToString(CS::EmailComposer::SendResult in_result)
            {
                switch (in_result)
                {
                    case CS::EmailComposer::SendResult::k_succeed:
                        return "Success";
                    case CS::EmailComposer::SendResult::k_failed:
                        return "Failed";
                    case CS::EmailComposer::SendResult::k_cancelled:
                        return "Cancelled";
                    default:
                        CS_LOG_FATAL("Invalid result.");
						return "";
                }
            }
        }
        
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::CreateSystems()
        {
            CreateSystem<Common::TestNavigator>("Email Composer");
            m_optionsMenuPresenter = CreateSystem<Common::OptionsMenuPresenter>();
            m_resultPresenter = CreateSystem<Common::ResultPresenter>();
            m_emailComposer = CreateSystem<CS::EmailComposer>();
            m_backButtonSystem = CreateSystem<Common::BackButtonSystem>();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::OnInit()
        {
            GetScene()->SetClearColour(CS::Colour(0.9f, 0.9f, 0.9f, 1.0f));
            
            Common::OptionsMenuDesc optionsMenuDesc;
            
            if (m_emailComposer != nullptr)
            {
                optionsMenuDesc.AddButton("Text Email", [=]()
                {
                    std::vector<std::string> addresses;
                    addresses.push_back("test.01@test.com");
                    addresses.push_back("test.02@test.com");
                    
                    std::string subject = "Example email subject.";
                    std::string contents = "This is an example email contents.";
                    
                    m_emailComposer->Present(addresses, subject, contents, CS::EmailComposer::ContentFormat::k_plainText, [=](CS::EmailComposer::SendResult in_result)
                    {
                        m_resultPresenter->Present("Email composer dismissed with result: " + ToString(in_result));
                    });
                });
                
                optionsMenuDesc.AddButton("HTML Email", [=]()
                {
                    std::vector<std::string> addresses;
                    addresses.push_back("test.01@test.com");
                    addresses.push_back("test.02@test.com");

                    std::string subject = "Example email subject.";
                    std::string contents = "<ul><li>This</li><li>Is</li><li>Some</li><li>HTML</li></ul>";

                    m_emailComposer->Present(addresses, subject, contents, CS::EmailComposer::ContentFormat::k_html, [=](CS::EmailComposer::SendResult in_result)
                    {
                        m_resultPresenter->Present("Email composer dismissed with result: " + ToString(in_result));
                    });
                });
                
                optionsMenuDesc.AddButton("Email With Attachment", [=]()
                {
                    std::vector<std::string> addresses;
                    addresses.push_back("test.01@test.com");

                    std::string subject = "Example email subject.";
                    std::string contents = "This is an example email contents.";

                    CS::EmailComposer::Attachment attachment;
                    attachment.m_filename = "EmailComposer/Attachment.zip";
                    attachment.m_storageLocation = CS::StorageLocation::k_chilliSource;
                    attachment.m_mimeType = "application/zip";

                    m_emailComposer->PresentWithAttachment(addresses, subject, contents, CS::EmailComposer::ContentFormat::k_plainText, attachment, [=](CS::EmailComposer::SendResult in_result)
                    {
                        m_resultPresenter->Present("Email composer dismissed with result: " + ToString(in_result));
                    });
                });
            }
            
            m_optionsMenuPresenter->Present(optionsMenuDesc);
        }
    }
}
