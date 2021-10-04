#pragma once

namespace OmedaDSP
{
	class BiquadFilter
	{
	public:
		enum class Type
		{
			Low,
			High,
			Peak
		};

	private:
		bool recalculate;

		Type type;
		float freq;
		float q;
		float gain;

		float a0, a1, a2, b1, b2;

		float z1, z2;

	public:
		BiquadFilter(Type type);

		float Run(float input);

		void SetType(Type t)
		{
			if (type == t) return;
			type = t;
			recalculate = true;
		}

		void SetFreq(float f)
		{
			if (freq == f) return;
			freq = f;
			recalculate = true;
		}

		void SetQ(float q)
		{
			if (this->q == q) return;
			this->q = q;
			recalculate = true;
		}

		void SetGain(float g)
		{
			if (gain == g) return;
			gain = g;
			recalculate = true;
		}

		Type GetType() const
		{
			return type;
		}

		float GetFreq() const
		{
			return freq;
		}

		float GetQ() const
		{
			return q;
		}

		float GetGain() const
		{
			return gain;
		}
	};

}