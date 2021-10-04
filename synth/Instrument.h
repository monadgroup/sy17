#pragma once
#include "Node.h"

namespace Omeda
{
	class Instrument : public OmedaNodes::Node
	{
		bool isInstalled = false;

	public:
		bool wasActive = false;
		bool isActive = false;
		bool noteOn = false;
		float notePan = 0;
		float noteVol = 0;
		float notePitch = 0;
		int setTag = 0;

		float *buffer = nullptr;
		int bufferSize = 0;

		int nodeCount = 0;
		Node **nodes = nullptr;

		//~Instrument() override;

		void Install(float *b, int bs, int nc, Node **n);

		void Uninstall();

		void Generate() override;

		void NoteOn() override;
	};

}
