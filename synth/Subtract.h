#pragma once
#include "Node.h"

namespace OmedaNodes
{

	class Subtract : public Node
	{
	public:
		void Generate() override;
		~Subtract() = default;
	};

}