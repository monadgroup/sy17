#pragma once
#include "Node.h"
#include "BiquadFilter.h"

namespace OmedaNodes
{
	
	class CommonBiquadFilter : public Node
	{
		OmedaDSP::BiquadFilter filter = OmedaDSP::BiquadFilter(OmedaDSP::BiquadFilter::Type::Low);

	public:
        explicit CommonBiquadFilter(OmedaDSP::BiquadFilter::Type type);

		void Generate() override;
	};

}
