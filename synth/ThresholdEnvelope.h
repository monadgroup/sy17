#pragma once
#include "Node.h"

namespace OmedaNodes
{

	class ThresholdEnvelope : public Node
	{
		float level;

	public:
		void Generate() override;
		~ThresholdEnvelope() = default;
	};

}