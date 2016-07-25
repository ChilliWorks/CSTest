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

#include <UI/State.h>

#include <Common/Core/TestNavigator.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Core/Scene.h>
#include <ChilliSource/UI/Base.h>

namespace CSTest
{
    namespace UI
    {
        //------------------------------------------------------------------------------
        void State::CreateSystems()
        {
            CreateSystem<Common::TestNavigator>("UI");
        }

        //------------------------------------------------------------------------------
        void State::OnInit()
        {
            GetScene()->SetClearColour(CS::Colour(0.9f, 0.9f, 0.9f, 1.0f));
            
            auto resourcePool = CS::Application::Get()->GetResourcePool();
            auto testUITemplate = resourcePool->LoadResource<CS::WidgetTemplate>(CS::StorageLocation::k_package, "UI/Test.csui");
            auto widgetFactory = CS::Application::Get()->GetSystem<CS::WidgetFactory>();
            CS::WidgetSPtr testUI = widgetFactory->Create(testUITemplate);
            GetUICanvas()->AddWidget(testUI);
        }
    }
}
