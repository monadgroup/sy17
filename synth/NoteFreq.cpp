#include "NoteFreq.h"
#include "Util.h"

using namespace OmedaNodes;

void NoteFreq::Generate()
{
	*outputs[0] = OmedaCommon::Util::NoteToFreq(*inputs[0]);
}
