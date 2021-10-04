#include "BiquadLowPassFilter.h"

using namespace OmedaNodes;

BiquadLowPassFilter::BiquadLowPassFilter()
	: CommonBiquadFilter(OmedaDSP::BiquadFilter::Type::Low)
{

}
