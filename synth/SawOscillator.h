#pragma once
#include "CommonOscillator.h"

namespace OmedaNodes
{

	class SawOscillator : public CommonOscillator
	{
	public:
		void Generate() override;
		~SawOscillator() = default;
	};

}
