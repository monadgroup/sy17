#include "BiquadFilter.h"
#include "Util.h"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace OmedaDSP;

BiquadFilter::BiquadFilter(Type type)
{
	recalculate = true;
	this->type = type;

	freq = 2000;
	q = 0.5;
	gain = 6;

	a0 = 1;
	a1 = a2 = b1 = b2 = 0;

	z1 = z2 = 0;
}

float BiquadFilter::Run(float input)
{
	// from http://www.earlevel.com/main/2012/11/26/biquad-c-source-code/
	if (recalculate)
	{
		float norm;
		auto V = powf(10, fabsf(gain) / 20);
		auto K = tanf(M_PI * freq / OmedaCommon::Util::CurrentSampleRate);

		switch (type)
		{
		case Type::Low:
			norm = 1 / (1 + K / q + K * K);
			a0 = K * K * norm;
			a1 = 2 * a0;
			a2 = a0;
			b1 = 2 * (K * K - 1) * norm;
			b2 = (1 - K / q + K * K) * norm;
			break;

		case Type::High:
			norm = 1 / (1 + K / q + K * K);
			a0 = 1 * norm;
			a1 = -2 * a0;
			a2 = a0;
			b1 = 2 * (K * K - 1) * norm;
			b2 = (1 - K / q + K * K) * norm;
			break;

		case Type::Peak:
			if (gain >= 0)
			{
				norm = 1 / (1 + 1 / q * K + K * K);
				a0 = (1 + V / q * K + K * K) * norm;
				a1 = 2 * (K * K - 1) * norm;
				a2 = (1 - V / q * K + K * K) * norm;
				b1 = a1;
				b2 = (1 - 1 / q * K + K * K) * norm;
			}
			else
			{
				norm = 1 / (1 + V / q * K + K * K);
				a0 = (1 + 1 / q * K + K * K) * norm;
				a1 = 2 * (K * K - 1) * norm;
				a2 = (1 - 1 / q * K + K * K) * norm;
				b1 = a1;
				b2 = (1 - V / q * K + K * K) * norm;
			}
			break;
		}
		recalculate = false;
	}

	auto out = input * a0 + z1;
	z1 = input * a1 + z2 - b1 * out;
	z2 = input * a2 - b2 * out;

	return out;
}
