#include "Subtract.h"

using namespace OmedaNodes;

void Subtract::Generate()
{
	*outputs[0] = *inputs[0] - *inputs[1];
}
