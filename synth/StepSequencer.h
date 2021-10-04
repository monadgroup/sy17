#pragma once
#include "Node.h"

namespace OmedaNodes
{

	class StepSequencer : public Node
	{
	public:
		void Generate() override;
		~StepSequencer() = default;
	};

}