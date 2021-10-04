#pragma once
#include "CommonOscillator.h"

namespace OmedaNodes
{

	class SquareOscillator : public CommonOscillator
	{
	public:
		void Generate() override;
		~SquareOscillator() = default;
	};

}
