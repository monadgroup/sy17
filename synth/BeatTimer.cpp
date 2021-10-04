#include "BeatTimer.h"
#include "Util.h"
#include <math.h>

using namespace OmedaNodes;

void BeatTimer::Generate()
{
	time = fmodf(time + (float)(1000.0 / OmedaCommon::Util::CurrentSampleRate) * OmedaCommon::Util::CurrentTempo / 60 * *inputs[0], *inputs[1] * 1000);

	*outputs[0] = time / 1000.0f;
}

void BeatTimer::NoteOn()
{
	time = 0;
}

