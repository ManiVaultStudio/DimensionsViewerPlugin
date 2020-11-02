#include "Configuration.h"
#include "DimensionsViewerPlugin.h"

#include <QDebug>

DimensionsViewerPlugin* Configuration::dimensionsViewerPlugin = nullptr;
std::int32_t Configuration::maxNoDimensions = 100;

Configuration::Configuration(QObject* parent, const QString& datasetName, const QString& dataName) :
	QObject(parent),
	_channels({ 
		new Channel(parent, 0, "Dataset", true, datasetName, dataName, Qt::black, 0.25f),
		new Channel(parent, 1, "Subset 1", false, "", dataName, QColor(249, 149, 0), 0.25f),
		new Channel(parent, 2, "Subset 2", false, "", dataName, QColor(0, 112, 249), 0.25f)
	}),
	_subsets(),
    _showDifferentialProfile(false),
    _profileDatasetNames(),
    _profileDatasetName(),
    _showDimensionNames(false),
    _globalSettings(true),
    _globalProfileType(Channel::ProfileType::Mean),
    _globalRangeType(Channel::RangeType::StandardDeviation1),
    _spec()
{
    _spec["modified"] = 0;

    for (auto channel : _channels) {
        QObject::connect(channel, &Channel::specChanged, [this](Channel* channel) {
            _spec["modified"] = _spec["modified"].toInt() + 1;
        });
    }
}

Qt::ItemFlags Configuration::getFlags(const QModelIndex& index) const
{
	Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsEditable;

	if (index.column() >= Column::ChannelDatasetNameStart && index.column() < Column::ChannelDatasetNameEnd) {
		const auto channelIndex = index.column() - Column::ChannelDatasetNameStart;

		if (channelIndex == 0) {
            flags |= Qt::ItemIsEnabled;
		}
		else {
			if (_channels[channelIndex]->isEnabled() && _subsets.size() >= channelIndex)
				flags |= Qt::ItemIsEnabled;
		}
	}

	if (index.column() >= Column::ChannelEnabledStart && index.column() < Column::ChannelEnabledEnd) {
		const auto channelIndex = index.column() - Column::ChannelEnabledStart;

		if (channelIndex == 0) {
			flags |= Qt::ItemIsEnabled;
		}
		else {
			if (_subsets.size() >= channelIndex)
				flags |= Qt::ItemIsEnabled;
		}
	}

	if (index.column() >= Column::ChannelColorStart && index.column() < Column::ChannelColorEnd) {
		const auto channelIndex = index.column() - Column::ChannelColorStart;

		if (channelIndex == 0) {
            if (_channels[0]->isEnabled())
			    flags |= Qt::ItemIsEnabled;
		}
		else {
			if (_channels[channelIndex]->isEnabled() && _subsets.size() >= channelIndex)
				flags |= Qt::ItemIsEnabled;
		}
	}

	if (index.column() >= Column::ChannelProfileTypeStart && index.column() < Column::ChannelProfileTypeEnd) {
		const auto channelIndex = index.column() - Column::ChannelProfileTypeStart;

        if (_channels[channelIndex]->isEnabled() && !_globalSettings && _subsets.size() >= channelIndex)
            flags |= Qt::ItemIsEnabled;
	}

	if (index.column() >= Column::ChannelRangeTypeStart && index.column() < Column::ChannelRangeTypeEnd) {
		const auto channelIndex = index.column() - Column::ChannelRangeTypeStart;

        if (_channels[channelIndex]->isEnabled() && !_globalSettings && _subsets.size() >= channelIndex)
            flags |= Qt::ItemIsEnabled;
	}

	if (index.column() >= Column::ChannelOpacityStart && index.column() < Column::ChannelOpacityEnd) {
		const auto channelIndex = index.column() - Column::ChannelOpacityStart;

		if (channelIndex == 0) {
			flags |= Qt::ItemIsEnabled;
		}
		else {
			if (_channels[channelIndex]->isEnabled() && _subsets.size() >= channelIndex)
				flags |= Qt::ItemIsEnabled;
		}
	}

    if (index.column() == Column::ShowDimensionNames) {
        if (getNoDisplayChannels() > 0)
            flags |= Qt::ItemIsEnabled;
    }

    if (index.column() == Column::GlobalSettings) {
        if (getNoDisplayChannels() > 0)
            flags |= Qt::ItemIsEnabled;
    }

    if (index.column() == Column::GlobalProfileType || index.column() == Column::GlobalRangeType) {
        if (_globalSettings)
            flags |= Qt::ItemIsEnabled;
    }
    
    if (index.column() == Column::ShowDifferentialProfile) {
        if (canShowDifferentialProfile())
            flags |= Qt::ItemIsEnabled;
    }

    if (index.column() == Column::Profile1DatasetName) {
        if (canShowDifferentialProfile() && _showDifferentialProfile && getNoChannelsEnabled() > 2)
            flags |= Qt::ItemIsEnabled;
    }

    if (index.column() == Column::Profile2DatasetName) {
        if (canShowDifferentialProfile() && _showDifferentialProfile && getNoChannelsEnabled() > 2)
            flags |= Qt::ItemIsEnabled;
    }

	return flags;
}

QVariant Configuration::getData(const QModelIndex& index, const int& role) const
{
	if (index.column() == Column::Subsets)
		return getSubsets(role);

	if (index.column() >= Column::ChannelEnabledStart && index.column() < Column::ChannelEnabledEnd)
		return isChannelEnabled(index.column() - Column::ChannelEnabledStart, role);

	if (index.column() >= Column::ChannelDatasetNameStart && index.column() < Column::ChannelDatasetNameEnd)
		return getChannelDatasetName(index.column() - Column::ChannelDatasetNameStart, role);

	if (index.column() >= Column::ChannelDataNameStart && index.column() < Column::ChannelDataNameEnd)
		return getChannelDataName(index.column() - Column::ChannelDataNameStart, role);

	if (index.column() >= Column::ChannelColorStart && index.column() < Column::ChannelColorEnd)
		return getChannelColor(index.column() - Column::ChannelColorStart, role);

	if (index.column() >= Column::ChannelOpacityStart && index.column() < Column::ChannelOpacityEnd)
		return getChannelOpacity(index.column() - Column::ChannelOpacityStart, role);

	if (index.column() >= Column::ChannelProfileTypeStart && index.column() < Column::ChannelProfileTypeEnd)
		return getChannelProfileType(index.column() - Column::ChannelProfileTypeStart, role);

	if (index.column() >= Column::ChannelRangeTypeStart && index.column() < Column::ChannelRangeTypeEnd)
		return getChannelRangeType(index.column() - Column::ChannelRangeTypeStart, role);

	if (index.column() == Column::GlobalSettings)
		return getGlobalSettings(role);

    if (index.column() == Column::GlobalProfileType)
        return getGlobalProfileType(role);

    if (index.column() == Column::GlobalRangeType)
        return getGlobalRangeType(role);

    if (index.column() == Column::ShowDimensionNames)
        return getShowDimensionNames(role);

    if (index.column() == Column::ShowDifferentialProfile)
        return getShowDifferentialProfile(role);

    if (index.column() == Column::Profile1DatasetNames)
        return _profileDatasetNames[0];

    if (index.column() == Column::Profile2DatasetNames)
        return _profileDatasetNames[1];

    if (index.column() == Column::Profile1DatasetName)
        return _profileDatasetName[0];

    if (index.column() == Column::Profile2DatasetName)
        return _profileDatasetName[1];

	return QVariant();
}

QModelIndexList Configuration::setData(const QModelIndex& index, const QVariant& value, const int& role)
{
	QModelIndexList affectedIndices{ index };

    const auto resetDifferentialProfile = [this]() {
        setProfileDatasetName(0, "");
        setProfileDatasetName(1, "");

        setProfileDatasetNames(0, QStringList());
        setProfileDatasetNames(1, QStringList());
    };

    const auto getAllDatasetNames = [this]() -> QStringList {
        QStringList allDatasetNames;

        for (auto channel : _channels) {
            if (channel->isEnabled())
                allDatasetNames << channel->getDatasetName();
        }

        return allDatasetNames;
    };

    const auto getDatasetName = [this, &getAllDatasetNames](const std::uint32_t& profileIndex) -> QString {
        const auto opposingProfileIndex = profileIndex == 0 ? 1 : 0;
        const auto otherDatasetName     = _profileDatasetName[opposingProfileIndex];
        const auto profileDatasetName   = _profileDatasetName[profileIndex];

        auto datasetNames = getAllDatasetNames();

        datasetNames.removeOne(otherDatasetName);

        if (datasetNames.contains(profileDatasetName))
            return profileDatasetName;

        return datasetNames.first();
    };

    const auto getDatasetNames = [this, &getAllDatasetNames](const std::uint32_t& profileIndex) -> QStringList {
        const auto opposingProfileIndex = profileIndex == 0 ? 1 : 0;
        const auto otherDatasetName     = _profileDatasetName[opposingProfileIndex];

        auto datasetNames = getAllDatasetNames();

        datasetNames.removeOne(otherDatasetName);

        return datasetNames;
    };

    const auto updateDifferentialProfile = [this, &resetDifferentialProfile, &getDatasetNames, index, &affectedIndices]() {
        if (!canShowDifferentialProfile()) {
            resetDifferentialProfile();
        }
        else {
            if (_showDifferentialProfile) {
                const auto channelsEnabled      = getChannelsEnabled();
                //const auto opposingProfileIndex = profileIndex == 0 ? 1 : 0;
                //const auto opposingDatasetName  = getChannelDatasetName(opposingProfileIndex, Qt::EditRole).toString();

                switch (channelsEnabled.size())
                {
                    case 2: {
                        setProfileDatasetName(0, getChannelDatasetName(channelsEnabled[0], Qt::EditRole).toString());
                        setProfileDatasetName(1, getChannelDatasetName(channelsEnabled[1], Qt::EditRole).toString());
                        break;
                    }

                    case 3: {
                        if (_profileDatasetName[0] == "")
                            setProfileDatasetName(0, getChannelDatasetName(1, Qt::EditRole).toString());

                        if (_profileDatasetName[1] == "")
                            setProfileDatasetName(1, getChannelDatasetName(2, Qt::EditRole).toString());

                        break;
                    }

                    default:
                        break;
                }

                setProfileDatasetNames(0, getDatasetNames(0));
                setProfileDatasetNames(1, getDatasetNames(1));
            }
            else {
                resetDifferentialProfile();
            }
        }

        affectedIndices << index.siblingAtColumn(Column::ShowDifferentialProfile);
        affectedIndices << index.siblingAtColumn(Column::Profile1DatasetNames);
        affectedIndices << index.siblingAtColumn(Column::Profile2DatasetNames);
        affectedIndices << index.siblingAtColumn(Column::Profile1DatasetName);
        affectedIndices << index.siblingAtColumn(Column::Profile2DatasetName);
    };

	if (index.column() == Column::Subsets) {
		setSubsets(value.toStringList());

		for (int s = 0; s < _subsets.size(); ++s) {
			affectedIndices << index.siblingAtColumn(Column::ChannelEnabledStart + s +1);
			affectedIndices << index.siblingAtColumn(Column::ChannelDatasetNameStart + s + 1);
			affectedIndices << index.siblingAtColumn(Column::ChannelColorStart + s + 1);
			affectedIndices << index.siblingAtColumn(Column::ChannelOpacityStart + s + 1);
			affectedIndices << index.siblingAtColumn(Column::ChannelProfileTypeStart + s + 1);
			affectedIndices << index.siblingAtColumn(Column::ChannelRangeTypeStart + s + 1);
		}

		switch (_subsets.size())
		{
			case 1: {
				setChannelDatasetName(1, _subsets[0]);
				setChannelEnabled(1, true);
				break;
			}

			case 2: {
				setChannelDatasetName(1, _subsets[0]);
				setChannelDatasetName(2, _subsets[1]);
				setChannelEnabled(1, true);
				setChannelEnabled(2, true);
				break;
			}

			default:
				break;
		}

        affectedIndices << index.siblingAtColumn(Column::ShowDimensionNames);
        affectedIndices << index.siblingAtColumn(Column::GlobalSettings);

        setShowDifferentialProfile(canShowDifferentialProfile());

        updateDifferentialProfile();

        setGlobalSettings(getNoDisplayChannels() > 0);
	}

	if (index.column() >= Column::ChannelEnabledStart && index.column() < Column::ChannelEnabledEnd) {
		const auto channelIndex = index.column() - Column::ChannelEnabledStart;

		setChannelEnabled(channelIndex, value.toBool());

		affectedIndices << index.siblingAtColumn(Column::ChannelDatasetNameStart + channelIndex);
		affectedIndices << index.siblingAtColumn(Column::ChannelColorStart + channelIndex);
		affectedIndices << index.siblingAtColumn(Column::ChannelOpacityStart + channelIndex);
		affectedIndices << index.siblingAtColumn(Column::ChannelProfileTypeStart + channelIndex);
		affectedIndices << index.siblingAtColumn(Column::ChannelRangeTypeStart + channelIndex);
        affectedIndices << index.siblingAtColumn(Column::GlobalSettings);
        affectedIndices << index.siblingAtColumn(Column::ShowDimensionNames);

        setShowDifferentialProfile(canShowDifferentialProfile());

        updateDifferentialProfile();

        setGlobalSettings(getNoDisplayChannels() > 0);
	}

	if (index.column() >= Column::ChannelDatasetNameStart && index.column() < Column::ChannelDatasetNameEnd) {
		setChannelDatasetName(index.column() - Column::ChannelDatasetNameStart, value.toString());
	}

	if (index.column() >= Column::ChannelColorStart && index.column() < Column::ChannelColorEnd) {
		setChannelColor(index.column() - Column::ChannelColorStart, value.value<QColor>());
	}

	if (index.column() >= Column::ChannelOpacityStart && index.column() < Column::ChannelOpacityEnd) {
		setChannelOpacity(index.column() - Column::ChannelOpacityStart, value.toFloat());
	}

	if (index.column() >= Column::ChannelProfileTypeStart && index.column() < Column::ChannelProfileTypeEnd) {
		const auto channelIndex = index.column() - Column::ChannelProfileTypeStart;

		setChannelProfileType(channelIndex, static_cast<Channel::ProfileType>(value.toInt()));

        affectedIndices << index.siblingAtColumn(Column::ShowDimensionNames);
	}

	if (index.column() >= Column::ChannelRangeTypeStart && index.column() < Column::ChannelRangeTypeEnd) {
		const auto channelIndex = index.column() - Column::ChannelRangeTypeStart;

		setChannelRangeType(channelIndex, static_cast<Channel::RangeType>(value.toInt()));

        affectedIndices << index.siblingAtColumn(Column::ShowDimensionNames);
	}

	if (index.column() == Column::GlobalSettings) {
		const auto globalSettings = value.toBool();

		setGlobalSettings(globalSettings);
		
        for (int channelIndex = 0; channelIndex < Configuration::noChannels; channelIndex++) {
            affectedIndices << index.siblingAtColumn(Column::ChannelProfileTypeStart + channelIndex);
            affectedIndices << index.siblingAtColumn(Column::ChannelRangeTypeStart + channelIndex);
        }
		
        affectedIndices << index.siblingAtColumn(Column::ShowDimensionNames);
	}

    if (index.column() == Column::GlobalProfileType) {
        const auto globalProfileType = static_cast<Channel::ProfileType>(value.toInt());

        setGlobalProfileType(globalProfileType);

        if (getGlobalSettings(Qt::EditRole).toBool())
            for (int channelIndex = 0; channelIndex < Configuration::noChannels; channelIndex++)
                affectedIndices << index.siblingAtColumn(Column::ChannelProfileTypeStart + channelIndex);
    }

    if (index.column() == Column::GlobalRangeType) {
        const auto globalRangeType = static_cast<Channel::RangeType>(value.toInt());

        setGlobalRangeType(globalRangeType);

        if (getGlobalSettings(Qt::EditRole).toBool())
            for (int channelIndex = 0; channelIndex < Configuration::noChannels; channelIndex++)
                affectedIndices << index.siblingAtColumn(Column::ChannelRangeTypeStart + channelIndex);
    }

    if (index.column() == Column::SelectionStamp) {
        //for (int channelIndex = 0; channelIndex < Configuration::noChannels; channelIndex++)
        //    _channels[channelIndex]->updateSpec();

        _channels[0]->updateSpec();
    }

    if (index.column() == Column::ShowDimensionNames) {
        setShowDimensionNames(value.toBool());
    }

    if (index.column() == Column::ShowDifferentialProfile) {
        setShowDifferentialProfile(value.toBool());

        updateDifferentialProfile();
    }

    if (index.column() == Column::Profile1DatasetName) {
        _profileDatasetName[0] = value.toString();

        setProfileDatasetName(1, getDatasetName(1));
        setProfileDatasetNames(1, getDatasetNames(1));

        affectedIndices << index.siblingAtColumn(Column::Profile2DatasetNames);
        affectedIndices << index.siblingAtColumn(Column::Profile2DatasetName);
    }

    if (index.column() == Column::Profile2DatasetName) {
        _profileDatasetName[1] = value.toString();
        
        setProfileDatasetName(0, getDatasetName(0));
        setProfileDatasetNames(0, getDatasetNames(0));

        affectedIndices << index.siblingAtColumn(Column::Profile1DatasetNames);
        affectedIndices << index.siblingAtColumn(Column::Profile1DatasetName);
    }

	return affectedIndices;
}

QVariant Configuration::isChannelEnabled(const std::int32_t& channelIndex, const std::int32_t& role) const
{
	try
	{
		const auto enabled			= _channels[channelIndex]->isEnabled();
		const auto enabledString	= enabled ? "on" : "off";

		switch (role)
		{
			case Qt::DisplayRole:
				return enabledString;

			case Qt::EditRole:
				return enabled;

			case Qt::ToolTipRole:
				return QString("Channel %1 is %2").arg(QString::number(channelIndex + 1), enabledString);

			default:
				return QVariant();
		}
	}
	catch (std::exception exception)
	{
		qDebug() << exception.what();
	}
	
	return QVariant();
}

void Configuration::setChannelEnabled(const std::int32_t& channelIndex, const bool& enabled)
{
	try
	{
		_channels[channelIndex]->setEnabled(enabled);
	}
	catch (std::exception exception)
	{
		qDebug() << exception.what();
	}
}

QVariant Configuration::getSubsets(const std::int32_t& role) const
{
	const auto subsetsString = QString("[%1]").arg(_subsets.join(", "));

	switch (role)
	{
		case Qt::DisplayRole:
			return subsetsString;

		case Qt::EditRole:
			return _subsets;

		case Qt::ToolTipRole:
			return QString("Subsets: %1").arg(subsetsString);

		default:
			return QVariant();
	}
}

void Configuration::setSubsets(const QStringList& subsets)
{
	_subsets = subsets;
}

QVariant Configuration::getChannelDatasetName(const std::int32_t& channelIndex, const std::int32_t& role) const
{
	try
	{
		const auto datasetName = _channels[channelIndex]->getDatasetName();

		switch (role)
		{
			case Qt::DisplayRole:
				return datasetName;

			case Qt::EditRole:
				return datasetName;

			case Qt::ToolTipRole:
				return QString("Channel %1 dataset name: %2").arg(QString::number(channelIndex + 1), datasetName);

			default:
				return QVariant();
		}
	}
	catch (std::exception exception)
	{
		qDebug() << exception.what();
	}

	return QVariant();
}

void Configuration::setChannelDatasetName(const std::int32_t& channelIndex, const QString& datasetName)
{
	try
	{
		_channels[channelIndex]->setDatasetName(datasetName);
	}
	catch (std::exception exception)
	{
		qDebug() << exception.what();
	}
}

QVariant Configuration::getChannelDataName(const std::int32_t& channelIndex, const std::int32_t& role) const
{
	try
	{
		const auto dataName = _channels[channelIndex]->getDataName();

		switch (role)
		{
			case Qt::DisplayRole:
				return dataName;

			case Qt::EditRole:
				return dataName;

			case Qt::ToolTipRole:
				return QString("Channel %1 data name: %2").arg(QString::number(channelIndex + 1), dataName);

			default:
				return QVariant();
		}
	}
	catch (std::exception exception)
	{
		qDebug() << exception.what();
	}

	return QVariant();
}

QVariant Configuration::getChannelColor(const std::int32_t& channelIndex, const std::int32_t& role) const
{
	try
	{
		const auto color		= _channels[channelIndex]->getColor();
		const auto colorString	= QString("(%1, %2, %3)").arg(QString::number(color.red()), QString::number(color.green()), QString::number(color.blue()));

		switch (role)
		{
			case Qt::DisplayRole:
				return colorString;

			case Qt::EditRole:
				return color;

			case Qt::ToolTipRole:
				return QString("Channel %1 color: %2").arg(QString::number(channelIndex + 1), colorString);

			default:
				return QVariant();
		}
	}
	catch (std::exception exception)
	{
		qDebug() << exception.what();
	}

	return QVariant();
}

void Configuration::setChannelColor(const std::int32_t& channelIndex, const QColor& color)
{
	try
	{
		_channels[channelIndex]->setColor(color);
	}
	catch (std::exception exception)
	{
		qDebug() << exception.what();
	}
}

QVariant Configuration::getChannelOpacity(const std::int32_t& channelIndex, const std::int32_t& role) const
{
	try
	{
		const auto opacity			= _channels[channelIndex]->getOpacity();
		const auto opacityString	= QString::number(opacity, 'f', 1);

		switch (role)
		{
			case Qt::DisplayRole:
				return opacityString;

			case Qt::EditRole:
				return opacity;

			case Qt::ToolTipRole:
				return QString("Channel %1 opacity: %2").arg(QString::number(channelIndex + 1), opacityString);

			default:
				return QVariant();
		}
	}
	catch (std::exception exception)
	{
		qDebug() << exception.what();
	}

	return QVariant();
}

void Configuration::setChannelOpacity(const std::int32_t& channelIndex, const float& opacity)
{
	try
	{
		_channels[channelIndex]->setOpacity(opacity);
	}
	catch (std::exception exception)
	{
		qDebug() << exception.what();
	}
}

QVariant Configuration::getChannelProfileType(const std::int32_t& channelIndex, const std::int32_t& role) const
{
	try
	{
		const auto profileType			= _channels[channelIndex]->getProfileType();
		const auto profileTypeString	= Channel::getProfileTypeName(profileType);

		switch (role)
		{
			case Qt::DisplayRole:
				return profileTypeString;

			case Qt::EditRole:
				return static_cast<int>(profileType);

			case Qt::ToolTipRole:
				return QString("Channel %1 profile type: %2").arg(QString::number(channelIndex + 1), profileTypeString);

			default:
				return QVariant();
		}
	}
	catch (std::exception exception)
	{
		qDebug() << exception.what();
	}

	return QVariant();
}

void Configuration::setChannelProfileType(const std::int32_t& channelIndex, const Channel::ProfileType& profileType)
{
	try
	{
		_channels[channelIndex]->setProfileType(profileType);
	}
	catch (std::exception exception)
	{
		qDebug() << exception.what();
	}
}

QVariant Configuration::getChannelRangeType(const std::int32_t& channelIndex, const std::int32_t& role) const
{
	try
	{
		const auto rangeType        = _channels[channelIndex]->getRangeType();
		const auto rangeTypeString	= Channel::getRangeTypeName(rangeType);

		switch (role)
		{
			case Qt::DisplayRole:
				return rangeTypeString;

			case Qt::EditRole:
				return static_cast<int>(rangeType);

			case Qt::ToolTipRole:
				return QString("%1: %2").arg(getColumnName(Column::ChannelRangeTypeStart + channelIndex), rangeTypeString);

			default:
				return QVariant();
		}
	}
	catch (std::exception exception)
	{
		qDebug() << exception.what();
	}

	return QVariant();
}

void Configuration::setChannelRangeType(const std::int32_t& channelIndex, const Channel::RangeType& rangeType)
{
	try
	{
		_channels[channelIndex]->setRangeType(rangeType);
	}
	catch (std::exception exception)
	{
		qDebug() << exception.what();
	}
}

QVariant Configuration::getGlobalSettings(const std::int32_t& role) const
{
	try
	{
		const auto globalSettings           = _globalSettings;
		const auto globalSettingsString     = globalSettings ? "on" : "off";

		switch (role)
		{
			case Qt::DisplayRole:
				return globalSettingsString;

			case Qt::EditRole:
				return globalSettings;

			case Qt::ToolTipRole:
				return QString("Global settings %1").arg(globalSettingsString);

			default:
				return QVariant();
		}
	}
	catch (std::exception exception)
	{
		qDebug() << exception.what();
	}

	return QVariant();
}

void Configuration::setGlobalSettings(const bool& globalSettings)
{
	try
	{
		_globalSettings = globalSettings;

        if (_globalSettings) {
            for (int channelIndex = 0; channelIndex < Configuration::noChannels; channelIndex++) {
                setChannelProfileType(channelIndex, _globalProfileType);
                setChannelRangeType(channelIndex, _globalRangeType);
            }
        }
	}
	catch (std::exception exception)
	{
		qDebug() << exception.what();
	}
}

QVariant Configuration::getGlobalProfileType(const std::int32_t& role) const
{
    const auto profileTypeString = Channel::getProfileTypeName(_globalProfileType);

    switch (role)
    {
        case Qt::DisplayRole:
            return profileTypeString;

        case Qt::EditRole:
            return static_cast<int>(_globalProfileType);

        case Qt::ToolTipRole:
            return QString("Global profile type: %1").arg(profileTypeString);

        default:
            return QVariant();
    }
}

void Configuration::setGlobalProfileType(const Channel::ProfileType& profileType)
{
    _globalProfileType = profileType;

    if (_globalSettings) {
        for (int channelIndex = 0; channelIndex < Configuration::noChannels; channelIndex++) {
            setChannelProfileType(channelIndex, _globalProfileType);
        }
    }
}

QVariant Configuration::getGlobalRangeType(const std::int32_t& role) const
{
    const auto rangeTypeString = Channel::getRangeTypeName(_globalRangeType);

    switch (role)
    {
        case Qt::DisplayRole:
            return rangeTypeString;

        case Qt::EditRole:
            return static_cast<int>(_globalRangeType);

        case Qt::ToolTipRole:
            return QString("Global range type: %1").arg(rangeTypeString);

        default:
            return QVariant();
    }
}

void Configuration::setGlobalRangeType(const Channel::RangeType& rangeType)
{
    _globalRangeType = rangeType;

    if (_globalSettings) {
        for (int channelIndex = 0; channelIndex < Configuration::noChannels; channelIndex++) {
            setChannelRangeType(channelIndex, _globalRangeType);
        }
    }
}

QVariant Configuration::getShowDimensionNames(const std::int32_t& role) const
{
    const auto showDimensionNames       = _showDimensionNames;
    const auto showDimensionNamesString = showDimensionNames ? "on" : "off";

    switch (role)
    {
        case Qt::DisplayRole:
            return showDimensionNamesString;

        case Qt::EditRole:
            return showDimensionNames;

        case Qt::ToolTipRole:
            return QString("Show dimension names: %1").arg(showDimensionNamesString);

        default:
            return QVariant();
    }

    return QVariant();
}

void Configuration::setShowDimensionNames(const bool& showDimensions)
{
    _showDimensionNames = showDimensions;
    _spec["modified"] = _spec["modified"].toInt() + 1;

    dimensionsViewerPlugin->setSetting("ShowDimensionNames", showDimensions);
}

Configuration::Channels& Configuration::getChannels()
{
	return _channels;
}

QVariant Configuration::getShowDifferentialProfile(const std::int32_t& role) const
{
    const auto showDifferentialProfile          = _showDifferentialProfile;
    const auto showDifferentialProfileString    = showDifferentialProfile ? "on" : "off";

    switch (role)
    {
        case Qt::DisplayRole:
            return showDifferentialProfileString;

        case Qt::EditRole:
            return showDifferentialProfile;

        case Qt::ToolTipRole:
            return QString("Show differential profile: %1").arg(showDifferentialProfileString);

        default:
            return QVariant();
    }

    return QVariant();
}

void Configuration::setShowDifferentialProfile(const bool& showDifferentialProfile)
{
    _showDifferentialProfile = showDifferentialProfile;
}

QVariant Configuration::getProfileDatasetNames(const std::int32_t& profileIndex, const std::int32_t& role) const
{
    try
    {
        const auto profileDatasetNames          = _profileDatasetNames[profileIndex];
        const auto profileDatasetNamesString    = profileDatasetNames.join(", ");

        switch (role)
        {
            case Qt::DisplayRole:
                return profileDatasetNamesString;

            case Qt::EditRole:
                return profileDatasetNames;

            case Qt::ToolTipRole:
                return QString("Profile %1 selectable dataset names: %2").arg(QString::number(profileIndex), profileDatasetNamesString);

            default:
                return QVariant();
        }
    }
    catch (std::exception exception)
    {
        qDebug() << exception.what();
    }

    return QVariant();
}

void Configuration::setProfileDatasetNames(const std::int32_t& profileIndex, const QStringList& datasetNames)
{
    try
    {
        _profileDatasetNames[profileIndex] = datasetNames;
    }
    catch (std::exception exception)
    {
        qDebug() << exception.what();
    }
}

QVariant Configuration::getProfileDatasetName(const std::int32_t& profileIndex, const std::int32_t& role) const
{
    try
    {
        const auto profileDatasetName = _profileDatasetName[profileIndex];

        switch (role)
        {
            case Qt::DisplayRole:
                return profileDatasetName;

            case Qt::EditRole:
                return profileDatasetName;

            case Qt::ToolTipRole:
                return QString("Profile %1 selected dataset name: %2").arg(QString::number(profileIndex), profileDatasetName);

            default:
                return QVariant();
        }
    }
    catch (std::exception exception)
    {
        qDebug() << exception.what();
    }

    return QVariant();
}

void Configuration::setProfileDatasetName(const std::int32_t& profileIndex, const QString& datasetName)
{
    try
    {
        _profileDatasetName[profileIndex] = datasetName;
    }
    catch (std::exception exception)
    {
        qDebug() << exception.what();
    }
}

Channel* Configuration::getChannelByDatasetName(const QString& datasetName)
{
	for (auto channel : _channels) {
		if (datasetName == channel->getDatasetName())
			return channel;
	}

	return nullptr;
}

bool Configuration::hasDataset(const QString& datasetName) const
{
	return const_cast<Configuration*>(this)->getChannelByDatasetName(datasetName) != nullptr;
}

void Configuration::updateSpec()
{
    QVariantMap channels;

    for (auto channel : _channels) {
        if (!channel->isEnabled())
            continue;

        channels[channel->getInternalName()] = channel->getSpec();
    }

    _spec["channels"]               = channels;
    _spec["showDimensionNames"]     = _showDimensionNames;
}

QVariantMap Configuration::getSpec() const
{
	return _spec;
}

std::int32_t Configuration::getModified() const
{
    return _spec["modified"].toInt();
}

QVector<std::uint32_t> Configuration::getChannelsEnabled() const
{
    QVector<std::uint32_t> channelsEnabled;

    for (auto channel : _channels) {
        if (channel->isEnabled())
            channelsEnabled << channel->getIndex();
    }

    return channelsEnabled;
}

std::int32_t Configuration::getNoChannelsEnabled() const
{
    auto noChannelsEnabled = 0;

    for (auto channel : _channels) {
        if (channel->isEnabled())
            noChannelsEnabled++;
    }

    return noChannelsEnabled;
}

std::int32_t Configuration::getNoDisplayChannels() const
{
    auto noDisplayChannels = 0;

    for (auto channel : _channels) {
        if (channel->canDisplay())
            noDisplayChannels++;
    }

    return noDisplayChannels;
}

bool Configuration::canShowDifferentialProfile() const
{
    return getNoDisplayChannels() >= 2;
}