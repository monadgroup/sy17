#include "SongPlayer.h"
#include "SongData.h"

using namespace OmedaPlayer;

SongPlayer::SongPlayer(SongData* data)
{
	this->data = data;
}

void SongPlayer::Render(short buffer[][2], int bufferSize) const
{
	for (auto i = 0; i < bufferSize; i++)
	{
		buffer[i][0] = buffer[i][1] = 0;
	}

	for (auto i = 0; i < data->trackCount; i++)
	{
		auto track = data->tracks[i];
		track->Render(buffer, bufferSize);
	}
}
