#pragma once
#include "Node.h"
#include "BiquadFilter.h"
#include "CommonBiquadFilter.h"

namespace OmedaNodes
{
	
	class BiquadLowPassFilter : public CommonBiquadFilter
	{
	public:
		BiquadLowPassFilter();
		~BiquadLowPassFilter() = default;
	};

}
