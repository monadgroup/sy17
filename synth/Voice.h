#pragma once
#include "Node.h"

namespace OmedaPlayer
{
	struct InputInstrumentData;

	class Voice
	{
	public:
		bool isEffect = false;
		bool isActive = false;
		bool noteOn = false;
		int remainingSamples = 0;

		float **inputs = nullptr;
		float **outputs = nullptr;

		float *buffer = nullptr;

		int nodeCount = 0;
		OmedaNodes::Node **nodes = nullptr;

		explicit Voice(const InputInstrumentData *inputInstrument, const float *staticBuffer, float *paramBuffer);
		~Voice();

		void Generate();

		void NoteOn(float pan, float vol, float pitch, int duration);
	};

}
