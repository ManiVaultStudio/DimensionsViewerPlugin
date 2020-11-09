#include "Channel.h"
#include "DimensionsViewerPlugin.h"
#include "Configuration.h"
#include "Miscellaneous.h"
#include "Channels.h"

#include "PointData.h"

#include "util/Timer.h"

#include <QDebug>
#include <QVariantList>

const QMap<QString, Channel::Columns> Channel::columns = {
    { "Name", Channel::Columns::Name },
    { "Index", Channel::Columns::Index },
    { "Internal name", Channel::Columns::InternalName },
    { "Display name", Channel::Columns::DisplayName },
    { "Enabled", Channel::Columns::Enabled },
    { "Dataset names", Channel::Columns::DatasetNames },
    { "Dataset name", Channel::Columns::DatasetName },
    { "Color", Channel::Columns::Color },
    { "Opacity", Channel::Columns::Opacity },
    { "Profile types", Channel::Columns::ProfileTypes },
    { "Profile type", Channel::Columns::ProfileType },
    { "Range types", Channel::Columns::RangeTypes },
    { "Range type", Channel::Columns::RangeType },
    { "Settings", Channel::Columns::Settings }
};

const QMap<Channel::Columns, std::function<QVariant(Channel* channel)>> Channel::getEditRoles = {
    { Channel::Columns::Name, [](Channel* channel) {
        return channel->_name;
    }},
    { Channel::Columns::Index, [](Channel* channel) {
        return channel->_index;
    }},
    { Channel::Columns::InternalName, [](Channel* channel) {
        return channel->_internalName;
    }},
    { Channel::Columns::DisplayName, [](Channel* channel) {
        return channel->_displayName;
    }},
    { Channel::Columns::Enabled, [](Channel* channel) {
        return channel->_enabled;
    }},
    { Channel::Columns::DatasetNames, [](Channel* channel) {
        if (channel->_index == 0)
            return channel->getConfigurationsModel()->getDatasetNames();
        
        return channel->getChannels()->getConfiguration()->getSubsets();
    }},
    { Channel::Columns::DatasetName, [](Channel* channel) {
        return channel->_datasetName;
    }},
    { Channel::Columns::NoDimensions, [](Channel* channel) {
        return channel->getNoDimensions();
    }},
    { Channel::Columns::NoPoints, [](Channel* channel) {
        return channel->getNoPoints();
    }},
    { Channel::Columns::Color, [](Channel* channel) {
        return QVariant::fromValue(channel->_color);
    }},
    { Channel::Columns::Opacity, [](Channel* channel) {
        return channel->_opacity;
    }},
    { Channel::Columns::ProfileTypes, [](Channel* channel) {
        const auto global = channel->getChannels()->getConfiguration()->getGlobal();

        if (global->_enabled)
            return global->_profile.getProfileTypeNames();

        return channel->_profile.getProfileTypeNames();
    }},
    { Channel::Columns::ProfileType, [](Channel* channel) {
        const auto global = channel->getChannels()->getConfiguration()->getGlobal();

        if (global->_enabled) {
            const auto profileType = global->_profile.getProfileType();
            channel->_profile.setProfileType(profileType);
            return to_ul(profileType);
        }

        return to_ul(channel->_profile.getProfileType());
    }},
    { Channel::Columns::RangeTypes, [](Channel* channel) {
        const auto global = channel->getChannels()->getConfiguration()->getGlobal();

        if (global->_enabled)
            return global->_profile.getRangeTypeNames();

        return channel->_profile.getRangeTypeNames();
    }},
    { Channel::Columns::RangeType, [](Channel* channel) {
        const auto global = channel->getChannels()->getConfiguration()->getGlobal();

        if (global->_enabled) {
            const auto rangeType = global->_profile.getRangeType();
            channel->_profile.setRangeType(static_cast<Profile::RangeType>(rangeType));
            return to_ul(rangeType);
        }

        return to_ul(channel->_profile.getRangeType());
    }}
};

const QMap<Channel::Columns, std::function<QVariant(Channel* channel)>> Channel::getDisplayRoles = {
    { Channel::Columns::Name, [](Channel* channel) {
        return getEditRoles[Channel::Columns::Name](channel).toString();
    }},
    { Channel::Columns::Index, [](Channel* channel) {
        return QString::number(getEditRoles[Channel::Columns::Index](channel).toInt());
    }},
    { Channel::Columns::InternalName, [](Channel* channel) {
        return getEditRoles[Channel::Columns::InternalName](channel).toString();
    }},
    { Channel::Columns::DisplayName, [](Channel* channel) {
        return getEditRoles[Channel::Columns::DisplayName](channel).toString();
    }},
    { Channel::Columns::Enabled, [](Channel* channel) {
        return getEditRoles[Channel::Columns::Enabled](channel).toBool() ? "on" : "off";
    }},
    { Channel::Columns::DatasetNames, [](Channel* channel) {
        return getEditRoles[Channel::Columns::DatasetNames](channel).toStringList().join(", ");
    }},
    { Channel::Columns::DatasetName, [](Channel* channel) { 
        return getEditRoles[Channel::Columns::DatasetName](channel).toString();
    }},
    { Channel::Columns::NoDimensions, [](Channel* channel) {
        return QString::number(getEditRoles[Channel::Columns::NoDimensions](channel).toInt());
    }},
    { Channel::Columns::NoPoints, [](Channel* channel) {
        return QString::number(getEditRoles[Channel::Columns::NoPoints](channel).toInt());
    }},
    { Channel::Columns::Color, [](Channel* channel) {
        return getEditRoles[Channel::Columns::Color](channel).value<QColor>().name();
    }},
    { Channel::Columns::Opacity, [](Channel* channel) {
        return QString::number(getEditRoles[Channel::Columns::Opacity](channel).toFloat());
    }},
    { Channel::Columns::ProfileTypes, [](Channel* channel) {
        return getEditRoles[Channel::Columns::ProfileTypes](channel).toStringList().join(", ");
    }},
    { Channel::Columns::ProfileType, [](Channel* channel) {
        const auto profileType = getEditRoles[Channel::Columns::ProfileType](channel).toInt();
        return Profile::getProfileTypeName(static_cast<Profile::ProfileType>(profileType));
    }},
    { Channel::Columns::RangeType, [](Channel* channel) {
        const auto rangeType = getEditRoles[Channel::Columns::RangeType](channel).toInt();
        return Profile::getRangeTypeName(static_cast<Profile::RangeType>(rangeType));
    }},
    { Channel::Columns::RangeTypes, [](Channel* channel) {
        return getEditRoles[Channel::Columns::RangeTypes](channel).toStringList().join(", ");
    }}
};

const QMap<Channel::Columns, std::function<QModelIndexList(Channel* channel, const QModelIndex& index, const QVariant& value)>> Channel::setEditRoles = {
    { Channel::Columns::DisplayName, [](Channel* channel, const QModelIndex& index, const QVariant& value) {
        channel->_displayName = value.toString();
        return QModelIndexList();
    }},
    { Channel::Columns::Enabled, [](Channel* channel, const QModelIndex& index, const QVariant& value) {
        channel->_enabled = value.toBool();

        QModelIndexList affectedIndices;

        for (int column = to_ul(Channel::Columns::Start); column <= to_ul(Channel::Columns::End); column++)
            affectedIndices << index.siblingAtColumn(column);

        const auto differentialProfileIndex = channel->index(to_ul(Configuration::Rows::DifferentialProfile), 0, index.parent().parent());

        for (int column = to_ul(DifferentialProfile::Column::Start); column <= to_ul(DifferentialProfile::Column::End); column++)
            affectedIndices << differentialProfileIndex.siblingAtColumn(column);

        affectedIndices << index.parent().parent().siblingAtColumn(to_ul(Miscellaneous::Column::ShowDimensionNames));

        return affectedIndices;
    }},
    { Channel::Columns::DatasetNames, [](Channel* channel, const QModelIndex& index, const QVariant& value) {
        channel->_datasetNames = value.toStringList();

        QModelIndexList affectedIndices;

        for (int column = to_ul(Channel::Columns::Start); column <= to_ul(Channel::Columns::End); column++)
            affectedIndices << index.siblingAtColumn(column);

        return affectedIndices;
    }},
    { Channel::Columns::DatasetName, [](Channel* channel, const QModelIndex& index, const QVariant& value) {
        channel->_datasetName = value.toString();

        channel->resolvePoints();

        return QModelIndexList();
    }},
    { Channel::Columns::Color, [](Channel* channel, const QModelIndex& index, const QVariant& value) {
        channel->_color = value.value<QColor>();
        return QModelIndexList();
    }},
    { Channel::Columns::Opacity, [](Channel* channel, const QModelIndex& index, const QVariant& value) {
        channel->_opacity = value.toFloat();
        return QModelIndexList();
    }},
    { Channel::Columns::ProfileType, [](Channel* channel, const QModelIndex& index, const QVariant& value) {
        channel->_profile.setProfileType(Profile::getProfileTypeEnum(value.toString()));
        
        QModelIndexList affectedIndices;

        affectedIndices << index.siblingAtColumn(to_ul(Channel::Columns::RangeTypes));
        affectedIndices << index.siblingAtColumn(to_ul(Channel::Columns::RangeType));

        return affectedIndices;
    }},
    { Channel::Columns::RangeType, [](Channel* channel, const QModelIndex& index, const QVariant& value) {
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
    resolvePoints();
}

int Channel::columnCount() const 
{
    return ModelItem::maxNoColumns;
}

Qt::ItemFlags Channel::getFlags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = Qt::ItemIsEditable;

    const auto column           = static_cast<Columns>(index.column());
    const auto globalEnabled    = getChannels()->getConfiguration()->getGlobal()->_enabled;
    const auto noSubsets        = getChannels()->getConfiguration()->getSubsets().count();

    switch (column)
    {
        case Columns::Index:
        case Columns::InternalName:
        case Columns::DisplayName:
        {
            flags |= Qt::ItemIsEnabled;

            break;
        }

        case Columns::Enabled: {
            switch (_index)
            {
                case Channels::Rows::Channel1: {
                    flags |= Qt::ItemIsEnabled;

                    break;
                }

                case Channels::Rows::Channel2: {
                    if (noSubsets >= 1)
                        flags |= Qt::ItemIsEnabled;

                    break;
                }

                case Channels::Rows::Channel3: {
                    if (noSubsets >= 2)
                        flags |= Qt::ItemIsEnabled;

                    break;
                }
                    
                default:
                    break;
            }

            break;
        }
        
        case Columns::DatasetNames:
        case Columns::DatasetName:
        {
            switch (_index)
            {
                case Channels::Rows::Channel1: {
                    flags |= Qt::ItemIsEnabled;

                    break;
                }

                case Channels::Rows::Channel2: {
                    if (_enabled && noSubsets >= 1)
                        flags |= Qt::ItemIsEnabled;

                    break;
                }

                case Channels::Rows::Channel3: {
                    if (_enabled && noSubsets >= 2)
                        flags |= Qt::ItemIsEnabled;

                    break;
                }

                default:
                    break;
            }

            break;
        }

        case Columns::Color:
        case Columns::Opacity:
        {
            if (_enabled)
                flags |= Qt::ItemIsEnabled;

            break;
        }

        case Columns::ProfileType:
        {
            if (_enabled && !globalEnabled)
                flags |= Qt::ItemIsEnabled;

            break;
        }

        case Columns::RangeType:
        {
            // TODO
            if (_enabled && _profile.getProfileType() != Profile::ProfileType::None && _profile.getProfileType() != Profile::ProfileType::Differential && !globalEnabled)
                flags |= Qt::ItemIsEnabled;

            break;
        }

        case Columns::Settings:
        {
            if (_enabled)
                flags |= Qt::ItemIsEnabled;

            //flags &= ~Qt::ItemIsEditable;

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
            if (getEditRoles.contains(static_cast<Columns>(column)))
                return getEditRoles[static_cast<Columns>(column)](const_cast<Channel*>(this));

            break;
        }

        case Qt::DisplayRole:
        {
            if (getDisplayRoles.contains(static_cast<Columns>(column)))
                return getDisplayRoles[static_cast<Columns>(column)](const_cast<Channel*>(this));

            break;
        }

        default:
            break;
    }

    return QVariant();
}

QModelIndexList Channel::setData(const QModelIndex& index, const QVariant& value, const int& role)
{
    const auto column = static_cast<Columns>(index.column());

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

void Channel::resolvePoints()
{
    if (!_datasetName.isEmpty())
        _points = &dynamic_cast<Points&>(getCore()->requestData(_datasetName));
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