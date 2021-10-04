#pragma once
#include "Node.h"

namespace OmedaNodes
{
	
	class DbToLinear : public Node
	{
	public:
		void Generate() override;
		~DbToLinear() = default;
	};

}