#include "Dimensions.h"

#include "PointData.h"

#include <QDebug>
#include <QList>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>

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

	auto dimensions = QVector<Values>();

	dimensions.resize(points->getNumDimensions());
	
	for (auto& dimension : dimensions) {
		dimension.resize(noPoints);
	}

	points->visitData([this, &pointIndices, &dimensionIndices, &dimensions](auto pointData) {
		auto localPointIndex = 0;

		for (const auto& pointIndex : pointIndices) {
			for (const auto& dimensionIndex : dimensionIndices) {
				dimensions[dimensionIndex][localPointIndex] = pointData[pointIndex][dimensionIndex];
			}

			localPointIndex++;
		}
	});

	auto jsonValues = QJsonArray();

	auto dimensionIndex = 0;

	for (auto& dimension : dimensions) {
		std::sort(dimension.begin(), dimension.end());

		const auto minMax		= std::minmax_element(dimension.begin(), dimension.end());
		const auto min			= *minMax.first;
		const auto max			= *minMax.second;
		const auto sum			= std::accumulate(dimension.begin(), dimension.end(), 0);
		const auto average		= sum / static_cast<float>(noPoints);
		const auto centerIndex	= static_cast<int>(floorf(dimension.size() / 2));
		const auto mean			= dimension[centerIndex];

		auto jsonObject = QJsonObject();

		jsonObject.insert("id", dimensionIndex);
		jsonObject.insert("name", points->getDimensionNames().at(dimensionIndex));
		jsonObject.insert("min", min);
		jsonObject.insert("max", max);
		jsonObject.insert("avg", average);
		jsonObject.insert("mean", mean);

		jsonValues.push_back(jsonObject);

		dimensionIndex++;
	}

	const auto data = QString(QJsonDocument(jsonValues).toJson(QJsonDocument::Indented));

	//qDebug() << data;

	emit changed(data);
}