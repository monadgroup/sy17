#include "Delay.h"
#include "Util.h"


using namespace OmedaNodes;

void Delay::setDelayTime(float newTime)
{
	delayTime = newTime;
	delaySize = newTime * 60 / OmedaCommon::Util::CurrentTempo * OmedaCommon::Util::CurrentSampleRate + 1;

	if (delaySize > bufferSize)
	{
		auto oldBuffer = buffer;
		buffer = new float[delaySize];
		auto i = 0;
		if (buffer != nullptr)
		{
			for (; i < bufferSize; i++) buffer[i] = oldBuffer[i];
			delete[] oldBuffer;
		}
		for (; i < delaySize; i++) buffer[i] = 0;

		bufferSize = delaySize;
	}

	//currentPosition %= delaySize;
}

Delay::Delay()
{
	setDelayTime(100);
}

Delay::~Delay()
{
	delete[] buffer;
}

void Delay::Generate()
{
	if (*inputs[2] != delayTime) setDelayTime(*inputs[2]);

	const int real_size =  *inputs[1] * 60 / OmedaCommon::Util::CurrentTempo * OmedaCommon::Util::CurrentSampleRate + 1;
	const auto real_len = real_size < delaySize ? real_size : delaySize;

	if (real_len == 0) currentPosition = 0;
	else currentPosition %= real_len;

	buffer[currentPosition] = *inputs[0];

	currentPosition++;
	if (real_len == 0) currentPosition = 0;
	else currentPosition %= real_len;

	*outputs[0] = buffer[currentPosition];
}
