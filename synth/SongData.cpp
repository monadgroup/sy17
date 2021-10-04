#include "SongData.h"
#include "InputData.h"

using namespace OmedaPlayer;

SongData::SongData(const float* staticBuffer, int trackCount, const InputTrackData* inputTracks)
{
	this->staticBuffer = staticBuffer;
	this->trackCount = trackCount;

	// convert input track data to real track data
	tracks = new Track*[trackCount];
	for (auto i = 0; i < trackCount; i++)
	{
		tracks[i] = new Track(inputTracks + i, staticBuffer);
	}
}

SongData::~SongData()
{
	for (auto i = 0; i < trackCount; i++)
	{
		delete tracks[i];
	}
	delete[] tracks;
}
