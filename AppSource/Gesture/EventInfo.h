//
//  EventInfo.h
//  CSTest
//  Created by Ian Copland on 04/03/2016.
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

#ifndef _GESTURE_EVENTINFO_H_
#define _GESTURE_EVENTINFO_H_

#include <CSTest.h>

#include <ChilliSource/Core/System.h>

namespace CSTest
{
    namespace Gesture
    {
        //------------------------------------------------------------------------------
        /// A container for information of events. This can record two types of events:
        /// immediate and active. Immediate events are recorded be keeping a tally of
        /// the number of events which have occurred. Active events keep track of those
        /// which are currently active, with the ability to present information of their
        /// current state.
        ///
        /// This is not thread safe and should never be accessed accross threads.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        class EventInfo final
        {
        public:
            //------------------------------------------------------------------------------
            /// A struct that contains information on a single immediate event counter,
            /// including the event name and the current event count.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            struct EventCounter final
            {
                std::string m_name;
                u32 m_count;
            };
            //------------------------------------------------------------------------------
            /// A struct that contains information on a single active event, including the
            /// name and a string of additional information.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            struct ActiveEvent final
            {
                std::string m_name;
                std::string m_info;
            };
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The list of all current event counters.
            //------------------------------------------------------------------------------
            const std::vector<EventCounter>& GetEventCounters() const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The list of all current active counters.
            //------------------------------------------------------------------------------
            const std::vector<ActiveEvent>& GetActiveEvents() const;
            //------------------------------------------------------------------------------
            /// Increments the counter with the given immediate event name. If one does not
            /// exist then a new counter with a count of one will be created.
            ///
            /// @author Ian Copland
            ///
            /// @param in_name - The unique, displayable name of the event.
            //------------------------------------------------------------------------------
            void IncrementEventCounter(const std::string& in_name);
            //------------------------------------------------------------------------------
            /// Adds a new active event with the given name and info. If an event with the
            /// same name already exists, a fatal error will occur.
            ///
            /// @author Ian Copland
            ///
            /// @param in_name - The unique, displayable name of the event.
            /// @param in_info - Information on the event.
            //------------------------------------------------------------------------------
            void AddActiveEvent(const std::string& in_name, const std::string& in_info);
            //------------------------------------------------------------------------------
            /// Updates the information on the given active event. The event must exist or
            /// a fatal error will occur.
            ///
            /// @author Ian Copland
            ///
            /// @param in_name - The name of the event to update.
            /// @param in_info - The updated information on the event.
            //------------------------------------------------------------------------------
            void UpdateActiveEvent(const std::string& in_name, const std::string& in_info);
            //------------------------------------------------------------------------------
            /// Removes the active event with the given name. The event must exist or a
            /// fatal error will occur.
            ///
            /// @author Ian Copland
            ///
            /// @param in_name - The name of the event to remove.
            //------------------------------------------------------------------------------
            void RemoveActiveEvent(const std::string& in_name);
            
        private:
            std::vector<EventCounter> m_eventCounters;
            std::vector<ActiveEvent> m_activeEvents;
        };
    }
}

#endif
