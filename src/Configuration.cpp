#include "Configuration.h"
#include "DimensionsViewerPlugin.h"

#include <QDebug>

DimensionsViewerPlugin* Configuration::dimensionsViewerPlugin = nullptr;
std::int32_t Configuration::maxNoDimensions = 100;

Configuration::Configuration(QObject* parent, const QString& datasetName, const QString& dataName) :
	QObject(parent),
	_channels(this, datasetName, dataName),
	_globalSettings(this),
	_subsets(),
    _showDifferentialProfile(false),
    _profileDatasetNames(),
    _profileDatasetName(),
    _showDimensionNames(false),
    _spec()
{
    _spec["modified"] = 0;
}

Qt::ItemFlags Configuration::getFlags(const QModelIndex& index) const
{
	Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsEditable;

    const auto column = static_cast<Column>(index.column());

	/*if (index.column() >= Column::ChannelEnabledStart && index.column() < Column::ChannelEnabledEnd) {
		const auto channelIndex = index.column() - Column::ChannelEnabledStart;

		if (channelIndex == 0) {
			flags |= Qt::ItemIsEnabled;
		}
		else {
			if (_subsets.size() >= channelIndex)
				flags |= Qt::ItemIsEnabled;
		}
	}

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

    if (index.column() >= Column::ChannelSettingsStart && index.column() < Column::ChannelSettingsEnd) {
        const auto channelIndex = index.column() - Column::ChannelSettingsStart;

        if (_channels[channelIndex]->isEnabled())
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

    if (column == Column::ShowDimensionNames) {
        if (getNoDisplayChannels() > 0)
            flags |= Qt::ItemIsEnabled;
    }

    if (column == Column::GlobalSettings) {
        if (getNoDisplayChannels() > 0)
            flags |= Qt::ItemIsEnabled;
    }

    if (column == Column::GlobalProfileType) {
        if (_globalSettings && !_globalProfile.getProfileTypeNames().isEmpty())
            flags |= Qt::ItemIsEnabled;
    }

    if (column == Column::GlobalRangeType) {
        if (_globalSettings && !_globalProfile.getRangeTypeNames().isEmpty())
            flags |= Qt::ItemIsEnabled;
    }
    
    if (column == Column::ShowDifferentialProfile) {
        if (canShowDifferentialProfile())
            flags |= Qt::ItemIsEnabled;
    }

    if (column == Column::Profile1DatasetName) {
        if (canShowDifferentialProfile() && _showDifferentialProfile && getNoChannelsEnabled() > 2)
            flags |= Qt::ItemIsEnabled;
    }

    if (column == Column::Profile2DatasetName) {
        if (canShowDifferentialProfile() && _showDifferentialProfile && getNoChannelsEnabled() > 2)
            flags |= Qt::ItemIsEnabled;
    }
    */

	return flags;
}

QVariant Configuration::getData(const QModelIndex& index, const int& role) const
{
    const auto column = static_cast<Column>(index.column());

    /*if (column == Column::Subsets)
		return getSubsets(role);

	if (column >= Column::ChannelEnabledStart && column < Column::ChannelEnabledEnd)
		return isChannelEnabled(column - Column::ChannelEnabledStart, role);

    if (column >= Column::ChannelNameStart && column < Column::ChannelNameEnd)
        return getChannelName(column - Column::ChannelNameStart, role);

	if (column >= Column::ChannelDatasetNameStart && column < Column::ChannelDatasetNameEnd)
		return getChannelDatasetName(column - Column::ChannelDatasetNameStart, role);

	if (column >= Column::ChannelDataNameStart && column < Column::ChannelDataNameEnd)
		return getChannelDataName(column - Column::ChannelDataNameStart, role);

	if (column >= Column::ChannelColorStart && column < Column::ChannelColorEnd)
		return getChannelColor(column - Column::ChannelColorStart, role);

	if (column >= Column::ChannelOpacityStart && column < Column::ChannelOpacityEnd)
		return getChannelOpacity(column - Column::ChannelOpacityStart, role);

	if (column >= Column::ChannelProfileTypeStart && column < Column::ChannelProfileTypeEnd)
		return getChannelProfileType(column - Column::ChannelProfileTypeStart, role);

	if (column >= Column::ChannelRangeTypeStart && column < Column::ChannelRangeTypeEnd)
		return getChannelRangeType(column - Column::ChannelRangeTypeStart, role);

    if (column >= Column::ChannelSettingsStart && column < Column::ChannelSettingsEnd) {
        switch (role)
        {
            case Qt::EditRole:
                return getTooltip(column, "");

            default:
                break;
        }
    }

	

    if (column == Column::ShowDimensionNames)
        return getShowDimensionNames(role);

    if (column == Column::ShowDifferentialProfile)
        return getShowDifferentialProfile(role);

    if (column == Column::Profile1DatasetNames)
        return _profileDatasetNames[0];

    if (column == Column::Profile2DatasetNames)
        return _profileDatasetNames[1];

    if (column == Column::Profile1DatasetName)
        return _profileDatasetName[0];

    if (column == Column::Profile2DatasetName)
        return _profileDatasetName[1];
        */
	return QVariant();
}

QModelIndexList Configuration::setData(const QModelIndex& index, const QVariant& value, const int& role)
{
	AffectedColumns affectedColumns;

    const auto column = static_cast<Column>(index.column());

    /*if (column == Column::Subsets)
        affectedColumns << setSubsets(value.toStringList());

	if (column >= Column::ChannelEnabledStart && column < Column::ChannelEnabledEnd)
        affectedColumns << setChannelEnabled(column - Column::ChannelEnabledStart, value.toBool());

    if (column >= Column::ChannelNameStart && column < Column::ChannelNameEnd)
        affectedColumns << setChannelName(column - Column::ChannelNameStart, value.toString());

	if (column >= Column::ChannelDatasetNameStart && column < Column::ChannelDatasetNameEnd)
        affectedColumns << setChannelDatasetName(column - Column::ChannelDatasetNameStart, value.toString());

	if (column >= Column::ChannelColorStart && column < Column::ChannelColorEnd)
        affectedColumns << setChannelColor(column - Column::ChannelColorStart, value.value<QColor>());

	if (column >= Column::ChannelOpacityStart && column < Column::ChannelOpacityEnd)
        affectedColumns << setChannelOpacity(column - Column::ChannelOpacityStart, value.toFloat());

	if (column >= Column::ChannelProfileTypeStart && column < Column::ChannelProfileTypeEnd)
        affectedColumns << setChannelProfileType(column - Column::ChannelProfileTypeStart, static_cast<Profile::ProfileType>(value.toInt()));

	if (column >= Column::ChannelRangeTypeStart && column < Column::ChannelRangeTypeEnd)
        affectedColumns << setChannelRangeType(column - Column::ChannelRangeTypeStart, static_cast<Profile::RangeType>(value.toInt()));*/

	

    /*if (column == Column::SelectionStamp)
        _channels[0]->updateSpec();

    if (column == Column::ShowDimensionNames)
        affectedColumns << setShowDimensionNames(value.toBool());

    if (column == Column::ShowDifferentialProfile)
        affectedColumns << setShowDifferentialProfile(value.toBool());

    if (column == Column::Profile1DatasetName)
        affectedColumns << setProfileDatasetName(0, value.toString());

    if (column == Column::Profile2DatasetName)
        affectedColumns << setProfileDatasetName(1, value.toString());
        */
    QModelIndexList affectedIndices;

    auto affectedColumnsSet = QSet(affectedColumns.begin(), affectedColumns.end());

    for (auto affectedColumn : affectedColumnsSet)
        affectedIndices << index.siblingAtColumn(affectedColumn);

	return affectedIndices;
}

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

//QVariant Configuration::getSubsets(const std::int32_t& role) const
//{
//	const auto subsetsString = QString("[%1]").arg(_subsets.join(", "));
//
//	switch (role)
//	{
//		case Qt::DisplayRole:
//			return subsetsString;
//
//		case Qt::EditRole:
//			return _subsets;
//
//		case Qt::ToolTipRole:
//            return getTooltip(Column::Subsets, subsetsString);
//
//		default:
//			return QVariant();
//	}
//}
//
//AffectedColumns Configuration::setSubsets(const QStringList& subsets)
//{
//    AffectedColumns affectedColumns{ Column::Subsets };
//
//    if (subsets == _subsets)
//        return affectedColumns;
//
//	_subsets = subsets;
//
//    for (int s = 0; s < _subsets.size(); ++s) {
//        affectedColumns << Column::ChannelEnabledStart + s + 1;
//        affectedColumns << Column::ChannelDatasetNameStart + s + 1;
//        affectedColumns << Column::ChannelColorStart + s + 1;
//        affectedColumns << Column::ChannelOpacityStart + s + 1;
//        affectedColumns << Column::ChannelProfileTypeStart + s + 1;
//        affectedColumns << Column::ChannelRangeTypeStart + s + 1;
//    }
//
//    switch (_subsets.size())
//    {
//        case 1: {
//            affectedColumns << setChannelDatasetName(1, _subsets[0]);
//            affectedColumns << setChannelEnabled(1, true);
//            break;
//        }
//
//        case 2: {
//            affectedColumns << setChannelDatasetName(1, _subsets[0]);
//            affectedColumns << setChannelDatasetName(2, _subsets[1]);
//            affectedColumns << setChannelEnabled(1, true);
//            affectedColumns << setChannelEnabled(2, true);
//            break;
//        }
//
//        default:
//            break;
//    }
//
//    affectedColumns << setShowDifferentialProfile(canShowDifferentialProfile());
//    affectedColumns << setGlobalSettings(getNoDisplayChannels() > 0);
//    affectedColumns << updateDifferentialProfile();*/
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


//


//QVariant Configuration::getShowDimensionNames(const std::int32_t& role) const
//{
//    const auto showDimensionNames       = _showDimensionNames;
//    const auto showDimensionNamesString = showDimensionNames ? "on" : "off";
//
//    switch (role)
//    {
//        case Qt::DisplayRole:
//            return showDimensionNamesString;
//
//        case Qt::EditRole:
//            return showDimensionNames;
//
//        case Qt::ToolTipRole:
//            return getTooltip(Column::ShowDimensionNames, showDimensionNamesString);
//
//        default:
//            return QVariant();
//    }
//
//    return QVariant();
//}
//
//Configuration::AffectedColumns Configuration::setShowDimensionNames(const bool& showDimensionNames)
//{
//    AffectedColumns affectedColumns{ Column::ShowDimensionNames };
//
//    if (showDimensionNames == _showDimensionNames)
//        return affectedColumns;
//
//    _showDimensionNames = showDimensionNames;
//    _spec["modified"] = _spec["modified"].toInt() + 1;
//
//    return affectedColumns;
//}

//QVariant Configuration::getShowDifferentialProfile(const std::int32_t& role) const
//{
//    const auto showDifferentialProfile          = _showDifferentialProfile;
//    const auto showDifferentialProfileString    = showDifferentialProfile ? "on" : "off";
//
//    switch (role)
//    {
//        case Qt::DisplayRole:
//            return showDifferentialProfileString;
//
//        case Qt::EditRole:
//            return showDifferentialProfile;
//
//        case Qt::ToolTipRole:
//            return getTooltip(Column::ShowDifferentialProfile, showDifferentialProfileString);
//
//        default:
//            return QVariant();
//    }
//
//    return QVariant();
//}
//
//Configuration::AffectedColumns Configuration::setShowDifferentialProfile(const bool& showDifferentialProfile)
//{
//    AffectedColumns affectedColumns{ Column::ShowDifferentialProfile };
//
//    if (showDifferentialProfile == _showDifferentialProfile)
//        return affectedColumns;
//
//    _showDifferentialProfile = showDifferentialProfile;
//
//    affectedColumns << updateDifferentialProfile();
//
//    return affectedColumns;
//}
//
//QVariant Configuration::getProfileDatasetNames(const std::int32_t& profileIndex, const std::int32_t& role) const
//{
//    try
//    {
//        const auto profileDatasetNames          = _profileDatasetNames[profileIndex];
//        const auto profileDatasetNamesString    = profileDatasetNames.join(", ");
//
//        switch (role)
//        {
//            case Qt::DisplayRole:
//                return profileDatasetNamesString;
//
//            case Qt::EditRole:
//                return profileDatasetNames;
//
//            case Qt::ToolTipRole:
//                return getTooltip(Column::Profile1DatasetNames + profileIndex, profileDatasetNamesString);
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
//Configuration::AffectedColumns Configuration::setProfileDatasetNames(const std::int32_t& profileIndex, const QStringList& datasetNames)
//{
//    AffectedColumns affectedColumns{ Column::Profile1DatasetNames + profileIndex };
//
//    try
//    {
//        if (datasetNames == _profileDatasetNames[profileIndex])
//            return affectedColumns;
//
//        _profileDatasetNames[profileIndex] = datasetNames;
//    }
//    catch (std::exception exception)
//    {
//        qDebug() << exception.what();
//    }
//
//    return affectedColumns;
//}
//
//QVariant Configuration::getProfileDatasetName(const std::int32_t& profileIndex, const std::int32_t& role) const
//{
//    try
//    {
//        const auto profileDatasetName = _profileDatasetName[profileIndex];
//
//        switch (role)
//        {
//            case Qt::DisplayRole:
//                return profileDatasetName;
//
//            case Qt::EditRole:
//                return profileDatasetName;
//
//            case Qt::ToolTipRole:
//                return getTooltip(Column::Profile1DatasetName + profileIndex, profileDatasetName);
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
//Configuration::AffectedColumns Configuration::setProfileDatasetName(const std::int32_t& profileIndex, const QString& datasetName)
//{
//    AffectedColumns affectedColumns{Column::Profile1DatasetName + profileIndex};
//
//    try
//    {
//        if (datasetName == _profileDatasetName[profileIndex])
//            return affectedColumns;
//
//        _profileDatasetName[profileIndex] = datasetName;
//    }
//    catch (std::exception exception)
//    {
//        qDebug() << exception.what();
//    }
//
//    return affectedColumns;
//}

bool Configuration::hasDataset(const QString& datasetName) const
{
	return const_cast<Configuration*>(this)->_channels.getChannelByDatasetName(datasetName) != nullptr;
}

void Configuration::updateSpec()
{
    /*QVariantMap channels;

    for (auto channel : _channels) {
        if (!channel->isEnabled())
            continue;

        channels[channel->getInternalName()] = channel->getSpec();
    }

    _spec["channels"]               = channels;
    _spec["showDimensionNames"]     = _showDimensionNames;*/
}

QVariantMap Configuration::getSpec() const
{
	return _spec;
}

std::int32_t Configuration::getModified() const
{
    return _spec["modified"].toInt();
}

bool Configuration::canShowDifferentialProfile() const
{
    return _channels.getNoDisplayChannels() >= 2;
}

QString Configuration::getTooltip(const std::int32_t& column, const QString& description) const
{
    const auto columnName = getColumnName(column);

    if (description.isEmpty())
        return columnName;

    return QString("%1: %2").arg(columnName, description);
}

//Configuration::AffectedColumns Configuration::updateDifferentialProfile()
//{
//    AffectedColumns affectedColumns;
//
//    const auto resetDifferentialProfile = [this, &affectedColumns]() {
//        affectedColumns << setProfileDatasetName(0, "");
//        affectedColumns << setProfileDatasetName(1, "");
//
//        affectedColumns << setProfileDatasetNames(0, QStringList());
//        affectedColumns << setProfileDatasetNames(1, QStringList());
//    };
//
//    const auto getAllDatasetNames = [this]() -> QStringList {
//        QStringList allDatasetNames;
//
//        for (auto channel : _channels) {
//            if (channel->isEnabled())
//                allDatasetNames << channel->getDatasetName();
//        }
//
//        return allDatasetNames;
//    };
//
//    const auto getDatasetName = [this, &getAllDatasetNames](const std::uint32_t& profileIndex) -> QString {
//        const auto opposingProfileIndex = profileIndex == 0 ? 1 : 0;
//        const auto otherDatasetName = _profileDatasetName[opposingProfileIndex];
//        const auto profileDatasetName = _profileDatasetName[profileIndex];
//
//        auto datasetNames = getAllDatasetNames();
//
//        datasetNames.removeOne(otherDatasetName);
//
//        if (datasetNames.contains(profileDatasetName))
//            return profileDatasetName;
//
//        return datasetNames.first();
//    };
//
//    const auto getDatasetNames = [this, &getAllDatasetNames](const std::uint32_t& profileIndex) -> QStringList {
//        const auto opposingProfileIndex = profileIndex == 0 ? 1 : 0;
//        const auto otherDatasetName = _profileDatasetName[opposingProfileIndex];
//
//        auto datasetNames = getAllDatasetNames();
//
//        datasetNames.removeOne(otherDatasetName);
//
//        return datasetNames;
//    };
//
//    if (!canShowDifferentialProfile()) {
//        resetDifferentialProfile();
//    }
//    else {
//        if (_showDifferentialProfile) {
//            const auto channelsEnabled = getChannelsEnabled();
//            //const auto opposingProfileIndex = profileIndex == 0 ? 1 : 0;
//            //const auto opposingDatasetName  = getChannelDatasetName(opposingProfileIndex, Qt::EditRole).toString();
//
//            switch (channelsEnabled.size())
//            {
//                case 2: {
//                    affectedColumns << setProfileDatasetName(0, getChannelDatasetName(channelsEnabled[0], Qt::EditRole).toString());
//                    affectedColumns << setProfileDatasetName(1, getChannelDatasetName(channelsEnabled[1], Qt::EditRole).toString());
//                    break;
//                }
//
//                case 3: {
//                    if (_profileDatasetName[0] == "")
//                        affectedColumns << setProfileDatasetName(0, getChannelDatasetName(1, Qt::EditRole).toString());
//
//                    if (_profileDatasetName[1] == "")
//                        affectedColumns << setProfileDatasetName(1, getChannelDatasetName(2, Qt::EditRole).toString());
//
//                    break;
//                }
//
//                default:
//                    break;
//            }
//
//            affectedColumns << setProfileDatasetNames(0, getDatasetNames(0));
//            affectedColumns << setProfileDatasetNames(1, getDatasetNames(1));
//        }
//        else {
//            resetDifferentialProfile();
//        }
//    }
//
//    return affectedColumns;
//}