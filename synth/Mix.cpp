#include "Mix.h"
#include "Util.h"

using namespace OmedaNodes;

void Mix::Generate()
{
	*outputs[0] = OmedaCommon::Util::Mix(*inputs[0], *inputs[1], *inputs[2]);
}
