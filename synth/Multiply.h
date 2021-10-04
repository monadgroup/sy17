#pragma once
#include "Node.h"

namespace OmedaNodes
{

	class Multiply : public Node
	{
	public:
		void Generate() override;
		~Multiply() = default;
	};

}
