#include "Instrument.h"

using namespace Omeda;

/*Instrument::~Instrument()
{
	Uninstall();
}*/

void Instrument::Uninstall()
{
	if (!isInstalled) return;

	delete[] buffer;
	buffer = nullptr;

	for (auto i = 0; i < nodeCount; i++)
	{
		delete nodes[i];
	}
	delete[] nodes;
	nodes = nullptr;

	isInstalled = false;
}

void Instrument::Install(float* b, int bs, int nc, Node** n)
{
	buffer = b;
	bufferSize = bs;
	nodeCount = nc;
	nodes = n;

	isInstalled = true;
}

void Instrument::Generate()
{
	for (auto i = 0; i < nodeCount; i++)
	{
		nodes[i]->Generate();
	}
}

void Instrument::NoteOn()
{
	for (auto i = 0; i < nodeCount; i++)
	{
		nodes[i]->NoteOn();
	}
}

