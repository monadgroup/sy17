#pragma once
#include "Node.h"

namespace OmedaNodes
{

	class Add2 : public Node
	{
	public:
		void Generate() override;
		~Add2() = default;
	};

}