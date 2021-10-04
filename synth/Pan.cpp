#include "Pan.h"

using namespace OmedaNodes;

void Pan::Generate()
{
	auto in = *inputs[0];
	auto pan = *inputs[1];

	if (pan < 0) {
		*outputs[0] = in;
		*outputs[1] = in * (1 + pan);
	} else
	{
		*outputs[0] = in * (1 - pan);
		*outputs[1] = in;
	}
}
