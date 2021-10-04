#include "SquareOscillator.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Util.h"

using namespace OmedaNodes;

void SquareOscillator::Generate()
{
	*outputs[0] = OmedaCommon::Util::Sgn(sinf(2 * M_PI * (phase + *inputs[1])));
	CommonOscillator::Generate();
}
