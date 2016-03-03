//
//  GesturePresenter.h
//  CSTest
//  Created by Ian Copland on 03/03/2016.
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

#ifndef _GESTURE_GESTUREPRESENTER_H_
#define _GESTURE_GESTUREPRESENTER_H_

#include <CSTest.h>

#include <ChilliSource/Core/System.h>

namespace CSTest
{
    namespace Gesture
    {
        //------------------------------------------------------------------------------
        /// A system which displays information about a series of different gesture
        /// types on screen.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        class GesturePresenter final : public CSCore::StateSystem
        {
        public:
            CS_DECLARE_NAMEDTYPE(GesturePresenter);
            //------------------------------------------------------------------------------
            /// Allows querying of whether or not the component implements the
            /// interface associated with the given interface Id.
            ///
            /// @author Ian Copland
            ///
            /// @param in_interfaceId - The interface Id.
            ///
            /// @return Whether the object implements the given interface.
            //------------------------------------------------------------------------------
            bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
            
        private:
            friend class CSCore::State;
            //------------------------------------------------------------------------------
            /// A struct containing various information about the gestures which have occured.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            struct GestureInfo
            {
                u32 m_numTaps = 0;
                u32 m_numDoubleTaps = 0;
                u32 m_numTwoFingerTaps = 0;
                u32 m_numTwoFingerDoubleTaps = 0;
                u32 m_numHolds = 0;
                u32 m_numTwoFingerHolds = 0;
            };
            //------------------------------------------------------------------------------
            /// Creates a new instance of the system.
            ///
            /// @author Ian Copland
            ///
            /// @return The new instance.
            //------------------------------------------------------------------------------
            static GesturePresenterUPtr Create();
            //------------------------------------------------------------------------------
            /// Constructor
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            GesturePresenter() = default;
            //------------------------------------------------------------------------------
            /// Initialises all of the gestures that will be presented.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void InitGestures();
            //------------------------------------------------------------------------------
            /// Initialises all of the UI on which the gesture information will be presented.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void InitUI();
            //------------------------------------------------------------------------------
            /// Called when the gesture presenter is first created. This will set up the
            /// gestures which should be presented.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnInit() override;
            //------------------------------------------------------------------------------
            /// Updated every frame. Presents information on the gestures.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnUpdate(f32 in_deltaTime) override;
            //------------------------------------------------------------------------------
            /// Called when the gesture presenter is about to be destroyed. This will clean
            /// up the gestures which were being presented.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnDestroy() override;
            
            std::vector<CSInput::GestureSPtr> m_gestures;
            std::vector<CSCore::EventConnectionSPtr> m_eventConnections;
            GestureInfo m_gestureInfo;
            
            bool m_uiDirty = true;
            CSUI::WidgetSPtr m_rootUI;
            CSUI::TextComponent* m_textComponent;
        };
    }
}

#endif
