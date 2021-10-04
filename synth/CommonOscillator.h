#pragma once
#include "Node.h"

namespace OmedaNodes
{
	
	class CommonOscillator : public Node
	{
	protected:
		float phase = 0;

	public:
		void Generate() override;
		void NoteOn() override;
	};

}