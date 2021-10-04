#pragma once
#include "Node.h"

namespace OmedaNodes
{

	class Max : public Node
	{
	public:
		void Generate() override;
		~Max() = default;
	};

}