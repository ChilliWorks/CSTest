//
//  NotificationListener.h
//  moFloTest
//
//  Created by Scott Downie on 01/02/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _NS_NOTIFICATION_ADAPTER_H_
#define _NS_NOTIFICATION_ADAPTER_H_

#import <Foundation/Foundation.h>

#include <ChilliSource/Core/Event/GenericEvent.h>

typedef fastdelegate::FastDelegate0<> NotificationEventDelegate;

@interface NSNotificationAdapter : NSObject 
{    
    //---Media Player
    moFlo::CEvent0<NotificationEventDelegate> mMPLoadStateChangeEvent;
    moFlo::CEvent0<NotificationEventDelegate> mMPPlaybackDidFinishEvent;
}

//-----------------------------------------------
/// Shared Instance
///
/// @return Singleton instance 
//-----------------------------------------------
+(NSNotificationAdapter*) sharedInstance;

//-----------------------------------------------
/// Begin Listening For MP Load State Changed
//-----------------------------------------------
-(void) BeginListeningForMPLoadStateChanged;
//-----------------------------------------------
/// Stop Listening For MP Load State Changed
//-----------------------------------------------
-(void) StopListeningForMPLoadStateChanged;

//-----------------------------------------------
/// Begin Listening For MP Playback Did Finish
//-----------------------------------------------
-(void) BeginListeningForMPPlaybackDidFinish;
//-----------------------------------------------
/// Stop Listening For MP Playback Did Finish
//-----------------------------------------------
-(void) StopListeningForMPPlaybackDidFinish;

//-----------------------------------------------
/// Get Movie Player Load State Change Event
///
/// @return Event Object
//-----------------------------------------------
-(moFlo::IEvent<NotificationEventDelegate>&) GetMPLoadStateChangeEvent;
//-----------------------------------------------
/// Get Movie Player Playback Did Finish Event
///
/// @return Event Object
//-----------------------------------------------
-(moFlo::IEvent<NotificationEventDelegate>&) GetMPPlaybackDidFinishEvent;

//---Notification callbacks
//-----------------------------------------------
/// On Movie Player Load State Changed
//-----------------------------------------------
-(void) OnMoviePlayerLoadStateChanged;
//-----------------------------------------------
/// On Movie Player Playback Did Finish
//-----------------------------------------------
-(void) OnMoviePlayerPlaybackDidFinish;

@end

#endif