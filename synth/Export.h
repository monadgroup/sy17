#pragma once
#include "InputData.h"
#include "AllNodes.h"

extern const int sampleRate;
extern const int tempo;
extern const int ppq;

extern const float staticBuffer[];

extern const int trackCount;
extern const OmedaPlayer::InputTrackData tracks[];

OmedaNodes::Node *CreateNodeFromType(unsigned char type);