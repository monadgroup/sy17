#include "Divide.h"

using namespace OmedaNodes;

void Divide::Generate()
{
	*outputs[0] = *inputs[0] / *inputs[1];
}
