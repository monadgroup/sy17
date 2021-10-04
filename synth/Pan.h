#pragma once
#include "Node.h"

namespace OmedaNodes
{
	
	class Pan : public Node
	{
	public:
		void Generate() override;
		~Pan() = default;
	};

}
