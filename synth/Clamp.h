#pragma once
#include "Node.h"

namespace OmedaNodes
{
	
	class Clamp : public Node
	{
	public:
		void Generate() override;
		~Clamp() = default;
	};

}