/*
 * File: FMODSystem.h
 * Date: 17/11/2010 2010 
 * Description: Wrapper that initialises FMOD and pools resources such as channels
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MO_FLO_AUDIO_FMOD_FMOD_SYSTEM_H_
#define _MO_FLO_AUDIO_FMOD_FMOD_SYSTEM_H_

#include <fmod.hpp>
#include <fmod_errors.h>
#include <fmod_event.hpp>

#include <ChilliSource/Audio/Base/AudioSystem.h>

namespace ChilliSource
{
	namespace iOS
	{
		const u32 kudwMaxFMODChannels = 32;
		
		class CFMODSystem : public Audio::IAudioSystem
		{
		public:
			CFMODSystem();
			virtual ~CFMODSystem();
			
			//-------------------------------------------------------
			/// Load Event Data
			///
			/// Tell FMOD to load event data generated by the
			/// fmod designer
			///
            /// @param Location
			/// @param File path
			//-------------------------------------------------------
			void LoadEventData(Core::StorageLocation ineLocation, const std::string& instrFilePath);
			//-------------------------------------------------------
			/// Unload Event Data
			///
			///	Unload all event data (useful for memory warnings)
			///
			/// @param File path
			//-------------------------------------------------------
			void UnloadEventData();
			//-------------------------------------------------------
			/// Unload Event Data
			///
			///	Unload all event data (useful for memory warnings)
			///
			/// @param File path
			//-------------------------------------------------------
			void UnloadEventData(const std::string& instrEventGroup);			
			//-------------------------------------------------------
			/// PreLoad Event Group
			///
			/// Preload events inside a specified group
			///
			/// @param The name of the group
			//-------------------------------------------------------
			void PreloadEventGroup(const std::string& instrGroupName);
			//-------------------------------------------------------
			/// Create Sound
			///
			/// Tell FMOD to create a new sound from the given
			/// file
			///
			/// @param File path
			/// @param Sound handle to be initialised with sound
			//-------------------------------------------------------
			void CreateSound(const std::string& instrFilePath, Audio::IAudioResource* inpAudio);
			//-------------------------------------------------------
			/// Create 3D Sound
			///
			/// Tell FMOD to create a new sound from the given
			/// file
			///
			/// @param File path
			/// @param Sound handle to be initialised with sound
			//-------------------------------------------------------
			void Create3DSound(const std::string& instrFilePath, Audio::IAudioResource* inpAudio);
			//-------------------------------------------------------
			/// Create Stream
			///
			/// Tell FMOD to stream from given file
			///
			/// @param File path
			/// @param Stream handle
			//-------------------------------------------------------
			void CreateStream(const std::string& instrFilePath, Audio::IAudioResource* inpAudio);
			//-------------------------------------------------------
			/// Create 3D Stream
			///
			/// Tell FMOD to stream from given file
			///
			/// @param File path
			/// @param Stream handle
			//-------------------------------------------------------
			void Create3DStream(const std::string& instrFilePath, Audio::IAudioResource* inpAudio);
			//-------------------------------------------------------
			/// Play Sound
			///
			/// Tell the FMOD system to play the sound with a free
			/// channel
			///
			/// @param FMOD sound handle
			//-------------------------------------------------------
			void PlaySound(Audio::CAudioComponent* inpAudioComponent);
			//-------------------------------------------------------
			/// Play Event
			///
			/// Tell the FMOD system to start the sound event
			///
			/// @param name of event
			//-------------------------------------------------------
			FMOD::Event * PlayEvent(const std::string& instrEventName);
			//-------------------------------------------------------
			/// Update
			///
			/// Tell the system to update
			/// @param Time since last frame
			//-------------------------------------------------------
			void Update(f32 dt);
			//-------------------------------------------------------
			/// Create Audio Listener
			///
			/// @return Audio listener
			//-------------------------------------------------------
			Audio::AudioListenerPtr CreateAudioListener();
			//-------------------------------------------------------
			/// Destroy 
			///
			/// Release the FMOD system
			//-------------------------------------------------------
			void Destroy();
			
            //-------------------------------------------------------
			/// Get FMOD Event System
			///
			/// @return FMOD Event System
			//-------------------------------------------------------
            FMOD::EventSystem* GetFMODEventSystem() { return mpFMODEventSystem; }
            
		private:
            //-------------------------------------------------------
            /// On Application Memory Warning
            ///
            /// Triggered when a message is received detailing
            /// that the OS has thrown a memory warning. We should
            /// release the FMOD event cache at this point
            //-------------------------------------------------------
			void OnApplicationMemoryWarning();
			//-------------------------------------------------------
			/// Error Check
			///
			/// Log any FMOD errors
			/// @param Result of FMOD function return
			//-------------------------------------------------------
			void ErrorCheck(FMOD_RESULT ineResult);
			
		private:
			
			FMOD::System* mpFMODSystem;
			FMOD::EventSystem* mpFMODEventSystem;
			FMOD::EventProject* mpFMODEventProject;

		};
	}
}

#endif