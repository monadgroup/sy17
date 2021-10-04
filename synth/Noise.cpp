#include "Noise.h"
#include "Util.h"

using namespace OmedaNodes;

void Noise::Generate()
{
	*outputs[0] = OmedaCommon::Util::RandFloat() * 2 - 1;
}
