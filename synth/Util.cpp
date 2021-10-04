#include "Util.h"
#include "windows.h"
#include <climits>
#include <cmath>

using namespace OmedaCommon;

double Util::CurrentSampleRate = 44100;
float Util::CurrentTempo = 0;
int Util::CurrentPpq = 0;
int Util::RandomSeed = 0;

float Util::RandFloat()
{
	return rand() / (float)RAND_MAX;
}

float Util::NoteToFreq(float note)
{
	return 440 * powf(2, (note - 69) / 12);
}

double Util::FreqToNote(double freq)
{
	return 69 + 12 * log2(freq / 440);
}

float Util::Mix(float v1, float v2, float mix)
{
	return v1 + (v2 - v1) * mix;
}

float Util::Clamp(float f, float min, float max)
{
	return f < min ? min : f > max ? max : f;
}

short Util::ClampToShort(float f)
{
	return (short)(f < -SHRT_MAX ? -SHRT_MAX : f > SHRT_MAX ? SHRT_MAX : f);
}

float Util::Min(float a, float b)
{
	if (a != a) return b;
	if (b != b) return a;
	return a < b ? a : b;
}


float Util::ResonanceToQ(float resonance)
{
	return 1 / (2 * (1 - resonance));
}

float Util::QToResonance(float q)
{
	return 1 - 1 / (2 * q);
}

float Util::Sgn(float in)
{
	return (0 < in) - (in < 0);
}

float Util::LinearToDb(float lin)
{
	return 20 * (lin > 0.00001f ? log10f(lin) : log10f(0.00001f));
}

float Util::DbToLinear(float db)
{
	return powf(10, db / 20);
}

int Util::BeatToSamples(int beat)
{
	return beat * CurrentSampleRate * 60.0 / CurrentTempo / CurrentPpq;
}


float Util::ParamToFrequency(float param)
{
	return powf(20000, param);
}

float Util::FrequencyToParam(float frequency)
{
	return logf(frequency) / (5 * logf(2) + 4 * logf(5));
}

float Util::ParamToQ(float param)
{
	return 1 / (-2 * param * 0.999 + 2);
}

float Util::QToParam(float q)
{
	return (1 - 1 / (2 * q)) / 0.999;
}

float Util::ParamToOctave(float param)
{
	return 6 * param - 3;
}

float Util::OctaveToParam(float octave)
{
	return (octave + 3) / 6;
}

float Util::ParamToWaveform(float param)
{
	return param * 4;
}

float Util::WaveformToParam(float waveform)
{
	return waveform / 4;
}

float Util::ParamToDetune(float param)
{
	return param - 0.5;
}

float Util::DetuneToParam(float detune)
{
	return detune + 0.5;
}

float Util::ParamToMs(float param)
{
	return param * 1000;
}

float Util::MsToParam(float ms)
{
	return ms / 1000;
}

float Util::ParamToVolume(float param)
{
	return powf(param, 2);
}

float Util::VolumeToParam(float volume)
{
	return sqrtf(volume);
}

float Util::ParamToDb(float param)
{
	return LinearToDb(param);
}

float Util::DbToParam(float db)
{
	return DbToLinear(db);
}


