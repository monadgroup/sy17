#include "CommonOscillator.h"
#include "Util.h"

using namespace OmedaNodes;

void CommonOscillator::Generate()
{
	phase += *inputs[0] / OmedaCommon::Util::CurrentSampleRate;
}

void CommonOscillator::NoteOn()
{
	phase = 0;
}
