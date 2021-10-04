#include "Modulo.h"
#include <math.h>

using namespace OmedaNodes;

void Modulo::Generate()
{
	*outputs[0] = fmodf(*inputs[0], *inputs[1]);
}
