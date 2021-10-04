#include "Max.h"
#include "Util.h"

using namespace OmedaNodes;

void Max::Generate()
{
	auto in1 = *inputs[0];
	auto in2 = *inputs[1];

	*outputs[0] = in1 > in2 ? in1 : in2;
}
