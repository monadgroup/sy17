#include "ThresholdEnvelope.h"
#include "Util.h"

using namespace OmedaNodes;

void ThresholdEnvelope::Generate()
{
	auto overAmount = *inputs[0] - *inputs[1];
	if (overAmount < 0) overAmount = 0;

	if (overAmount > level)
	{
		level = OmedaCommon::Util::Mix(overAmount, level, *inputs[2]);
	} else
	{
		level = OmedaCommon::Util::Mix(overAmount, level, *inputs[3]);
	}

	*outputs[0] = level;
}
