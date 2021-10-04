#include "SineOscillator.h"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace OmedaNodes;

void SineOscillator::Generate()
{
	*outputs[0] = sinf(2 * M_PI * (phase + *inputs[1]));
	CommonOscillator::Generate();
}
