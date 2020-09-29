#pragma once

#include <QObject>
#include <QVector>

/**
 *
 */
class DimensionStatistics {

public: // Construction

	/** Default constructor */
	DimensionStatistics();

public: // Miscellaneous

	/** Add value */
	void addValue(const float& value);
		
public: // Getters

	float getMin() { return _min; };
	float getMax() { return _max; };
	float getAverage() { return _sum / static_cast<float>(_numValues); };

private:
	float			_min;				/** Minimum of all values */
	float			_max;				/** Maximum of all values */
	float			_sum;				/** Sum of all values */
	std::int32_t	_numValues;			/** Number of values processed so far */
};