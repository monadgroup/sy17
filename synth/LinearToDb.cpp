#include "LinearToDb.h"
#include "Util.h"

using namespace OmedaNodes;

void LinearToDb::Generate()
{
	*outputs[0] = OmedaCommon::Util::LinearToDb(*inputs[0]);
}
