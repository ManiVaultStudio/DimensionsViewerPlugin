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
    { "Profile type", Channel::Column::ProfileType },
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
    { Channel::Column::Color, [](Channel* channel) { return channel->_color; }},
    { Channel::Column::Opacity, [](Channel* channel) { return channel->_opacity; }},
    { Channel::Column::ProfileType, [](Channel* channel) { return static_cast<int>(channel->_profile.getProfileType()); }},
    { Channel::Column::RangeType, [](Channel* channel) { return static_cast<int>(channel->_profile.getRangeType()); }}
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
    { Channel::Column::ProfileType, [](Channel* channel) { return Profile::getProfileTypeName(channel->_profile.getProfileType()); }},
    { Channel::Column::RangeType, [](Channel* channel) { return Profile::getRangeTypeName(channel->_profile.getRangeType()); }}
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
        affectedIndices << index.siblingAtColumn(Channel::Column::ProfileType);
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
        case Column::ProfileType:
        case Column::RangeType:
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
    const auto column = static_cast<Column>(index.column());

    switch (role)
    {
        case Qt::EditRole:
        {
            if (getDisplayRoles.contains(column))
                return getEditRoles[column](const_cast<Channel*>(this));

            break;
        }

        case Qt::DisplayRole:
        {
            if (getDisplayRoles.contains(column))
                return getDisplayRoles[column](const_cast<Channel*>(this));

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

    QModelIndexList affectedIndices;

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


//if (index.column() >= Column::ChannelEnabledStart && index.column() < Column::ChannelEnabledEnd) {
//    const auto channelIndex = index.column() - Column::ChannelEnabledStart;
//
//    if (channelIndex == 0) {
//        flags |= Qt::ItemIsEnabled;
//    }
//    else {
//        if (_subsets.size() >= channelIndex)
//            flags |= Qt::ItemIsEnabled;
//    }
//}
//
//if (index.column() >= Column::ChannelDatasetNameStart && index.column() < Column::ChannelDatasetNameEnd) {
//    const auto channelIndex = index.column() - Column::ChannelDatasetNameStart;
//
//    if (channelIndex == 0) {
//        flags |= Qt::ItemIsEnabled;
//    }
//    else {
//        if (_channels[channelIndex]->isEnabled() && _subsets.size() >= channelIndex)
//            flags |= Qt::ItemIsEnabled;
//    }
//}
//
//if (index.column() >= Column::ChannelColorStart && index.column() < Column::ChannelColorEnd) {
//    const auto channelIndex = index.column() - Column::ChannelColorStart;
//
//    if (channelIndex == 0) {
//        if (_channels[0]->isEnabled())
//            flags |= Qt::ItemIsEnabled;
//    }
//    else {
//        if (_channels[channelIndex]->isEnabled() && _subsets.size() >= channelIndex)
//            flags |= Qt::ItemIsEnabled;
//    }
//}
//
//if (index.column() >= Column::ChannelProfileTypeStart && index.column() < Column::ChannelProfileTypeEnd) {
//    const auto channelIndex = index.column() - Column::ChannelProfileTypeStart;
//
//    if (_channels[channelIndex]->isEnabled() && !_globalSettings && _subsets.size() >= channelIndex)
//        flags |= Qt::ItemIsEnabled;
//}
//
//if (index.column() >= Column::ChannelRangeTypeStart && index.column() < Column::ChannelRangeTypeEnd) {
//    const auto channelIndex = index.column() - Column::ChannelRangeTypeStart;
//
//    if (_channels[channelIndex]->isEnabled() && !_globalSettings && _subsets.size() >= channelIndex)
//        flags |= Qt::ItemIsEnabled;
//}
//
//if (index.column() >= Column::ChannelSettingsStart && index.column() < Column::ChannelSettingsEnd) {
//    const auto channelIndex = index.column() - Column::ChannelSettingsStart;
//
//    if (_channels[channelIndex]->isEnabled())
//        flags |= Qt::ItemIsEnabled;
//}

//if (column >= Column::ChannelEnabledStart && column < Column::ChannelEnabledEnd)
//    return isChannelEnabled(column - Column::ChannelEnabledStart, role);
//
//if (column >= Column::ChannelNameStart && column < Column::ChannelNameEnd)
//    return getChannelName(column - Column::ChannelNameStart, role);
//
//if (column >= Column::ChannelDatasetNameStart && column < Column::ChannelDatasetNameEnd)
//    return getChannelDatasetName(column - Column::ChannelDatasetNameStart, role);
//
//if (column >= Column::ChannelDataNameStart && column < Column::ChannelDataNameEnd)
//    return getChannelDataName(column - Column::ChannelDataNameStart, role);
//
//if (column >= Column::ChannelColorStart && column < Column::ChannelColorEnd)
//    return getChannelColor(column - Column::ChannelColorStart, role);
//
//if (column >= Column::ChannelOpacityStart && column < Column::ChannelOpacityEnd)
//    return getChannelOpacity(column - Column::ChannelOpacityStart, role);
//
//if (column >= Column::ChannelProfileTypeStart && column < Column::ChannelProfileTypeEnd)
//    return getChannelProfileType(column - Column::ChannelProfileTypeStart, role);
//
//if (column >= Column::ChannelRangeTypeStart && column < Column::ChannelRangeTypeEnd)
//    return getChannelRangeType(column - Column::ChannelRangeTypeStart, role);
//
//if (column >= Column::ChannelSettingsStart && column < Column::ChannelSettingsEnd) {
//    switch (role)
//    {
//        case Qt::EditRole:
//            return getTooltip(column, "");
//
//        default:
//            break;
//    }
//}


//if (column >= Column::ChannelEnabledStart && column < Column::ChannelEnabledEnd)
//    affectedColumns << setChannelEnabled(column - Column::ChannelEnabledStart, value.toBool());
//
//if (column >= Column::ChannelNameStart && column < Column::ChannelNameEnd)
//    affectedColumns << setChannelName(column - Column::ChannelNameStart, value.toString());
//
//if (column >= Column::ChannelDatasetNameStart && column < Column::ChannelDatasetNameEnd)
//    affectedColumns << setChannelDatasetName(column - Column::ChannelDatasetNameStart, value.toString());
//
//if (column >= Column::ChannelColorStart && column < Column::ChannelColorEnd)
//    affectedColumns << setChannelColor(column - Column::ChannelColorStart, value.value<QColor>());
//
//if (column >= Column::ChannelOpacityStart && column < Column::ChannelOpacityEnd)
//    affectedColumns << setChannelOpacity(column - Column::ChannelOpacityStart, value.toFloat());
//
//if (column >= Column::ChannelProfileTypeStart && column < Column::ChannelProfileTypeEnd)
//    affectedColumns << setChannelProfileType(column - Column::ChannelProfileTypeStart, static_cast<Profile::ProfileType>(value.toInt()));
//
//if (column >= Column::ChannelRangeTypeStart && column < Column::ChannelRangeTypeEnd)
//    affectedColumns << setChannelRangeType(column - Column::ChannelRangeTypeStart, static_cast<Profile::RangeType>(value.toInt()));


//QVariant Configuration::isChannelEnabled(const std::int32_t& channelIndex, const std::int32_t& role) const
//{
//	try
//	{
//		const auto enabled			= _channels[channelIndex]->isEnabled();
//		const auto enabledString	= enabled ? "on" : "off";
//
//		switch (role)
//		{
//			case Qt::DisplayRole:
//				return enabledString;
//
//			case Qt::EditRole:
//				return enabled;
//
//			case Qt::ToolTipRole:
//                return getTooltip(Column::ChannelEnabledStart + channelIndex, enabledString);
//
//			default:
//				return QVariant();
//		}
//	}
//	catch (std::exception exception)
//	{
//		qDebug() << exception.what();
//	}
//	
//	return QVariant();
//}
//
//Configuration::AffectedColumns Configuration::setChannelEnabled(const std::int32_t& channelIndex, const bool& enabled)
//{
//    AffectedColumns affectedColumns{ Column::ChannelEnabledStart + channelIndex };
//
//    try
//    {
//        if (enabled == _channels[channelIndex]->isEnabled())
//            return affectedColumns;
//
//        _channels[channelIndex]->setEnabled(enabled);
//
//        affectedColumns << Column::ChannelDatasetNameStart + channelIndex;
//        affectedColumns << Column::ChannelColorStart + channelIndex;
//        affectedColumns << Column::ChannelOpacityStart + channelIndex;
//        affectedColumns << Column::ChannelProfileTypeStart + channelIndex;
//        affectedColumns << Column::ChannelRangeTypeStart + channelIndex;
//        affectedColumns << Column::ChannelSettingsStart + channelIndex;
//        affectedColumns << Column::GlobalSettings;
//        affectedColumns << Column::ShowDimensionNames;
//        affectedColumns << setShowDifferentialProfile(canShowDifferentialProfile());
//        affectedColumns << setGlobalSettings(getNoDisplayChannels() > 0);
//        affectedColumns << updateDifferentialProfile();
//    }
//    catch (std::exception exception)
//    {
//        qDebug() << exception.what();
//    }
//
//    return affectedColumns;
//}
//
//QVariant Configuration::getChannelName(const std::int32_t& channelIndex, const std::int32_t& role) const
//{
//    try
//    {
//        const auto name         = _channels[channelIndex]->getDisplayName();
//        const auto nameString   = name;
//
//        switch (role)
//        {
//            case Qt::DisplayRole:
//                return nameString;
//
//            case Qt::EditRole:
//                return name;
//
//            case Qt::ToolTipRole:
//                return getTooltip(Column::ChannelNameStart + channelIndex, nameString);
//
//            default:
//                return QVariant();
//        }
//    }
//    catch (std::exception exception)
//    {
//        qDebug() << exception.what();
//    }
//
//    return QVariant();
//}
//
//Configuration::AffectedColumns Configuration::setChannelName(const std::int32_t& channelIndex, const QString& name)
//{
//    AffectedColumns affectedColumns{ Column::ChannelNameStart + channelIndex };
//
//    try
//    {
//        if (name == _channels[channelIndex]->getDisplayName())
//            return affectedColumns;
//
//        _channels[channelIndex]->setDisplayName(name);
//    }
//    catch (std::exception exception)
//    {
//        qDebug() << exception.what();
//    }
//
//    return affectedColumns;
//}


//QVariant Configuration::getChannelDatasetName(const std::int32_t& channelIndex, const std::int32_t& role) const
//{
//    try
//    {
//        const auto datasetName = _channels[channelIndex]->getDatasetName();
//
//        switch (role)
//        {
//            case Qt::DisplayRole:
//                return datasetName;
//
//            case Qt::EditRole:
//                return datasetName;
//
//            case Qt::ToolTipRole:
//                return getTooltip(Column::ChannelDatasetNameStart + channelIndex, datasetName);
//
//            default:
//                return QVariant();
//        }
//    }
//    catch (std::exception exception)
//    {
//        qDebug() << exception.what();
//    }
//
//	return QVariant();
//}
//
//Configuration::AffectedColumns Configuration::setChannelDatasetName(const std::int32_t& channelIndex, const QString& datasetName)
//{
//    AffectedColumns affectedColumns{ Column::ChannelDatasetNameStart + channelIndex };
//
//	try
//	{
//        if (datasetName == _channels[channelIndex]->getDatasetName())
//            return affectedColumns;
//
//		_channels[channelIndex]->setDatasetName(datasetName);
//	}
//	catch (std::exception exception)
//	{
//		qDebug() << exception.what();
//	}
//
//    return affectedColumns;
//}
//
//QVariant Configuration::getChannelDataName(const std::int32_t& channelIndex, const std::int32_t& role) const
//{
//	try
//	{
//		const auto dataName = _channels[channelIndex]->getDataName();
//
//		switch (role)
//		{
//			case Qt::DisplayRole:
//				return dataName;
//
//			case Qt::EditRole:
//				return dataName;
//
//			case Qt::ToolTipRole:
//                return getTooltip(Column::ChannelDataNameStart + channelIndex, dataName);
//
//			default:
//				return QVariant();
//		}
//	}
//	catch (std::exception exception)
//	{
//		qDebug() << exception.what();
//	}
//
//	return QVariant();
//}
//
//QVariant Configuration::getChannelColor(const std::int32_t& channelIndex, const std::int32_t& role) const
//{
//	try
//	{
//		const auto color		= _channels[channelIndex]->getColor();
//		const auto colorString	= QString("(%1, %2, %3)").arg(QString::number(color.red()), QString::number(color.green()), QString::number(color.blue()));
//
//		switch (role)
//		{
//			case Qt::DisplayRole:
//				return colorString;
//
//			case Qt::EditRole:
//				return color;
//
//			case Qt::ToolTipRole:
//                return getTooltip(Column::ChannelColorStart + channelIndex, colorString);
//
//			default:
//				return QVariant();
//		}
//	}
//	catch (std::exception exception)
//	{
//		qDebug() << exception.what();
//	}
//
//	return QVariant();
//}
//
//Configuration::AffectedColumns Configuration::setChannelColor(const std::int32_t& channelIndex, const QColor& color)
//{
//    AffectedColumns affectedColumns{ Column::ChannelColorStart + channelIndex };
//
//	try
//	{
//        if (color == _channels[channelIndex]->getColor())
//            return affectedColumns;
//
//		_channels[channelIndex]->setColor(color);
//	}
//	catch (std::exception exception)
//	{
//		qDebug() << exception.what();
//	}
//
//    return affectedColumns;
//}
//
//QVariant Configuration::getChannelOpacity(const std::int32_t& channelIndex, const std::int32_t& role) const
//{
//	try
//	{
//		const auto opacity			= _channels[channelIndex]->getOpacity();
//		const auto opacityString	= QString::number(opacity, 'f', 1);
//
//		switch (role)
//		{
//			case Qt::DisplayRole:
//				return opacityString;
//
//			case Qt::EditRole:
//				return opacity;
//
//			case Qt::ToolTipRole:
//                return getTooltip(Column::ChannelOpacityStart + channelIndex, opacityString);
//
//			default:
//				return QVariant();
//		}
//	}
//	catch (std::exception exception)
//	{
//		qDebug() << exception.what();
//	}
//
//	return QVariant();
//}
//
//Configuration::AffectedColumns Configuration::setChannelOpacity(const std::int32_t& channelIndex, const float& opacity)
//{
//    AffectedColumns affectedColumns{ Column::ChannelOpacityStart + channelIndex };
//
//	try
//	{
//        if (opacity == _channels[channelIndex]->getOpacity())
//            return affectedColumns;
//
//		_channels[channelIndex]->setOpacity(opacity);
//	}
//	catch (std::exception exception)
//	{
//		qDebug() << exception.what();
//	}
//
//    return affectedColumns;
//}
//
//QVariant Configuration::getChannelProfileType(const std::int32_t& channelIndex, const std::int32_t& role) const
//{
//	try
//	{
//		const auto profileType			= _channels[channelIndex]->getProfileType();
//		const auto profileTypeString	= Profile::getProfileTypeName(profileType);
//
//		switch (role)
//		{
//			case Qt::DisplayRole:
//				return profileTypeString;
//
//			case Qt::EditRole:
//				return static_cast<int>(profileType);
//
//			case Qt::ToolTipRole:
//                return getTooltip(Column::ChannelProfileTypeStart + channelIndex, profileTypeString);
//
//			default:
//				return QVariant();
//		}
//	}
//	catch (std::exception exception)
//	{
//		qDebug() << exception.what();
//	}
//
//	return QVariant();
//}
//
//Configuration::AffectedColumns Configuration::setChannelProfileType(const std::int32_t& channelIndex, const Profile::ProfileType& profileType)
//{
//    AffectedColumns affectedColumns{ Column::ChannelProfileTypeStart + channelIndex };
//
//	try
//	{
//        if (profileType == _channels[channelIndex]->getProfileType())
//            return affectedColumns;
//
//		_channels[channelIndex]->setProfileType(profileType);
//
//        affectedColumns << Column::ChannelRangeTypeStart + channelIndex;
//        affectedColumns << Column::ShowDimensionNames;
//	}
//	catch (std::exception exception)
//	{
//		qDebug() << exception.what();
//	}
//
//    return affectedColumns;
//}
//
//QVariant Configuration::getChannelRangeType(const std::int32_t& channelIndex, const std::int32_t& role) const
//{
//	try
//	{
//		const auto rangeType        = _channels[channelIndex]->getRangeType();
//		const auto rangeTypeString	= Profile::getRangeTypeName(rangeType);
//
//		switch (role)
//		{
//			case Qt::DisplayRole:
//				return rangeTypeString;
//
//			case Qt::EditRole:
//				return static_cast<int>(rangeType);
//
//			case Qt::ToolTipRole:
//				return getTooltip(Column::ChannelRangeTypeStart + channelIndex, rangeTypeString);
//
//			default:
//				return QVariant();
//		}
//	}
//	catch (std::exception exception)
//	{
//		qDebug() << exception.what();
//	}
//
//	return QVariant();
//}
//
//Configuration::AffectedColumns Configuration::setChannelRangeType(const std::int32_t& channelIndex, const Profile::RangeType& rangeType)
//{
//    AffectedColumns affectedColumns{ Column::ChannelRangeTypeStart + channelIndex };
//
//	try
//	{
//        if (rangeType == _channels[channelIndex]->getRangeType())
//            return affectedColumns;
//
//		_channels[channelIndex]->setRangeType(rangeType);
//
//        affectedColumns << Column::ShowDimensionNames;
//	}
//	catch (std::exception exception)
//	{
//		qDebug() << exception.what();
//	}
//
//    return affectedColumns;
//}
