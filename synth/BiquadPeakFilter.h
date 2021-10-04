#pragma once
#include "Node.h"
#include "BiquadFilter.h"
#include "CommonBiquadFilter.h"

namespace OmedaNodes
{

	class BiquadPeakFilter : public CommonBiquadFilter
	{
	public:
		BiquadPeakFilter();
		~BiquadPeakFilter() = default;
	};

}
