#include "Add4.h"

using namespace OmedaNodes;

void Add4::Generate()
{
	*outputs[0] = *inputs[0] + *inputs[1] + *inputs[2] + *inputs[3];
}
