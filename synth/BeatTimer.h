#pragma once
#include "Node.h"

namespace OmedaNodes
{
	
	class BeatTimer : public Node
	{
		float time = 0;

	public:
		void Generate() override;
		void NoteOn() override;
		~BeatTimer() = default;
	};

}