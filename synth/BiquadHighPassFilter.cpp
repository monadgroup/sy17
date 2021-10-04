#include "BiquadHighPassFilter.h"

using namespace OmedaNodes;

BiquadHighPassFilter::BiquadHighPassFilter()
	: CommonBiquadFilter(OmedaDSP::BiquadFilter::Type::High)
{
	
}
