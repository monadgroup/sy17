#pragma once
#include "Node.h"

namespace OmedaNodes
{

	class TriggerEnvelope : public Node
	{
		enum class State
		{
			Attack,
			Decay,
			Sustain,
			Release,
			Finished
		};

		bool lastTrig = false;
		float pos = 0;
		float releaseValue = 0;
		float lastValue = 0;
		State state = State::Finished;

	public:
		void Generate() override;
		~TriggerEnvelope() = default;
	};

}