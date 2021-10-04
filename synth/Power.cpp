#include "Power.h"
#include <cmath>

using namespace OmedaNodes;

void Power::Generate()
{
	*outputs[0] = powf(*inputs[0], *inputs[1]);
}
