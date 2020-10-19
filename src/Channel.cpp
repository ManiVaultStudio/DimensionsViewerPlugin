#include "Channel.h"
#include "DimensionsViewerPlugin.h"
#include "DimensionStatistics.h"

#include "PointData.h"

#include "util/Timer.h"

#include <QDebug>
#include <QVariantList>

DimensionsViewerPlugin* Channel::dimensionsViewerPlugin = nullptr;

Channel::Channel(QObject* parent, const QString& internalName, const QString& displayName, const bool& enabled, const QString& datasetName, const QString& dataName, const QColor& color) :
	QObject(parent),
	_internalName(internalName),
	_displayName(displayName),
	_enabled(enabled),
	_datasetName(),
	_dataName(dataName),
	_color(color),
	_profileType(ProfileType::Mean),
	_bandType(BandType::MinMax),
	_points(nullptr),
	_profile(),
	_band()
{
	setDatasetName(datasetName);

	QObject::connect(this, &Channel::datasetNameChanged, [this]() {
		computeStatistics();
	});

	QObject::connect(this, &Channel::profileTypeChanged, [this]() {
		computeStatistics();
	});

	QObject::connect(this, &Channel::bandTypeChanged, [this]() {
		computeStatistics();
	});

	QObject::connect(dimensionsViewerPlugin, &DimensionsViewerPlugin::pointsSelectionChanged, [this](const QString& dataName) {
		if (dataName == _dataName && !isSubset())
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
	
	std::vector<std::uint32_t> pointIndices;
	
	if (isSubset()) {
		pointIndices = _points->indices;
	}
	else {
		if (selection.indices.size() > 0) {
			pointIndices = selection.indices;
		}
		else {
			pointIndices.resize(_points->getNumPoints());
			std::iota(pointIndices.begin(), pointIndices.end(), 0);
		}
	}

	std::vector<std::uint32_t> dimensionIndices;

	dimensionIndices.resize(_points->getNumDimensions());
	std::iota(dimensionIndices.begin(), dimensionIndices.end(), 0);

	QVariantList payload;
	
	if (!pointIndices.empty()) {
		_points->visitSourceData([this, &pointIndices, &dimensionIndices, &payload](auto pointData) {
			std::vector<float> dimensionValues;

			dimensionValues.resize(pointIndices.size());

			for (const auto& dimensionIndex : dimensionIndices) {
				auto localPointIndex = 0;

				for (const auto& pointIndex : pointIndices) {
					dimensionValues[localPointIndex] = pointData[dimensionIndex][pointIndex];
					localPointIndex++;
				}

				/*for (const auto& pointValue : pointData[dimensionIndex]) {
					dimensionValues[localPointIndex] = pointValue;
					localPointIndex++;
				}*/

				QVariantMap dimension;

				dimension["id"] = dimensionIndex;
				dimension["name"] = _points->getDimensionNames().at(dimensionIndex);

				const float sum = std::accumulate(dimensionValues.begin(), dimensionValues.end(), 0.0);
				const float mean = sum / dimensionValues.size();

				std::vector<float> diff(dimensionValues.size());

				std::transform(dimensionValues.begin(), dimensionValues.end(), diff.begin(), [mean](double x) { return x - mean; });

				switch (_profileType)
				{
					case ProfileType::Mean: {
						dimension["mean"] = sum / dimensionValues.size();
						break;
					}

					case ProfileType::Median: {
						std::sort(dimensionValues.begin(), dimensionValues.end());
						dimension["median"] = dimensionValues[static_cast<int>(floorf(dimensionValues.size() / 2))];
						break;
					}

					default:
						break;
				}

				payload.append(dimension);
			}
		});
	}
	
	emit dimensionsChanged(_internalName, payload);
}

bool Channel::isSubset() const
{
	if (_points == nullptr)
		return false;

	return !_points->indices.empty();
}