#pragma once
#include "Node.h"

namespace OmedaNodes
{

	class Modulo : public Node
	{
	public:
		void Generate() override;
		~Modulo() = default;
	};

}