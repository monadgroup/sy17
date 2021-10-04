#include "StateVariableFilter.h"
#include "Util.h"

#define _USE_MATH_DEFINES
#include <cmath>

using namespace OmedaNodes;


void StateVariableFilter::Generate()
{
	auto input = *inputs[0];
	auto freq = *inputs[1];
	auto q = *inputs[2];

	auto f = 2 * sinf(M_PI * freq / OmedaCommon::Util::CurrentSampleRate);
	auto qv = 1 / q;

	auto damp = OmedaCommon::Util::Min(
		2 * (1 - pow(1 - 1 / (2 * q), 0.25)),
		OmedaCommon::Util::Min(
			2,
			2 / f - f * 0.5
		)
	);

	for (auto i = 0; i < 2; i++)
	{
		notch = input - damp * band;
		low = low + f * band;
		high = notch - low;
		band = f * high + band;
	}

	*outputs[0] = high;
	*outputs[1] = low;
	*outputs[2] = band;
	*outputs[3] = notch;
}
