#pragma once
#include "Util.h"
#include <climits>
#include <cstdio>

namespace OmedaPlayer
{
	struct InputParameterData;

	class Parameter
	{
		static float toVal(unsigned char i)
		{
			return i / (float)UCHAR_MAX;
		}

	public:
		float *value = nullptr;

		int lastEvent = -1;
		int samplesToNextEvent = 0;
		int nextEventTime = 0;

		int eventCount = 0;
		short *eventDeltaTimes = nullptr;
		unsigned char *eventValues = nullptr;

		Parameter(float *value, InputParameterData *data);

		void Update()
		{
			while (samplesToNextEvent == 0)
			{
				lastEvent++;
				*value = toVal(eventValues[lastEvent]);

				nextEventTime = lastEvent + 1 >= eventCount ? -1 : OmedaCommon::Util::BeatToSamples(eventDeltaTimes[lastEvent + 1]);
				samplesToNextEvent = nextEventTime;
			}

			if (samplesToNextEvent == -1) return;

			auto valStart = toVal(eventValues[lastEvent]);
			auto valEnd = toVal(eventValues[lastEvent + 1]);
			auto transition = samplesToNextEvent / (float)nextEventTime;
			*value = OmedaCommon::Util::Mix(valEnd, valStart, transition);

			samplesToNextEvent--;
		}
	};

}
