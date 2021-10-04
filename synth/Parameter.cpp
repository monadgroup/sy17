#include "Parameter.h"
#include "InputData.h"

using namespace OmedaPlayer;

Parameter::Parameter(float *value, InputParameterData *data)
{
	this->value = value;

	eventCount = data->eventCount;
	eventDeltaTimes = data->eventDeltaTimes;
	eventValues = data->eventValues;
}
