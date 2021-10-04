#pragma once
#include "Track.h"


namespace OmedaPlayer
{
	struct InputTrackData;

	class SongData
	{
	public:
		SongData(const float *staticBuffer, int trackCount, const InputTrackData *inputTracks);
		~SongData();

		const float *staticBuffer;
		int trackCount;
		Track **tracks;
	};

}
