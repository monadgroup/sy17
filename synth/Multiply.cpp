#include "Multiply.h"

using namespace OmedaNodes;

void Multiply::Generate()
{
	*outputs[0] = *inputs[0] * *inputs[1];
}
