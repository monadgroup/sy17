#pragma once
#include "CommonOscillator.h"

namespace OmedaNodes
{

	class TriangleOscillator : public CommonOscillator
	{
	public:
		void Generate() override;
		~TriangleOscillator() = default;
	};

}
