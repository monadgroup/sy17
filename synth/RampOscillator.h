#pragma once
#include "CommonOscillator.h"

namespace OmedaNodes
{

	class RampOscillator : public CommonOscillator
	{
	public:
		void Generate() override;
		~RampOscillator() = default;
	};

}
