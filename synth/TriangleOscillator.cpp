#include "TriangleOscillator.h"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace OmedaNodes;

void TriangleOscillator::Generate()
{
	*outputs[0] = fabsf(fmodf(4 * (phase + *inputs[1]), 4) - 2) - 1;
	CommonOscillator::Generate();
}
