#pragma once
#include "Node.h"

namespace OmedaNodes
{

	class Power : public Node
	{
	public:
		void Generate() override;
		~Power() = default;
	};

}