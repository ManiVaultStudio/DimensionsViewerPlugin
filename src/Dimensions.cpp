#include "Dimensions.h"
#include "DimensionStatistics.h"

#include "PointData.h"

#include "util/Timer.h"

#include <QDebug>
#include <QList>
#include <QVariantList>

Dimensions::Dimensions(DimensionsViewerPlugin* dimensionsViewerPlugin) :
	QObject(),
	_dimensionsViewerPlugin(dimensionsViewerPlugin)
{
}

void Dimensions::update(Points* points, const std::vector<std::uint32_t>& selectedIndices /*= std::vector<std::uint32_t>()*/)
{
	qDebug() << "Updating dimensions for" << points->getName() << "with selection" << selectedIndices.size();

	const auto noSelectedPoints = selectedIndices.size();
	const auto hasSelection		= noSelectedPoints > 0;
	const auto noPoints			= hasSelection ? noSelectedPoints : points->getNumPoints();
	
	std::vector<std::uint32_t> pointIndices;
	
	if (hasSelection) {
		pointIndices = selectedIndices;
	}
	else {
		/*
		pointIndices.resize(points->getNumPoints());
		std::iota(pointIndices.begin(), pointIndices.end(), 0);
		*/
	}

	std::vector<std::uint32_t> dimensionIndices;

	dimensionIndices.resize(points->getNumDimensions());
	std::iota(dimensionIndices.begin(), dimensionIndices.end(), 0);

	QVector<DimensionStatistics> statistics;

	statistics.resize(points->getNumDimensions());

	points->visitData([this, &pointIndices, &dimensionIndices, &statistics](auto pointData) {
		auto localPointIndex = 0;

		for (const auto& pointIndex : pointIndices) {
			for (const auto& dimensionIndex : dimensionIndices) {
				statistics[dimensionIndex].addValue(pointData[pointIndex][dimensionIndex]);
			}

			localPointIndex++;
		}
	});

	QVariantList payload;

	auto dimensionIndex = 0;

	for (auto& statistic : statistics) {
		QVariantMap dimension;

		dimension["id"]		= dimensionIndex;
		dimension["name"]	= points->getDimensionNames().at(dimensionIndex);
		dimension["min"]	= statistic.getMin();
		dimension["max"]	= statistic.getMax();
		dimension["avg"]	= statistic.getAverage();

		payload.append(dimension);

		dimensionIndex++;
	}

	emit changed(payload);
}