#include "Voice.h"
#include "InputData.h"
#include "Export.h"
#include <cstdio>
#include <cmath>

using namespace OmedaPlayer;

static float *getPlugLocation(InputPlugData *inputPlug, float *dynamicBuffer, const float *staticBuffer, float *paramBuffer)
{
	switch (inputPlug->mode)
	{
	case InputPlugMode::Dynamic: return dynamicBuffer + inputPlug->bufferLocation;
	case InputPlugMode::Static: return (float*)(staticBuffer + inputPlug->bufferLocation);
	case InputPlugMode::Parameter: return paramBuffer + inputPlug->bufferLocation;
	default: return nullptr;
	}
}

Voice::Voice(const InputInstrumentData* inputInstrument, const float *staticBuffer, float *paramBuffer)
{
	buffer = new float[inputInstrument->bufferSize];

	inputs = new float*[inputInstrument->inputCount];
	for (auto i = 0; i < inputInstrument->inputCount; i++)
	{
		inputs[i] = getPlugLocation(inputInstrument->inputs + i, buffer, staticBuffer, paramBuffer);
	}
	outputs = new float*[inputInstrument->outputCount];
	for (auto i = 0; i < inputInstrument->outputCount; i++)
	{
		outputs[i] = buffer + inputInstrument->outputs[i];
	}

	nodeCount = inputInstrument->nodeCount;
	nodes = new OmedaNodes::Node*[nodeCount];
	for (auto i = 0; i < nodeCount; i++)
	{
		auto inputNode = inputInstrument->nodeData + i;
		auto node = CreateNodeFromType(inputNode->type);
		nodes[i] = node;

		// setup inputs
		node->inputs = new float*[inputNode->inputCount];
		for (auto m = 0; m < inputNode->inputCount; m++)
		{
			node->inputs[m] = getPlugLocation(inputNode->inputs + m, buffer, staticBuffer, paramBuffer);
		}

		// setup outputs
		node->outputs = new float*[inputNode->outputCount];
		for (auto m = 0; m < inputNode->outputCount; m++)
		{
			node->outputs[m] = buffer + inputNode->outputs[m];
		}
	}
}

Voice::~Voice()
{
	for (auto i = 0; i < nodeCount; i++)
	{
		auto node = nodes[i];
		delete[] node->inputs;
		delete[] node->outputs;
		delete node;
	}

	delete[] nodes;

	delete[] inputs;
	delete[] outputs;
	delete[] buffer;
}

void Voice::Generate()
{
	if (!isEffect && noteOn) {
		if (remainingSamples <= 0)
		{
			noteOn = false;
			*outputs[0] = 0;
		}
		else remainingSamples--;

		//if (fmod((double)remainingSamples, 1000.0) == 0) printf("Remaining samples: %i\n", remainingSamples);
	}

	for (auto v = 0; v < nodeCount; v++)
	{
		nodes[v]->Generate();
	}

	if (!isEffect) isActive = *inputs[0] != 0;
}

void Voice::NoteOn(float pan, float vol, float pitch, int duration)
{
	isActive = true;
	noteOn = true;
	*outputs[0] = 1;
	*outputs[1] = pitch;
	*outputs[2] = pan;
	*outputs[3] = vol;
	remainingSamples = duration;

	for (auto i = 0; i < nodeCount; i++)
	{
		nodes[i]->NoteOn();
	}
}
