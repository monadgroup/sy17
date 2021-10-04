#pragma once
#include "Voice.h"
#include "Parameter.h"

namespace OmedaPlayer
{
	struct InputTrackData;

	class Track
	{
		void render(short buffer[][2], int bufferSize) const;

	public:
		float masterVolume = 1;

		int voiceCount = 0;
		Voice **voices = nullptr;
		Voice *fx = nullptr;

		int paramCount = 0;
		float *paramValues = nullptr;
		Parameter **parameters = nullptr;

		int nextNoteEvent = 0;
		int samplesToNextNote = 0;

		int noteEventCount = 0;
		short *noteDeltaTimes = nullptr;
		short *noteDurations = nullptr;
		unsigned char *notes = nullptr;
		unsigned char *noteVelocities = nullptr;
		unsigned char *notePans = nullptr;

		int nextAutomationEvent = 0;
		int samplesToNextAutomation = 0;

		int automationEventCount = 0;
		short *automationDeltaTimes = nullptr;
		unsigned char *automationParameters = nullptr;
		unsigned char *automationValues = nullptr;

		unsigned char lastAutomationValue = 0;

		Track(const InputTrackData *inputTrack, const float *staticBuffer);
		~Track();

		void Render(short buffer[][2], int bufferSize);
	};

}
