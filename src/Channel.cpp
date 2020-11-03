#include "Channel.h"
#include "DimensionsViewerPlugin.h"
#include "Configuration.h"

#include "PointData.h"

#include "util/Timer.h"

#include <QDebug>
#include <QVariantList>

DimensionsViewerPlugin* Channel::dimensionsViewerPlugin = nullptr;

Channel::Channel(QObject* parent, const std::uint32_t& index, const QString& displayName, const bool& enabled, const QString& datasetName, const QString& dataName, const QColor& color, const float& opacity /*= 1.0f*/) :
	QObject(parent),
    _configuration(dynamic_cast<Configuration*>(parent)),
	_index(index),
	_internalName(QString("channel%1").arg(QString::number(index))),
	_displayName(displayName),
	_enabled(enabled),
	_datasetName(),
	_dataName(dataName),
	_color(color),
	_opacity(opacity),
	_profile(Profile::ProfileType::Mean),
	_spec(),
    _points(nullptr)
{
	setDatasetName(datasetName);
}

void Channel::setDisplayName(const QString& displayName)
{
    _displayName = displayName;
}

void Channel::setEnabled(const bool& enabled)
{
	if (enabled == _enabled)
		return;

	_enabled = enabled;

	updateSpec();
}

void Channel::setDatasetName(const QString& datasetName)
{
	if (datasetName == _datasetName)
		return;

	_datasetName = datasetName;

	_points = &dynamic_cast<Points&>(dimensionsViewerPlugin->getCore()->requestData(_datasetName));

	updateSpec();
}

void Channel::setColor(const QColor& color)
{
	if (color == _color)
		return;

	_color = color;

	updateSpec();
}

void Channel::setOpacity(const float& opacity)
{
	if (opacity == _opacity)
		return;

	_opacity = opacity;

	updateSpec();
}

Profile::ProfileType Channel::getProfileType() const
{
    auto& globalSettings = _configuration->getGlobalSettings();

    if (globalSettings.getEnabled(Qt::EditRole).toBool())
        return static_cast<Profile::ProfileType>(globalSettings.getProfileType(Qt::EditRole).toInt());

    return _profile.getProfileType();
}

void Channel::setProfileType(const Profile::ProfileType& profileType)
{
	if (profileType == _profile.getProfileType())
		return;

	_profile.setProfileType(profileType);
	
	updateSpec();
}

Profile::RangeType Channel::getRangeType() const
{
    auto& globalSettings = _configuration->getGlobalSettings();

    if (globalSettings.getEnabled(Qt::EditRole).toBool())
        return static_cast<Profile::RangeType>(globalSettings.getRangeType(Qt::EditRole).toInt());

    return _profile.getRangeType();
}

void Channel::setRangeType(const Profile::RangeType& rangeType)
{
	if (rangeType == _profile.getRangeType())
		return;

	_profile.setRangeType(rangeType);

	updateSpec();
}

bool Channel::canDisplay() const
{
    if (!_enabled)
        return false;

    return _profile.canDisplay();
}

std::int32_t Channel::getNoDimensions() const
{
    return _points->getNumDimensions();
}

std::int32_t Channel::getNoPoints() const
{
    return _points->getNumPoints();
}

bool Channel::isSubset() const
{
	if (_points == nullptr)
		return false;

	return !_points->indices.empty();
}

void Channel::updateSpec()
{
	if (_points == nullptr)
		return;

	//qDebug() << "Updating dimensions for" << _points->getName();

	const auto& selection = dynamic_cast<Points&>(dimensionsViewerPlugin->getCore()->requestSelection(_points->getDataName()));
	
	std::vector<std::uint32_t> pointIndices;
	
	if (isSubset()) {
		pointIndices.resize(_points->indices.size());
		std::iota(pointIndices.begin(), pointIndices.end(), 0);
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
	
	QVariantList dimensions;

    std::vector<float> dimensionValues;

    dimensionValues.resize(pointIndices.size());

	if (_enabled && !pointIndices.empty()) {
		_points->visitSourceData([this, &pointIndices, &dimensionIndices, &dimensions, &dimensionValues](auto& pointData) {
			for (const auto& dimensionIndex : dimensionIndices) {
				auto localPointIndex = 0;

				for (const auto& pointIndex : pointIndices) {
					dimensionValues[localPointIndex] = pointData[pointIndex][dimensionIndex];
					localPointIndex++;
				}

				QVariantMap dimension;

				dimension["chn"]		= _index;
				dimension["dimId"]		= dimensionIndex;
				dimension["dimName"]	= _points->getDimensionNames().at(dimensionIndex);

                const float sum     = std::accumulate(dimensionValues.begin(), dimensionValues.end(), 0.0);
				const float mean    = sum / dimensionValues.size();

				std::vector<float> diff(dimensionValues.size());

				std::transform(dimensionValues.begin(), dimensionValues.end(), diff.begin(), [mean](double x) { return x - mean; });

				switch (_profile.getProfileType())
				{
					case Profile::ProfileType::Mean: {
						dimension["agg"] = mean;
						break;
					}

					case Profile::ProfileType::Median: {
						std::sort(dimensionValues.begin(), dimensionValues.end());
						dimension["agg"] = dimensionValues[static_cast<int>(floorf(dimensionValues.size() / 2))];
						break;
					}

					default:
						break;
				}

				switch (_profile.getRangeType())
				{
                    case Profile::RangeType::StandardDeviation1: {
						double sqSum	= std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
						double stdDev1	= std::sqrt(sqSum / dimensionValues.size());

						dimension["v1"] = mean - stdDev1;
						dimension["v2"] = mean + stdDev1;
						break;
					}

					case Profile::RangeType::StandardDeviation2: {
						double sqSum	= std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
						double stdDev2	= 2.0 * std::sqrt(sqSum / dimensionValues.size());

						dimension["v1"] = mean - stdDev2;
						dimension["v2"] = mean + stdDev2;
						break;
					}

                    case Profile::RangeType::MinMax: {
                        auto result = std::minmax_element(dimensionValues.begin(), dimensionValues.end());

                        dimension["min"] = *result.first;
                        dimension["max"] = *result.second;
                        break;
                    }

					default:
						break;
				}

				dimensions.append(dimension);
			}
		});
	}

	_spec["enabled"]		= _enabled;
	_spec["index"]			= _index;
	_spec["displayName"]    = _displayName;
	_spec["datasetName"]	= _datasetName;
	_spec["dimensions"]		= dimensions;
	_spec["color"]			= _color;
	_spec["opacity"]		= _opacity;
	_spec["profileType"]	= static_cast<int>(_profile.getProfileType());
	_spec["rangeType"]		= static_cast<int>(_profile.getRangeType());
	_spec["canDisplay"]		= canDisplay();

    emit specChanged(this);
}