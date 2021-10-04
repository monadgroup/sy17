#include "Clamp.h"

using namespace OmedaNodes;

void Clamp::Generate()
{
	auto in = *inputs[0];
	auto min = *inputs[1];
	auto max = *inputs[2];

	*outputs[0] = in < min ? min : in > max ? max : in;
}
