//  The MIT License (MIT)
//
//  Copyright © 2016 Tag Games. All rights reserved.
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

#include <DialogueBox/State.h>

#include <Common/Core/ResultPresenter.h>
#include <Common/Core/TestNavigator.h>
#include <Common/Input/BackButtonSystem.h>
#include <Common/UI/OptionsMenuPresenter.h>
#include <Common/UI/OptionsMenuDesc.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Scene.h>
#include <ChilliSource/Core/DialogueBox.h>
#include <ChilliSource/UI/Base.h>

#include <functional>

namespace CSTest
{
    namespace DialogueBox
    {
        //------------------------------------------------------------------------------
        void State::CreateSystems()
        {
            CreateSystem<Common::TestNavigator>("Dialogue Boxes");
            m_optionsMenuPresenter = CreateSystem<Common::OptionsMenuPresenter>();
            CreateSystem<Common::BackButtonSystem>();
        }

        //------------------------------------------------------------------------------
        void State::OnInit()
        {
            GetScene()->SetClearColour(CS::Colour(0.9f, 0.9f, 0.9f, 1.0f));
 
            m_dialogueBoxSystem = CS::Application::Get()->GetSystem<CS::DialogueBoxSystem>();

            Common::OptionsMenuDesc optionsMenuDesc;

            optionsMenuDesc.AddButton("Show Dialogue", [=]()
            {
                m_dialogueBoxSystem->ShowSystemDialogue(0, nullptr, "System Dialogue", "Test Successful", "OK");
            });

            optionsMenuDesc.AddButton("Show Confirm Dialogue", [=]()
            {
                auto handleConfirmationDialogue = [=](u32 in_id, CS::DialogueBoxSystem::DialogueResult in_result)
                {
                    if (in_result == CS::DialogueBoxSystem::DialogueResult::k_confirm)
                    {
                        m_dialogueBoxSystem->ShowSystemDialogue(0, nullptr, "Dialogue Result", "User clicked Confirm.", "OK");
                    }
                    else
                    {
                        m_dialogueBoxSystem->ShowSystemDialogue(0, nullptr, "Dialogue Result", "User clicked Cancel.", "OK");
                    }
                };

                m_dialogueBoxSystem->ShowSystemConfirmDialogue(1, handleConfirmationDialogue, "Confirmation Dialogue", "Confirmation Test?", "Confirm", "Cancel");
            });

#ifndef CS_TARGETPLATFORM_WINDOWS
            optionsMenuDesc.AddButton("Show Toast", [=]()
            {
                m_dialogueBoxSystem->MakeToast("Toast Test Successful");
            });
#endif

            m_optionsMenuPresenter->Present(optionsMenuDesc);
        }
    }
}
