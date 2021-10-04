#pragma once

namespace OmedaCommon {

	class Util
	{
	public:
		static double CurrentSampleRate;
		static float CurrentTempo;
		static int CurrentPpq;
		static int RandomSeed;

		static float RandFloat();

		static float NoteToFreq(float note);
		static double FreqToNote(double freq);

		static float Mix(float v1, float v2, float mix);
		static float Clamp(float f, float min, float max);
		static short ClampToShort(float f);
		static float Min(float a, float b);

		static float ResonanceToQ(float resonance);
		static float QToResonance(float q);

		static float Sgn(float in);

		static float LinearToDb(float lin);
		static float DbToLinear(float db);

		static int BeatToSamples(int beat);

		// parameters
		static float ParamToFrequency(float param);
		static float FrequencyToParam(float frequency);

		static float ParamToQ(float param);
		static float QToParam(float q);

		static float ParamToOctave(float param);
		static float OctaveToParam(float octave);

		static float ParamToWaveform(float param);
		static float WaveformToParam(float waveform);

		static float ParamToDetune(float param);
		static float DetuneToParam(float detune);

		static float ParamToMs(float param);
		static float MsToParam(float ms);

		static float ParamToVolume(float param);
		static float VolumeToParam(float volume);

		static float ParamToDb(float param);
		static float DbToParam(float db);
	};

}