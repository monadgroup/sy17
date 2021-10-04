#include "RampOscillator.h"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace OmedaNodes;

void RampOscillator::Generate()
{
	*outputs[0] = fmodf(2 * (phase + *inputs[1]), 2) - 1;
	CommonOscillator::Generate();
}
