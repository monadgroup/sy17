#include "Add2.h"

using namespace OmedaNodes;

void Add2::Generate()
{
	*outputs[0] = *inputs[0] + *inputs[1];
}
