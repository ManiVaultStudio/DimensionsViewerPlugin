#include "Channel.h"
#include "DimensionsViewerPlugin.h"
#include "DimensionStatistics.h"

#include "PointData.h"

#include "util/Timer.h"

#include <QDebug>
#include <QVariantList>

Channel::Channel(QObject* parent, const QString& name, const bool& enabled, const QString& datasetName, const QString& dataName, const QColor& color) :
	QObject(parent),
	_name(name),
	_enabled(enabled),
	_datasetName(datasetName),
	_dataName(dataName),
	_color(color),
	_profileType(ProfileType::Average),
	_bandType(BandType::MinMax)
{
}

void Channel::setEnabled(const bool& enabled)
{
	if (enabled == _enabled)
		return;

	_enabled = enabled;

	emit enabledChanged(_enabled);
}

void Channel::setDatasetName(const QString& datasetName)
{
	if (datasetName == _datasetName)
		return;

	_datasetName = datasetName;

	emit datasetNameChanged(_datasetName);
}

void Channel::setColor(const QColor& color)
{
	if (color == _color)
		return;

	_color = color;

	emit colorChanged(_color);
}

void Channel::setProfileType(const ProfileType& profileType)
{
	if (profileType == _profileType)
		return;

	_profileType = profileType;

	emit profileTypeChanged(_profileType);
}

void Channel::setBandType(const BandType& bandType)
{
	if (bandType == _bandType)
		return;

	_bandType = bandType;

	emit bandTypeChanged(_bandType);
}

//Channel::Channel(const QString& name, DimensionsViewerPlugin* dimensionsViewerPlugin) :
//	QObject(reinterpret_cast<QObject*>(dimensionsViewerPlugin)),
//	_name(name),
//	_dimensionsViewerPlugin(dimensionsViewerPlugin),
//	_dataSetName(),
//	_points(nullptr)
//{
//}
//
//void Channel::update(Points* points, const std::vector<std::uint32_t>& selectedIndices /*= std::vector<std::uint32_t>()*/)
//{
//	qDebug() << "Updating dimensions for" << points->getName() << "with selection" << selectedIndices.size();
//
//	const auto noSelectedPoints = selectedIndices.size();
//	const auto hasSelection		= noSelectedPoints > 0;
//	const auto noPoints			= hasSelection ? noSelectedPoints : points->getNumPoints();
//	
//	std::vector<std::uint32_t> pointIndices;
//	
//	if (hasSelection) {
//		pointIndices = selectedIndices;
//	}
//	else {
//		/*
//		pointIndices.resize(points->getNumPoints());
//		std::iota(pointIndices.begin(), pointIndices.end(), 0);
//		*/
//	}
//
//	std::vector<std::uint32_t> dimensionIndices;
//
//	dimensionIndices.resize(points->getNumDimensions());
//	std::iota(dimensionIndices.begin(), dimensionIndices.end(), 0);
//
//	QVector<DimensionStatistics> statistics;
//
//	statistics.resize(points->getNumDimensions());
//
//	points->visitData([this, &pointIndices, &dimensionIndices, &statistics](auto pointData) {
//		auto localPointIndex = 0;
//
//		for (const auto& pointIndex : pointIndices) {
//			for (const auto& dimensionIndex : dimensionIndices) {
//				statistics[dimensionIndex].addValue(pointData[pointIndex][dimensionIndex]);
//			}
//
//			localPointIndex++;
//		}
//	});
//
//	QVariantList payload;
//
//	auto dimensionIndex = 0;
//
//	for (auto& statistic : statistics) {
//		QVariantMap dimension;
//
//		dimension["id"]		= dimensionIndex;
//		dimension["name"]	= points->getDimensionNames().at(dimensionIndex);
//		dimension["min"]	= statistic.getMin();
//		dimension["max"]	= statistic.getMax();
//		dimension["avg"]	= statistic.getAverage();
//
//		payload.append(dimension);
//
//		dimensionIndex++;
//	}
//
//	emit changed(payload);
//}
//
//void Channel::setDataSetName(const QString& dataSetName)
//{
//	_dataSetName = dataSetName;
//
//	_points = &dynamic_cast<Points&>(_dimensionsViewerPlugin->getCore()->requestData(dataSetName));
//
//	//_dimensions.update(_points, selectedIndices());
//}
//
//QString Channel::getDataSetName() const
//{
//	return _dataSetName;
//}
//
//std::vector<std::uint32_t> Channel::selectedIndices() const
//{
//	if (_points == nullptr)
//		return std::vector<std::uint32_t>();
//
//	const auto& selection = dynamic_cast<Points&>(_dimensionsViewerPlugin->getCore()->requestSelection(_points->getDataName()));
//
//	return selection.indices;
////////}