#include "StepSequencer.h"

using namespace OmedaNodes;

void StepSequencer::Generate()
{
	auto time = int(*inputs[0]);
	if (time < 0) time = 0;
	if (time > 7) time = 7;

	*outputs[0] = *inputs[time + 1];
}
