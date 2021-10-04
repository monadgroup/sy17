#pragma once
#include "Node.h"

namespace OmedaNodes
{
	
	class StateVariableFilter : public Node
	{
		float high = 0;
		float low = 0;
		float band = 0;
		float notch = 0;

	public:
		void Generate() override;
		~StateVariableFilter() = default;
	};

}