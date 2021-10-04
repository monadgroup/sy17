#include "Track.h"
#include "Util.h"
#include "InputData.h"
#include <memory.h>
#include <climits>

using namespace OmedaPlayer;
using namespace OmedaCommon;

Track::Track(const InputTrackData *inputTrack, const float *staticBuffer)
{
	masterVolume = 0.3f;//inputTrack->masterVolume / 255.0f;
	paramValues = new float[inputTrack->paramCount];
	voiceCount = inputTrack->voiceCount;

	// create voices
	voices = new Voice*[voiceCount];
	for (auto i = 0; i < voiceCount; i++)
	{
		auto voice = new Voice(&inputTrack->polyData, staticBuffer, paramValues);
		voices[i] = voice;
	}

	fx = new Voice(&inputTrack->fxData, staticBuffer, paramValues);
	fx->isEffect = true;

	// create note events
	noteEventCount = inputTrack->noteEventCount;
	noteDeltaTimes = inputTrack->noteDeltaTimes;
	noteDurations = inputTrack->noteDurations;
	notes = inputTrack->notes;
	noteVelocities = inputTrack->noteVelocities;
	notePans = inputTrack->notePans;

	// create automation events
	paramCount = inputTrack->paramCount;
	parameters = new Parameter*[paramCount];
	for (auto i = 0; i < paramCount; i++)
	{
		parameters[i] = new Parameter(paramValues + i, inputTrack->parameters + i);
	}

	nextNoteEvent = 0;
	samplesToNextNote = nextNoteEvent >= noteEventCount ? -1 : Util::BeatToSamples(noteDeltaTimes[nextNoteEvent]);
}

Track::~Track()
{
	delete[] paramValues;

	for (auto i = 0; i < voiceCount; i++)
	{
		delete voices[i];
	}
	delete[] voices;

	delete fx;

	for (auto i = 0; i < paramCount; i++)
	{
		delete parameters[i];
	}
	delete[] parameters;
}

void Track::render(short buffer[][2], int bufferSize) const
{
	for (auto s = 0; s < bufferSize; s++)
	{
		auto leftSample = 0.0f;
		auto rightSample = 0.0f;

		for (auto i = 0; i < voiceCount; i++)
		{
			auto voice = voices[i];
			if (!voice->isActive && !voice->noteOn) continue;

			voice->Generate();
			if (voice->isActive)
			{
				leftSample += *voice->inputs[1];
				rightSample += *voice->inputs[2];
			}
		}

		*fx->outputs[0] = leftSample;
		*fx->outputs[1] = rightSample;
		fx->Generate();
		buffer[s][0] = Util::Clamp(*fx->inputs[0] * masterVolume, -1.0f, 1.0f) * SHRT_MAX;
		buffer[s][1] = Util::Clamp(*fx->inputs[1] * masterVolume, -1.0f, 1.0f) * SHRT_MAX;
	}
}

void Track::Render(short buffer[][2], int bufferSize)
{
	for (auto s = 0; s < bufferSize; s++)
	{
		while (samplesToNextNote == 0)
		{
			for (auto i = 0; i < voiceCount; i++)
			{
				auto voice = voices[i];
				if (voice->isActive || voice->noteOn) continue;

				voice->isActive = true;
				voice->NoteOn(
					notePans[nextNoteEvent] / 64.0f - 1,
					noteVelocities[nextNoteEvent] / 255.0f,
					notes[nextNoteEvent],
					Util::BeatToSamples(noteDurations[nextNoteEvent])
				);
				break;
			}

			nextNoteEvent++;
			samplesToNextNote = nextNoteEvent >= noteEventCount ? -1 : Util::BeatToSamples(noteDeltaTimes[nextNoteEvent]);
		}
		if (samplesToNextNote > 0) samplesToNextNote--;

		for (auto i = 0; i < paramCount; i++)
		{
			parameters[i]->Update();
		}

		auto leftSample = 0.0f;
		auto rightSample = 0.0f;

		for (auto i = 0; i < voiceCount; i++)
		{
			auto voice = voices[i];
			if (!voice->isActive && !voice->noteOn) continue;

			voice->Generate();
			if (voice->isActive)
			{
				leftSample += *voice->inputs[1];
				rightSample += *voice->inputs[2];
			}
		}

		*fx->outputs[0] = leftSample;
		*fx->outputs[1] = rightSample;
		fx->Generate();

		buffer[s][0] = Util::ClampToShort(buffer[s][0] + *fx->inputs[0] * masterVolume * SHRT_MAX);
		buffer[s][1] = Util::ClampToShort(buffer[s][1] + *fx->inputs[1] * masterVolume * SHRT_MAX);
	}
}
