//
//  ReportPresenter.h
//  CSTest
//  Created by Ian Copland on 16/03/2016.
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

#ifndef _INTEGRATIONTEST_TESTSYSTEM_REPORTPRESENTER_H_
#define _INTEGRATIONTEST_TESTSYSTEM_REPORTPRESENTER_H_

#include <CSTest.h>
#include <UnitTest/TestSystem/Report.h>

#include <ChilliSource/Core/System.h>

namespace CSTest
{
    namespace IntegrationTest
    {
        //------------------------------------------------------------------------------
        /// TODO
        ///
        /// This is not thread safe.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        class ReportPresenter final : public CSCore::StateSystem
        {
        public:
            CS_DECLARE_NAMEDTYPE(ReportPresenter);
            //------------------------------------------------------------------------------
            /// Allows querying of whether or not this system implements the interface
            /// described by the given interface Id. Typically this is not called directly
            /// as the templated equivalent IsA<Interface>() is preferred.
            ///
            /// @author Ian Copland
            ///
            /// @param The interface Id.
            ///
            /// @return Whether or not the interface is implemented.
            //------------------------------------------------------------------------------
            bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
            //------------------------------------------------------------------------------
            /// TODO
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void PresentProgress(const TestDesc& in_testDesc, u32 in_testIndex, u32 in_numTests);
            //------------------------------------------------------------------------------
            /// Presents the given report on screen. Further details of the report will also
            /// be printed to the console if appropriate.
            ///
            /// @author Ian Copland
            ///
            /// @param in_report - The test report.
            //------------------------------------------------------------------------------
            void PresentReport(const Report& in_report);
            
        private:
            friend class CSCore::State;
            //------------------------------------------------------------------------------
            /// A factory method for creating new instances of the system.
            ///
            /// @author Ian Copland
            ///
            /// @return The new instance.
            //------------------------------------------------------------------------------
            static ReportPresenterUPtr Create();
            //------------------------------------------------------------------------------
            /// Default constructor. Declared private to ensure the system is created
            /// through State::CreateSystem<ReportPresenter>().
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            ReportPresenter() = default;
            //------------------------------------------------------------------------------
            /// Sets the text displayed by the presenter to the given string. Text is
            /// rendered using a medium font, centred in the screen.
            ///
            /// @author Ian Copland
            ///
            /// @param in_text - The text to display.
            //------------------------------------------------------------------------------
            void SetText(const std::string& in_text);
            //------------------------------------------------------------------------------
            /// Initialises the report presenter.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnInit() override;
            //------------------------------------------------------------------------------
            /// Destroys the report presenter.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnDestroy() override;
            
            CSUI::WidgetSPtr m_text;
        };
    }
}

#endif
