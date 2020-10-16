#include "Channel.h"
#include "DimensionsViewerPlugin.h"
#include "DimensionStatistics.h"

#include "PointData.h"

#include "util/Timer.h"

#include <QDebug>
#include <QVariantList>

DimensionsViewerPlugin* Channel::dimensionsViewerPlugin = nullptr;

Channel::Channel(QObject* parent, const QString& name, const bool& enabled, const QString& datasetName, const QString& dataName, const QColor& color) :
	QObject(parent),
	_name(name),
	_enabled(enabled),
	_datasetName(datasetName),
	_dataName(dataName),
	_color(color),
	_profileType(ProfileType::Average),
	_bandType(BandType::MinMax),
	_points(nullptr),
	_profile(),
	_band()
{
	QObject::connect(this, &Channel::datasetNameChanged, [this]() {
		computeStatistics();
	});

	QObject::connect(this, &Channel::profileTypeChanged, [this]() {
		computeStatistics();
	});

	QObject::connect(this, &Channel::bandTypeChanged, [this]() {
		computeStatistics();
	});
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

	_points = &dynamic_cast<Points&>(dimensionsViewerPlugin->getCore()->requestData(_datasetName));

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

void Channel::computeStatistics()
{
	if (_points == nullptr)
		return;

	qDebug() << "Updating dimensions for" << _points->getName();

	const auto& selection = dynamic_cast<Points&>(dimensionsViewerPlugin->getCore()->requestSelection(_points->getDataName()));
	
	const auto noselectedpoints = selection.indices.size();
	const auto hasselection		= noselectedpoints > 0;
	const auto nopoints			= hasselection ? noselectedpoints : _points->getNumPoints();
	
	std::vector<std::uint32_t> pointIndices;
	
	if (hasselection) {
		pointIndices = selection.indices;
	}
	else {
		/*
		pointindices.resize(points->getnumpoints());
		std::iota(pointindices.begin(), pointindices.end(), 0);
		*/
	}

	std::vector<std::uint32_t> dimensionIndices;

	dimensionIndices.resize(_points->getNumDimensions());
	std::iota(dimensionIndices.begin(), dimensionIndices.end(), 0);

	QVector<DimensionStatistics> statistics;

	statistics.resize(_points->getNumDimensions());

	/*
	_points->visitData([this, &pointIndices, &dimensionIndices, &statistics](auto pointData) {
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
		dimension["name"]	= _points->getDimensionNames().at(dimensionIndex);
		dimension["min"]	= statistic.getMin();
		dimension["max"]	= statistic.getMax();
		dimension["avg"]	= statistic.getAverage();

		payload.append(dimension);

		dimensionIndex++;
	}
	*/
}