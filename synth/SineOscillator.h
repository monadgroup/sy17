#pragma once
#include "CommonOscillator.h"

namespace OmedaNodes
{
	
	class SineOscillator : public CommonOscillator
	{
	public:
		void Generate() override;
		~SineOscillator() = default;
	};

}
