#pragma once
#include "Node.h"
#include "BiquadFilter.h"
#include "CommonBiquadFilter.h"

namespace OmedaNodes
{

	class BiquadHighPassFilter : public CommonBiquadFilter
	{
	public:
		BiquadHighPassFilter();
		~BiquadHighPassFilter() = default;
	};

}
