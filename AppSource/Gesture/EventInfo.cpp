//
//  EventInfo.cpp
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

#include <Gesture/EventInfo.h>

#include <algorithm>

namespace CSTest
{
    namespace Gesture
    {
        namespace
        {
            //------------------------------------------------------------------------------
            /// Searches the given active event list for the event with the given name. If
            /// found a valid iterator is returned, otherwise in_activeEvents.end() is
            /// returned.
            ///
            /// @author Ian Copland
            ///
            /// @param in_activeEvents - The active events list to search.
            /// @param in_name - The name of the event.
            ///
            /// @return The iterator.
            //------------------------------------------------------------------------------
            std::vector<EventInfo::ActiveEvent>::iterator FindActiveEventWithName(std::vector<EventInfo::ActiveEvent>& in_activeEvents, const std::string& in_name)
            {
                return std::find_if(in_activeEvents.begin(), in_activeEvents.end(), [=](const EventInfo::ActiveEvent& activeEvent)
                {
                    return (activeEvent.m_name == in_name);
                });
            }
        }
        
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        const std::vector<EventInfo::EventCounter>& EventInfo::GetEventCounters() const
        {
            return m_eventCounters;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        const std::vector<EventInfo::ActiveEvent>& EventInfo::GetActiveEvents() const
        {
            return m_activeEvents;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void EventInfo::IncrementEventCounter(const std::string& in_eventName)
        {
            for (auto& counter : m_eventCounters)
            {
                if (counter.m_name == in_eventName)
                {
                    counter.m_count++;
                    return;
                }
            }
            
            EventCounter counter;
            counter.m_name = in_eventName;
            counter.m_count = 1;
            m_eventCounters.push_back(counter);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void EventInfo::AddActiveEvent(const std::string& in_name, const std::string& in_info)
        {
            CS_ASSERT(FindActiveEventWithName(m_activeEvents, in_name) == m_activeEvents.end(), "Event already exists.");
            
            ActiveEvent event;
            event.m_name = in_name;
            event.m_info = in_info;
            m_activeEvents.push_back(event);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void EventInfo::UpdateActiveEvent(const std::string& in_name, const std::string& in_info)
        {
            auto event = FindActiveEventWithName(m_activeEvents, in_name);
            CS_ASSERT(event != m_activeEvents.end(), "Event doesn't exist.");
            
            event->m_info = in_info;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void EventInfo::RemoveActiveEvent(const std::string& in_name)
        {
            auto event = FindActiveEventWithName(m_activeEvents, in_name);
            CS_ASSERT(event != m_activeEvents.end(), "Event doesn't exist.");
            
            m_activeEvents.erase(event);
        }
    }
}
