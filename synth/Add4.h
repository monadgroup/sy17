#pragma once
#include "Node.h"

namespace OmedaNodes
{

	class Add4 : public Node
	{
	public:
		void Generate() override;
		~Add4() = default;
	};

}