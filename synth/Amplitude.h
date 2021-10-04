#pragma once
#include "Node.h"

namespace OmedaNodes
{

	class Amplitude : public Node
	{
		float a1 = 0, b0 = 0;
		float estimate = 0;

	public:
		Amplitude();

		void Generate() override;
		~Amplitude() = default;
	};

}