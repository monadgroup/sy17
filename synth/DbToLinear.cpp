#include "DbToLinear.h"
#include "Util.h"

using namespace OmedaNodes;

void DbToLinear::Generate()
{
	*outputs[0] = OmedaCommon::Util::DbToLinear(*inputs[0]);
}
