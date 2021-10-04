#include "CommonBiquadFilter.h"

using namespace OmedaNodes;

CommonBiquadFilter::CommonBiquadFilter(OmedaDSP::BiquadFilter::Type type)
{
	filter.SetType(type);
}

void CommonBiquadFilter::Generate()
{
	filter.SetFreq(*inputs[1]);
	filter.SetQ(*inputs[2]);
	if (filter.GetType() == OmedaDSP::BiquadFilter::Type::Peak) filter.SetGain(*inputs[3]);
	*outputs[0] = filter.Run(*inputs[0]);
}
