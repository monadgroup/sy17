#pragma once
#include "Node.h"

namespace OmedaNodes
{
	
	class Delay : public Node
	{
		float delayTime;

		float *buffer = nullptr;
		int bufferSize = 0;
		int delaySize = 0;
		int currentPosition = 0;

		void setDelayTime(float newTime);

	public:
		Delay();
		~Delay();

		void Generate() override;
	};

}