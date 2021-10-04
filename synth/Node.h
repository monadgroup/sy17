#pragma once

namespace OmedaNodes
{
	
	class Node
	{
	public:
		float **inputs = nullptr;
		float **outputs = nullptr;

		virtual ~Node();
		virtual void Generate();
		virtual void NoteOn();
	};

}
