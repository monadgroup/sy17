#pragma once
#include "Node.h"

namespace OmedaNodes
{

	class Noise : public Node
	{
	public:
		void Generate() override;
		~Noise() = default;
	};

}