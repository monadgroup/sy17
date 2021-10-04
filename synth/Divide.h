#pragma once
#include "Node.h"

namespace OmedaNodes
{

	class Divide : public Node
	{
	public:
		void Generate() override;
		~Divide() = default;
	};

}