#include "DimensionStatistics.h"

DimensionStatistics::DimensionStatistics() :
	_min(std::numeric_limits<float>::max()),
	_max(std::numeric_limits<float>::min()),
	_sum(0.f),
	_numValues(0)
{
}

void DimensionStatistics::addValue(const float& value)
{
	_min = std::min(_min, value);
	_max = std::max(_max, value);

	_sum += value;

	_numValues++;
}