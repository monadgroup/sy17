#pragma once
#include "Node.h"

namespace OmedaNodes
{
	
	class LinearToDb : public Node
	{
	public:
		void Generate() override;
		~LinearToDb() = default;
	};

}