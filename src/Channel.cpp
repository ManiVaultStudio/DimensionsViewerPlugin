#include "Channel.h"
#include "DimensionsViewerPlugin.h"
#include "Configuration.h"

#include "PointData.h"

#include "util/Timer.h"

#include <QDebug>
#include <QVariantList>

const QMap<QString, Channel::Column> Channel::columns = {
    { "Index", Channel::Column::Index },
    { "Internal name", Channel::Column::InternalName },
    { "Display name", Channel::Column::DisplayName },
    { "Enabled", Channel::Column::Enabled },
    { "Dataset names", Channel::Column::DatasetNames },
    { "Dataset name", Channel::Column::DatasetName },
    { "Data name", Channel::Column::DataName },
    { "Color", Channel::Column::Color },
    { "Opacity", Channel::Column::Opacity },
    { "Profile types", Channel::Column::ProfileTypes },
    { "Profile type", Channel::Column::ProfileType },
    { "Range types", Channel::Column::RangeTypes },
    { "Range type", Channel::Column::RangeType },
    { "Settings", Channel::Column::Settings }
};

const QMap<Channel::Column, std::function<QVariant(Channel* channel)>> Channel::getEditRoles = {
    { Channel::Column::Index, [](Channel* channel) { return channel->_index; }},
    { Channel::Column::InternalName, [](Channel* channel) { return channel->_internalName; }},
    { Channel::Column::DisplayName, [](Channel* channel) { return channel->_displayName; }},
    { Channel::Column::Enabled, [](Channel* channel) { return channel->_enabled; }},
    { Channel::Column::DatasetNames, [](Channel* channel) { return channel->_datasetNames; }},
    { Channel::Column::DatasetName, [](Channel* channel) { return channel->_datasetName; }},
    { Channel::Column::DataName, [](Channel* channel) { return channel->_dataName; }},
    { Channel::Column::Color, [](Channel* channel) { return QVariant::fromValue(channel->_color); }},
    { Channel::Column::Opacity, [](Channel* channel) { return channel->_opacity; }},
    { Channel::Column::ProfileTypes, [](Channel* channel) {
        return channel->_profile.getProfileTypeNames();
    }},
    { Channel::Column::ProfileType, [](Channel* channel) {
        if (channel->getGlobalSettings().getData(GlobalSettings::Column::Enabled, Qt::EditRole).toBool())
            return channel->getGlobalSettings().getData(GlobalSettings::Column::ProfileType, Qt::EditRole).toInt();

        return static_cast<int>(channel->_profile.getProfileType());
    }},
    { Channel::Column::RangeTypes, [](Channel* channel) {
        return channel->_profile.getRangeTypeNames();
    }},
    { Channel::Column::RangeType, [](Channel* channel) {
        if (channel->getGlobalSettings().getData(GlobalSettings::Column::Enabled, Qt::EditRole).toBool())
            return channel->getGlobalSettings().getData(GlobalSettings::Column::RangeType, Qt::EditRole).toInt();

        return static_cast<int>(channel->_profile.getRangeType());
    }}
};

const QMap<Channel::Column, std::function<QVariant(Channel* channel)>> Channel::getDisplayRoles = {
    { Channel::Column::Index, [](Channel* channel) { return QString::number(channel->_index); }},
    { Channel::Column::InternalName, [](Channel* channel) { return channel->_internalName; }},
    { Channel::Column::DisplayName, [](Channel* channel) { return channel->_displayName; }},
    { Channel::Column::Enabled, [](Channel* channel) { return channel->_enabled; }},
    { Channel::Column::DatasetNames, [](Channel* channel) { return channel->_datasetNames; }},
    { Channel::Column::DatasetName, [](Channel* channel) { return channel->_datasetName; }},
    { Channel::Column::DataName, [](Channel* channel) { return channel->_dataName; }},
    { Channel::Column::Color, [](Channel* channel) { return channel->_color; }},
    { Channel::Column::Opacity, [](Channel* channel) { return channel->_opacity; }},
    { Channel::Column::ProfileType, [](Channel* channel) {
        if (channel->getGlobalSettings().getData(GlobalSettings::Column::Enabled, Qt::EditRole).toBool())
            return channel->getGlobalSettings().getData(GlobalSettings::Column::ProfileType, Qt::DisplayRole).toString();

        return Profile::getProfileTypeName(channel->_profile.getProfileType());
    }},
    { Channel::Column::RangeType, [](Channel* channel) {
        if (channel->getGlobalSettings().getData(GlobalSettings::Column::Enabled, Qt::EditRole).toBool())
            return channel->getGlobalSettings().getData(GlobalSettings::Column::RangeType, Qt::DisplayRole).toString();

        return Profile::getRangeTypeName(channel->_profile.getRangeType());
    }}
};

const QMap<Channel::Column, std::function<QModelIndexList(Channel* channel, const QVariant& value, const QModelIndex& index)>> Channel::setEditRoles = {
    { Channel::Column::DisplayName, [](Channel* channel, const QVariant& value, const QModelIndex& index) {
        channel->_displayName = value.toString();
        return QModelIndexList();
    }},
    { Channel::Column::Enabled, [](Channel* channel, const QVariant& value, const QModelIndex& index) {
        channel->_enabled = value.toBool();

        QModelIndexList affectedIndices;

        affectedIndices << index.siblingAtColumn(Channel::Column::DatasetNames);
        affectedIndices << index.siblingAtColumn(Channel::Column::DatasetName);
        affectedIndices << index.siblingAtColumn(Channel::Column::DataName);
        affectedIndices << index.siblingAtColumn(Channel::Column::Color);
        affectedIndices << index.siblingAtColumn(Channel::Column::Opacity);
        affectedIndices << index.siblingAtColumn(Channel::Column::ProfileTypes);
        affectedIndices << index.siblingAtColumn(Channel::Column::ProfileType);
        affectedIndices << index.siblingAtColumn(Channel::Column::RangeTypes);
        affectedIndices << index.siblingAtColumn(Channel::Column::RangeType);
        affectedIndices << index.siblingAtColumn(Channel::Column::Settings);

        return affectedIndices;
    }},
    { Channel::Column::DatasetNames, [](Channel* channel, const QVariant& value, const QModelIndex& index) {
        channel->_datasetNames = value.toStringList();
        return QModelIndexList();
    }},
    { Channel::Column::DatasetName, [](Channel* channel, const QVariant& value, const QModelIndex& index) {
        channel->_datasetName = value.toString();

        if (!channel->_datasetName.isEmpty())
            channel->_points = &dynamic_cast<Points&>(dimensionsViewerPlugin->getCore()->requestData(channel->_datasetName));

        return QModelIndexList();
    }},
    { Channel::Column::DataName, [](Channel* channel, const QVariant& value, const QModelIndex& index) {
        channel->_dataName = value.toString();
        return QModelIndexList();
    }},
    { Channel::Column::Color, [](Channel* channel, const QVariant& value, const QModelIndex& index) {
        channel->_color = value.value<QColor>();
        return QModelIndexList();
    }},
    { Channel::Column::Opacity, [](Channel* channel, const QVariant& value, const QModelIndex& index) {
        channel->_opacity = value.toFloat();
        return QModelIndexList();
    }},
    { Channel::Column::ProfileType, [](Channel* channel, const QVariant& value, const QModelIndex& index) {
        channel->_profile.setProfileType(Profile::getProfileTypeEnum(value.toString()));
        
        QModelIndexList affectedIndices;

        affectedIndices << index.siblingAtColumn(Channel::Column::RangeTypes);
        affectedIndices << index.siblingAtColumn(Channel::Column::RangeType);

        return affectedIndices;
    }},
    { Channel::Column::RangeType, [](Channel* channel, const QVariant& value, const QModelIndex& index) {
        channel->_profile.setRangeType(Profile::getRangeTypeEnum(value.toString()));
        return QModelIndexList();
    }}
};

Channel::Channel(ModelItem* parent, const std::uint32_t& index, const QString& displayName, const bool& enabled, const QString& datasetName, const QString& dataName, const QColor& color, const float& opacity /*= 1.0f*/) :
    ModelItem(parent),
    _configuration(dynamic_cast<Configuration*>(parent)),
	_index(index),
	_internalName(QString("channel%1").arg(QString::number(index))),
	_displayName(displayName),
	_enabled(enabled),
    _datasetNames(),
	_datasetName(datasetName),
	_dataName(dataName),
	_color(color),
	_opacity(opacity),
	_profile(Profile::ProfileType::Mean),
	_spec(),
    _points(nullptr)
{
}

Qt::ItemFlags Channel::getFlags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = Qt::ItemIsSelectable;

    const auto column = static_cast<Column>(index.column());

    switch (column)
    {
        case Column::Index:
        case Column::InternalName:
        case Column::DisplayName:
            break;

        case Column::Enabled: {
            flags |= Qt::ItemIsEnabled;
            break;
        }
        
        case Column::DatasetNames:
            break;

        case Column::DatasetName:
        case Column::Color:
        case Column::Opacity:
        {
            if (_enabled)
                flags |= Qt::ItemIsEnabled;

            break;
        }

        case Column::ProfileType:
        {
            if (_enabled && !getGlobalSettings().getData(GlobalSettings::Column::Enabled, Qt::EditRole).toBool())
                flags |= Qt::ItemIsEnabled;

            break;
        }

        case Column::RangeType:
        {
            if (_enabled && _profile.getProfileType() != Profile::ProfileType::None && _profile.getProfileType() != Profile::ProfileType::Differential && !getGlobalSettings().getData(GlobalSettings::Column::Enabled, Qt::EditRole).toBool())
                flags |= Qt::ItemIsEnabled;

            break;
        }

        case Column::Settings:
        {
            if (_enabled)
                flags |= Qt::ItemIsEnabled;

            break;
        }

        case Column::DataName:
            break;

        default:
            break;
    }

    return flags;
}

QVariant Channel::getData(const QModelIndex& index, const int& role) const
{
    return getData(index.column(), role);
}

QVariant Channel::getData(const int& column, const int& role) const
{
    switch (role)
    {
        case Qt::EditRole:
        {
            if (getEditRoles.contains(static_cast<Column>(column)))
                return getEditRoles[static_cast<Column>(column)](const_cast<Channel*>(this));

            break;
        }

        case Qt::DisplayRole:
        {
            if (getDisplayRoles.contains(static_cast<Column>(column)))
                return getDisplayRoles[static_cast<Column>(column)](const_cast<Channel*>(this));

            break;
        }

        default:
            break;
    }

    return QVariant();
}

QModelIndexList Channel::setData(const QModelIndex& index, const QVariant& value, const int& role)
{
    const auto column = static_cast<Column>(index.column());

    QModelIndexList affectedIndices{ index };

    switch (role)
    {
        case Qt::EditRole:
        {
            if (setEditRoles.contains(column))
                affectedIndices << setEditRoles[column](const_cast<Channel*>(this), value, index);

            break;
        }

        default:
            break;
    }

    return affectedIndices;
}

ModelItem* Channel::getChild(const int& index) const
{
    return nullptr;
}

int Channel::getChildCount() const
{
    return 0;
}

int Channel::getChildIndex(ModelItem* child) const
{
    return 0;
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

Channels* Channel::getChannels() const
{
    return dynamic_cast<Channels*>(parent());
}

GlobalSettings& Channel::getGlobalSettings() const
{
    return getChannels()->getConfiguration()->getGlobalSettings();
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