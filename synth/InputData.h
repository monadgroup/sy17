#pragma once

namespace OmedaPlayer
{
	enum class InputPlugMode
	{
		Dynamic,
		Static,
		Parameter
	};

	struct InputPlugData
	{
		InputPlugMode mode;
		int bufferLocation;

		InputPlugData(InputPlugMode m, int bl)
		{
			mode = m;
			bufferLocation = bl;
		}
	};

	struct InputNodeData
	{
		unsigned char type;

		int inputCount;
		InputPlugData *inputs;

		int outputCount;
		int *outputs;
	};

	struct InputInstrumentData
	{
		int nodeCount;
		InputNodeData *nodeData;

		int inputCount;
		InputPlugData *inputs;

		int outputCount;
		int *outputs;

		int bufferSize;
	};

	struct InputParameterData
	{
		int eventCount;
		short *eventDeltaTimes;
		unsigned char *eventValues;
	};

	struct InputTrackData
	{
		unsigned char masterVolume;

		int voiceCount;
		InputInstrumentData polyData;
		InputInstrumentData fxData;

		int paramCount;
		InputParameterData *parameters;

		int noteEventCount = 0;
		short *noteDeltaTimes = nullptr;
		short *noteDurations = nullptr;
		unsigned char *notes = nullptr;
		unsigned char *noteVelocities = nullptr;
		unsigned char *notePans = nullptr;

		InputTrackData(unsigned char masterVolume,
					   int voiceCount, InputInstrumentData polyData, InputInstrumentData fxData,
					   int paramCount, InputParameterData *parameters,
                       int noteEventCount, short *noteDeltaTimes, short *noteDurations, unsigned char *notes, unsigned char *noteVelocities, unsigned char *notePans)
                : masterVolume(masterVolume),
                  voiceCount(voiceCount), polyData(polyData), fxData(fxData),
                  paramCount(paramCount), parameters(parameters),
                  noteEventCount(noteEventCount), noteDeltaTimes(noteDeltaTimes), noteDurations(noteDurations), notes(notes), noteVelocities(noteVelocities), notePans(notePans) { }
	};
}