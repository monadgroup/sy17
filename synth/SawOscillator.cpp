#include "SawOscillator.h"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace OmedaNodes;

void SawOscillator::Generate()
{
	*outputs[0] = 1 - fmodf(2 * (phase + *inputs[1]), 2);
	CommonOscillator::Generate();
}
