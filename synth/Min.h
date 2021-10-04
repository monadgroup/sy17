#pragma once
#include "Node.h"

namespace OmedaNodes
{

	class Min : public Node
	{
	public:
		void Generate() override;
		~Min() = default;
	};

}