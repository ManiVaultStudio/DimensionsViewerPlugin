#include "Channel.h"
#include "DimensionsViewerPlugin.h"
#include "Configuration.h"
#include "Channels.h"

#include "PointData.h"

#include "util/Timer.h"

#include <QDebug>
#include <QVariantList>

const QMap<QString, Channel::Column> Channel::columns = {
    { "Name", Channel::Column::Name },
    { "Index", Channel::Column::Index },
    { "Internal name", Channel::Column::InternalName },
    { "Display name", Channel::Column::DisplayName },
    { "Enabled", Channel::Column::Enabled },
    { "Dataset names", Channel::Column::DatasetNames },
    { "Dataset name", Channel::Column::DatasetName },
    { "Color", Channel::Column::Color },
    { "Opacity", Channel::Column::Opacity },
    { "Profile types", Channel::Column::ProfileTypes },
    { "Profile type", Channel::Column::ProfileType },
    { "Range types", Channel::Column::RangeTypes },
    { "Range type", Channel::Column::RangeType },
    { "Settings", Channel::Column::Settings }
};

const QMap<Channel::Column, std::function<QVariant(Channel* channel)>> Channel::getEditRoles = {
    { Channel::Column::Name, [](Channel* channel) {
        return channel->_name;
    }},
    { Channel::Column::Index, [](Channel* channel) {
        return channel->_index;
    }},
    { Channel::Column::InternalName, [](Channel* channel) {
        return channel->_internalName;
    }},
    { Channel::Column::DisplayName, [](Channel* channel) {
        return channel->_displayName;
    }},
    { Channel::Column::Enabled, [](Channel* channel) {
        return channel->_enabled;
    }},
    { Channel::Column::DatasetNames, [](Channel* channel) {
        if (channel->_index == 0)
            return channel->getConfigurationsModel()->getDatasetNames();
        
        return channel->getChannels()->getConfiguration()->getSubsets();
    }},
    { Channel::Column::DatasetName, [](Channel* channel) {
        return channel->_datasetName;
    }},
    { Channel::Column::Color, [](Channel* channel) {
        return QVariant::fromValue(channel->_color);
    }},
    { Channel::Column::Opacity, [](Channel* channel) {
        return channel->_opacity;
    }},
    { Channel::Column::ProfileTypes, [](Channel* channel) {
        const auto global = channel->getChannels()->getConfiguration()->getGlobal();

        if (global->_enabled)
            return global->_profile.getProfileTypeNames();

        return channel->_profile.getProfileTypeNames();
    }},
    { Channel::Column::ProfileType, [](Channel* channel) {
        const auto global = channel->getChannels()->getConfiguration()->getGlobal();

        if (global->_enabled) {
            const auto profileType = global->_profile.getProfileType();
            channel->_profile.setProfileType(profileType);
            return to_ul(profileType);
        }

        return to_ul(channel->_profile.getProfileType());
    }},
    { Channel::Column::RangeTypes, [](Channel* channel) {
        const auto global = channel->getChannels()->getConfiguration()->getGlobal();

        if (global->_enabled)
            return global->_profile.getRangeTypeNames();

        return channel->_profile.getRangeTypeNames();
    }},
    { Channel::Column::RangeType, [](Channel* channel) {
        const auto global = channel->getChannels()->getConfiguration()->getGlobal();

        if (global->_enabled) {
            const auto rangeType = global->_profile.getRangeType();
            channel->_profile.setRangeType(static_cast<Profile::RangeType>(rangeType));
            return to_ul(rangeType);
        }

        return to_ul(channel->_profile.getRangeType());
    }}
};

const QMap<Channel::Column, std::function<QVariant(Channel* channel)>> Channel::getDisplayRoles = {
    { Channel::Column::Name, [](Channel* channel) {
        return getEditRoles[Channel::Column::Name](channel).toString();
    }},
    { Channel::Column::Index, [](Channel* channel) {
        return QString::number(getEditRoles[Channel::Column::Index](channel).toInt());
    }},
    { Channel::Column::InternalName, [](Channel* channel) {
        return getEditRoles[Channel::Column::InternalName](channel).toString();
    }},
    { Channel::Column::DisplayName, [](Channel* channel) {
        return getEditRoles[Channel::Column::DisplayName](channel).toString();
    }},
    { Channel::Column::Enabled, [](Channel* channel) {
        return getEditRoles[Channel::Column::Enabled](channel).toBool() ? "on" : "off";
    }},
    { Channel::Column::DatasetNames, [](Channel* channel) {
        return getEditRoles[Channel::Column::DatasetNames](channel).toStringList().join(", ");
    }},
    { Channel::Column::DatasetName, [](Channel* channel) { 
        return getEditRoles[Channel::Column::DatasetName](channel).toString();
    }},
    { Channel::Column::Color, [](Channel* channel) {
        return getEditRoles[Channel::Column::Color](channel).value<QColor>().name();
    }},
    { Channel::Column::Opacity, [](Channel* channel) {
        return QString::number(getEditRoles[Channel::Column::Opacity](channel).toFloat());
    }},
    { Channel::Column::ProfileTypes, [](Channel* channel) {
        return getEditRoles[Channel::Column::ProfileTypes](channel).toStringList().join(", ");
    }},
    { Channel::Column::ProfileType, [](Channel* channel) {
        const auto profileType = getEditRoles[Channel::Column::ProfileType](channel).toInt();
        return Profile::getProfileTypeName(static_cast<Profile::ProfileType>(profileType));
    }},
    { Channel::Column::RangeType, [](Channel* channel) {
        const auto rangeType = getEditRoles[Channel::Column::RangeType](channel).toInt();
        return Profile::getRangeTypeName(static_cast<Profile::RangeType>(rangeType));
    }},
    { Channel::Column::RangeTypes, [](Channel* channel) {
        return getEditRoles[Channel::Column::RangeTypes](channel).toStringList().join(", ");
    }}
};

const QMap<Channel::Column, std::function<QModelIndexList(Channel* channel, const QModelIndex& index, const QVariant& value)>> Channel::setEditRoles = {
    { Channel::Column::DisplayName, [](Channel* channel, const QModelIndex& index, const QVariant& value) {
        channel->_displayName = value.toString();
        return QModelIndexList();
    }},
    { Channel::Column::Enabled, [](Channel* channel, const QModelIndex& index, const QVariant& value) {
        channel->_enabled = value.toBool();

        QModelIndexList affectedIndices;

        for (int column = to_ul(Channel::Column::Start); column <= to_ul(Channel::Column::End); column++)
            affectedIndices << index.siblingAtColumn(column);

        return affectedIndices;
    }},
    { Channel::Column::DatasetNames, [](Channel* channel, const QModelIndex& index, const QVariant& value) {
        channel->_datasetNames = value.toStringList();

        QModelIndexList affectedIndices;

        for (int column = to_ul(Channel::Column::Start); column <= to_ul(Channel::Column::End); column++)
            affectedIndices << index.siblingAtColumn(column);

        return affectedIndices;
    }},
    { Channel::Column::DatasetName, [](Channel* channel, const QModelIndex& index, const QVariant& value) {
        channel->_datasetName = value.toString();

        if (!channel->_datasetName.isEmpty())
            channel->_points = &dynamic_cast<Points&>(channel->getCore()->requestData(channel->_datasetName));

        return QModelIndexList();
    }},
    { Channel::Column::Color, [](Channel* channel, const QModelIndex& index, const QVariant& value) {
        channel->_color = value.value<QColor>();
        return QModelIndexList();
    }},
    { Channel::Column::Opacity, [](Channel* channel, const QModelIndex& index, const QVariant& value) {
        channel->_opacity = value.toFloat();
        return QModelIndexList();
    }},
    { Channel::Column::ProfileType, [](Channel* channel, const QModelIndex& index, const QVariant& value) {
        channel->_profile.setProfileType(Profile::getProfileTypeEnum(value.toString()));
        
        QModelIndexList affectedIndices;

        affectedIndices << index.siblingAtColumn(to_ul(Channel::Column::RangeTypes));
        affectedIndices << index.siblingAtColumn(to_ul(Channel::Column::RangeType));

        return affectedIndices;
    }},
    { Channel::Column::RangeType, [](Channel* channel, const QModelIndex& index, const QVariant& value) {
        channel->_profile.setRangeType(Profile::getRangeTypeEnum(value.toString()));
        return QModelIndexList();
    }}
};

Channel::Channel(ModelItem* parent, const std::uint32_t& index, const QString& displayName, const bool& enabled, const QString& datasetName, const QString& dataName, const QColor& color, const float& opacity /*= 1.0f*/) :
    ModelItem("Channel", parent),
    _configuration(dynamic_cast<Configuration*>(parent)),
	_index(index),
	_internalName(QString("channel%1").arg(QString::number(index))),
	_displayName(displayName),
	_enabled(enabled),
    _datasetNames(),
	_datasetName(datasetName),
	_color(color),
	_opacity(opacity),
	_profile(Profile::ProfileType::Mean),
	_spec(),
    _points(nullptr)
{
}

int Channel::columnCount() const 
{
    return ModelItem::maxNoColumns;
}

Qt::ItemFlags Channel::getFlags(const QModelIndex& index) const
{
    Qt::ItemFlags flags;

    const auto column           = static_cast<Column>(index.column());
    const auto globalEnabled    = getChannels()->getConfiguration()->getGlobal()->_enabled;
    const auto subsets          = getChannels()->getConfiguration()->getSubsets();

    switch (column)
    {
        case Column::Index:
        case Column::InternalName:
        case Column::DisplayName:
            break;

        case Column::Enabled: {
            switch (_index)
            {
                case Channels::Child::Channel1: {
                    flags |= Qt::ItemIsEnabled;

                    break;
                }

                case Channels::Child::Channel2: {
                    if (subsets.count() >= 1)
                        flags |= Qt::ItemIsEnabled;

                    break;
                }

                case Channels::Child::Channel3: {
                    if (subsets.count() >= 2)
                        flags |= Qt::ItemIsEnabled;

                    break;
                }
                    
                default:
                    break;
            }
            if (_index == 0) {
                
            }
            else {

            }
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
            if (_enabled && !globalEnabled)
                flags |= Qt::ItemIsEnabled;

            break;
        }

        case Column::RangeType:
        {
            // TODO
            if (_enabled && _profile.getProfileType() != Profile::ProfileType::None && _profile.getProfileType() != Profile::ProfileType::Differential && !globalEnabled)
                flags |= Qt::ItemIsEnabled;

            break;
        }

        case Column::Settings:
        {
            if (_enabled)
                flags |= Qt::ItemIsEnabled;

            break;
        }

        default:
            break;
    }

    return flags;
}

QVariant Channel::getData(const QModelIndex& index, const int& role) const
{
    const auto column = index.column();

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
                affectedIndices << setEditRoles[column](const_cast<Channel*>(this), index, value);

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

std::int32_t Channel::getNoDimensions() const
{
    return _points->getNumDimensions();
}

std::int32_t Channel::getNoPoints() const
{
    return _points->getNumPoints();
}

const Channels* Channel::getChannels() const
{
    return dynamic_cast<Channels*>(_parent);
}

bool Channel::canDisplay() const
{
    if (!_enabled)
        return false;

    return _profile.canDisplay();
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

	const auto& selection = dynamic_cast<Points&>(getCore()->requestSelection(_points->getDataName()));
	
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

QVariantMap Channel::getSpec()
{
    return _spec;
}