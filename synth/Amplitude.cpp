#include "Amplitude.h"
#include <math.h>
#include "Util.h"

using namespace OmedaNodes;

Amplitude::Amplitude()
{
	a1 = expf(-1 / (0.05f * (float)OmedaCommon::Util::CurrentSampleRate));
	b0 = 1 - a1;
}

void Amplitude::Generate()
{
	estimate += b0 * (fabsf(*inputs[0]) - estimate);
	*outputs[0] = estimate;
}
