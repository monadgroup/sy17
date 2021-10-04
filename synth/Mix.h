#pragma once
#include "Node.h"

namespace OmedaNodes
{

	class Mix : public Node
	{
	public:
		void Generate() override;
		~Mix() = default;
	};

}