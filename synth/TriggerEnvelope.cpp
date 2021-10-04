#include "TriggerEnvelope.h"
#include "Util.h"
#include <math.h>

using namespace OmedaNodes;

void TriggerEnvelope::Generate()
{
	auto trig = *inputs[0] != 0;
	auto attack = *inputs[1];
	auto decay = *inputs[2];
	auto sustain = *inputs[3];
	auto release = *inputs[4];

	// detect rising [trigger]/falling [off] edge
	if (trig != lastTrig)
	{
		if (trig)
		{
			state = State::Attack;
			pos = 0;
		} else
		{
			releaseValue = lastValue;
			state = release > 0 ? State::Release : State::Finished;
			pos = 0;
		}
	}
	lastTrig = trig;

	// update pos
	if (state != State::Finished)
	{
		auto posDelta = (float)(1000.0 / OmedaCommon::Util::CurrentSampleRate);
		switch (state)
		{
		case State::Attack:
			pos += posDelta;
			if (pos >= attack)
			{
				pos = 0.0f;
				state = decay > 0 ? State::Decay : State::Sustain;
			}
			break;
		case State::Decay:
			pos += posDelta;
			if (pos >= decay)
			{
				pos = 0.0f;
				state = State::Sustain;
			}
			break;
		case State::Release:
			pos += posDelta;
			if (pos >= release)
			{
				pos = 0.0f;
				state = State::Finished;
			}
			break;
		}
	}

	// update value
	switch (state)
	{
	case State::Attack:
		lastValue = pos / attack;
		break;
	case State::Decay:
		{
		auto f = 1.0f - powf(1.0f - pos / decay, 2.0f);
		lastValue = 1.0f * (1.0f - f) + sustain * f;
		}
		break;
	case State::Sustain:
		lastValue = sustain;
		break;
	case State::Release:
		{
		auto f = 1.0f - powf(1.0f - pos / release, 2.0f);
		lastValue = releaseValue * (1.0f - f);
		}
		break;
	default: lastValue = 0.0f;;
	}

	*outputs[0] = lastValue;
}
