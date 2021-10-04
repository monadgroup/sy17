#pragma once

#include "SongPlayer.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>

namespace OmedaPlayer
{
	
	class DirectSoundPlayer
	{
		SongPlayer *player;
		CRITICAL_SECTION criticalSection;
		HANDLE thread;
		bool isPlaying;

		DWORD startTime;

		static DWORD WINAPI threadProc(LPVOID lpParameter);

		bool getIsPlaying();
		void setStartTime(DWORD val);
		void writeData(LPDIRECTSOUNDBUFFER buffer, int writePos, int chunkSize) const;

	public:
		DirectSoundPlayer(SongPlayer *player);
		~DirectSoundPlayer();

		void StartPlaying();
		void StopPlaying();
		double GetPlayTime();
		void Wait() const;
	};

}
