#include "BiquadPeakFilter.h"

using namespace OmedaNodes;

BiquadPeakFilter::BiquadPeakFilter()
	: CommonBiquadFilter(OmedaDSP::BiquadFilter::Type::Peak)
{

}
