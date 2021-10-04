#pragma once
#include "Node.h"

namespace OmedaNodes
{

	class NoteFreq : public Node
	{
	public:
		void Generate() override;
		~NoteFreq() = default;
	};

}