#pragma once

namespace OmedaPlayer
{
	class SongData;

	class SongPlayer
	{
		SongData *data;

	public:
		explicit SongPlayer(SongData *data);

		void Render(short buffer[][2], int bufferSize) const;
	};

}