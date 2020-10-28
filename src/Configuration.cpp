#include "Configuration.h"
#include "DimensionsViewerPlugin.h"

#include <QDebug>

Configuration::Configuration(QObject* parent, const QString& datasetName, const QString& dataName) :
	QObject(parent),
	_channels({ 
		new Channel(parent, 0, "Dataset", true, datasetName, dataName, Qt::black, 0.5f, false),
		new Channel(parent, 1, "Subset 1", false, "", dataName, QColor(249, 149, 0), 0.5f, true),
		new Channel(parent, 2, "Subset 2", false, "", dataName, QColor(0, 112, 249), 0.5f, true)
	}),
	_subsets(),
	_settings()
{
	for (auto channel : _channels) {
		QObject::connect(channel, &Channel::specChanged, [this]() {
			emit changed(toVariantMap());
		});
	}
}

Qt::ItemFlags Configuration::getFlags(const QModelIndex& index) const
{
	Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsEditable;

	const auto channel1Enabled = _channels[0]->isEnabled();
	const auto channel2Enabled = _channels[1]->isEnabled() && _subsets.size() >= 1;
	const auto channel3Enabled = _channels[2]->isEnabled() && _subsets.size() >= 2;

	switch (index.column()) {
		

		case Column::Channel1Color: {
			if (_channels[0]->isEnabled())
				flags |= Qt::ItemIsEnabled;

			break;
		}

		case Column::Channel2Color: {
			if (_channels[1]->isEnabled() && _subsets.size() >= 1)
				flags |= Qt::ItemIsEnabled;

			break;
		}

		case Column::Channel3Color: {
			if (_channels[2]->isEnabled() && _subsets.size() >= 2)
				flags |= Qt::ItemIsEnabled;

			break;
		}

		case Column::Channel1Opacity: {
			if (channel1Enabled)
				flags |= Qt::ItemIsEnabled;

			break;
		}

		case Column::Channel2Opacity: {
			if (_channels[1]->isEnabled() && _subsets.size() >= 1)
				flags |= Qt::ItemIsEnabled;

			break;
		}

		case Column::Channel3Opacity: {
			if (_channels[2]->isEnabled() && _subsets.size() >= 2)
				flags |= Qt::ItemIsEnabled;

			break;
		}

		case Column::Channel1ProfileType: {
			if (_channels[0]->isEnabled() || _channels[1]->isEnabled() || _channels[2]->isEnabled())
				flags |= Qt::ItemIsEnabled;

			break;
		}

		case Column::Channel2ProfileType: {
			if (channel2Enabled)
				flags |= Qt::ItemIsEnabled;

			break;
		}

		case Column::Channel3ProfileType: {
			if (channel3Enabled)
				flags |= Qt::ItemIsEnabled;

			break;
		}

		case Column::Channel1BandType: {
			if (_channels[0]->isEnabled() || _channels[1]->isEnabled() || _channels[2]->isEnabled())
				flags |= Qt::ItemIsEnabled;

			break;
		}

		case Column::Channel2BandType: {
			if (channel2Enabled)
				flags |= Qt::ItemIsEnabled;

			break;
		}

		case Column::Channel3BandType: {
			if (channel3Enabled)
				flags |= Qt::ItemIsEnabled;

			break;
		}

		default:
			break;
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

	if (index.column() >= Column::ChannelShowRangeStart && index.column() < Column::ChannelShowRangeEnd) {
		const auto channelIndex = index.column() - Column::ChannelShowRangeStart;
		
		if (channelIndex == 0) {
			flags |= Qt::ItemIsEnabled;
		}
		else {
			if (_channels[channelIndex]->isEnabled() && _subsets.size() >= channelIndex)
				flags |= Qt::ItemIsEnabled;
		}
	}

	if (index.column() >= Column::ChannelLockedStart && index.column() < Column::ChannelLockedEnd) {
		const auto channelIndex = index.column() - Column::ChannelLockedStart;

		if (channelIndex >= 1) {
			if (_channels[channelIndex]->isEnabled() && _subsets.size() >= channelIndex)
				flags |= Qt::ItemIsEnabled;
		}
	}

	return flags;
}

QVariant Configuration::getData(const QModelIndex& index, const int& role) const
{
	switch (static_cast<Column>(index.column())) {
		case Column::Subsets:
			return getSubsets(role);

		case Column::Channel1DataName:
			return getChannelDataName(0, role);

		case Column::Channel2DataName:
			return getChannelDataName(1, role);

		case Column::Channel3DataName:
			return getChannelDataName(2, role);

		case Column::Channel1Color:
			return getChannelColor(0, role);

		case Column::Channel2Color:
			return getChannelColor(1, role);

		case Column::Channel3Color:
			return getChannelColor(2, role);

		case Column::Channel1Opacity:
			return getChannelOpacity(0, role);

		case Column::Channel2Opacity:
			return getChannelOpacity(1, role);

		case Column::Channel3Opacity:
			return getChannelOpacity(2, role);

		case Column::Channel1ProfileType:
			return getChannelProfileType(0, role);

		case Column::Channel2ProfileType:
			return getChannelProfileType(1, role);

		case Column::Channel3ProfileType:
			return getChannelProfileType(2, role);

		case Column::Channel1BandType:
			return getChannelBandType(0, role);

		case Column::Channel2BandType:
			return getChannelBandType(1, role);

		case Column::Channel3BandType:
			return getChannelBandType(2, role);

		default:
			break;
	}

	if (index.column() >= Column::ChannelDatasetNameStart && index.column() < Column::ChannelDatasetNameEnd)
		return getChannelDatasetName(index.column() - Column::ChannelDatasetNameStart, role);

	if (index.column() >= Column::ChannelEnabledStart && index.column() < Column::ChannelEnabledEnd)
		return getChannelEnabled(index.column() - Column::ChannelEnabledStart, role);

	if (index.column() >= Column::ChannelShowRangeStart && index.column() < Column::ChannelShowRangeEnd)
		return getChannelShowRange(index.column() - Column::ChannelShowRangeStart, role);

	if (index.column() >= Column::ChannelLockedStart && index.column() < Column::ChannelLockedEnd)
		return getChannelLocked(index.column() - Column::ChannelLockedStart, role);

	return QVariant();
}

QModelIndexList Configuration::setData(const QModelIndex& index, const QVariant& value, const int& role)
{
	QModelIndexList affectedIndices{ index };

	const auto column = static_cast<Column>(index.column());

	switch (role)
	{
		case Qt::EditRole:
		{
			switch (column) {
				case Column::Subsets: {
					setSubsets(value.toStringList());

					for (int s = 0; s < _subsets.size(); ++s) {
						affectedIndices << index.siblingAtColumn(Column::ChannelEnabledStart + 1);
						affectedIndices << index.siblingAtColumn(Column::ChannelDatasetNameStart + 1);
						affectedIndices << index.siblingAtColumn(Column::ChannelShowRangeStart + s + 1);
						affectedIndices << index.siblingAtColumn(Column::ChannelLockedStart + s + 1);
					}

					switch (_subsets.size())
					{
						case 1: {
							setChannelDatasetName(1, _subsets[0]);
							setChannelEnabled(1, true);

							affectedIndices << index.siblingAtColumn(Column::Channel2Color);
							affectedIndices << index.siblingAtColumn(Column::Channel2Opacity);
							affectedIndices << index.siblingAtColumn(Column::Channel2ProfileType);
							affectedIndices << index.siblingAtColumn(Column::Channel2BandType);
							break;
						}

						case 2: {
							setChannelDatasetName(1, _subsets[0]);
							setChannelDatasetName(2, _subsets[1]);

							setChannelEnabled(1, true);
							setChannelEnabled(2, true);

							affectedIndices << index.siblingAtColumn(Column::Channel2Color);
							affectedIndices << index.siblingAtColumn(Column::Channel2Opacity);
							affectedIndices << index.siblingAtColumn(Column::Channel2ProfileType);
							affectedIndices << index.siblingAtColumn(Column::Channel2BandType);
							
							affectedIndices << index.siblingAtColumn(Column::Channel3Color);
							affectedIndices << index.siblingAtColumn(Column::Channel3Opacity);
							affectedIndices << index.siblingAtColumn(Column::Channel3ProfileType);
							affectedIndices << index.siblingAtColumn(Column::Channel3BandType);

							break;
						}

						default:
							break;
					}

					//affectedIndices << index.siblingAtColumn(static_cast<int>(Column::GlobalSettings));

					break;
				}

				case Column::Channel1Color: {
					setChannelColor(0, value.value<QColor>());
					break;
				}

				case Column::Channel2Color: {
					setChannelColor(1, value.value<QColor>());
					break;
				}

				case Column::Channel3Color: {
					setChannelColor(2, value.value<QColor>());
					break;
				}

				case Column::Channel1Opacity: {
					setChannelOpacity(0, value.toFloat());
					break;
				}

				case Column::Channel2Opacity: {
					setChannelOpacity(1, value.toFloat());
					break;
				}

				case Column::Channel3Opacity: {
					setChannelOpacity(2, value.toFloat());
					break;
				}

				case Column::Channel1ProfileType: {
					const auto profileType = static_cast<Channel::ProfileType>(value.toInt());

					setChannelProfileType(0, profileType);

					/*if (_globalSettings) {
						setChannelProfileType(1, profileType);
						setChannelProfileType(2, profileType);

						affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel2ProfileType));
						affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel3ProfileType));
					}*/

					break;
				}
				
				case Column::Channel2ProfileType: {
					setChannelProfileType(1, static_cast<Channel::ProfileType>(value.toInt()));
					break;
				}

				case Column::Channel3ProfileType: {
					setChannelProfileType(2, static_cast<Channel::ProfileType>(value.toInt()));
					break;
				}

				case Column::Channel1BandType: {
					const auto bandType = static_cast<Channel::BandType>(value.toInt());

					setChannelBandType(0, bandType);

					/*if (_globalSettings) {
						setChannelBandType(1, bandType);
						setChannelBandType(2, bandType);

						affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel2BandType));
						affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel3BandType));
					}*/

					break;
				}

				case Column::Channel2BandType: {
					setChannelBandType(1, static_cast<Channel::BandType>(value.toInt()));
					break;
				}

				case Column::Channel3BandType: {
					setChannelBandType(2, static_cast<Channel::BandType>(value.toInt()));
					break;
				}

				default:
					break;
			}

			break;
		}

		default:
			break;
	}

	if (index.column() >= Column::ChannelEnabledStart && index.column() < Column::ChannelEnabledEnd) {
		const auto channelIndex = index.column() - Column::ChannelEnabledStart;

		setChannelEnabled(channelIndex, value.toBool());

		//affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel1DatasetName));
		//affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel1Color));
		//affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel1Opacity));
		//affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel1ProfileType));
		//affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel1BandType));
		affectedIndices << index.siblingAtColumn(static_cast<int>(Column::ChannelShowRangeStart + channelIndex));
		affectedIndices << index.siblingAtColumn(static_cast<int>(Column::ChannelLockedStart + channelIndex));
	}

	if (index.column() >= Column::ChannelDatasetNameStart && index.column() < Column::ChannelDatasetNameEnd) {
		const auto channelIndex = index.column() - Column::ChannelDatasetNameStart;

		setChannelDatasetName(channelIndex, value.toString());
	}

	if (index.column() >= Column::ChannelShowRangeStart && index.column() < Column::ChannelShowRangeEnd) {
		const auto channelIndex = index.column() - Column::ChannelShowRangeStart;
		const auto showRange    = value.toBool();

		setChannelShowRange(channelIndex, showRange);

		for (int c = 0; c < Configuration::noChannels; ++c) {
			if (getChannelLocked(channelIndex, Qt::EditRole).toBool()) {
				setChannelShowRange(channelIndex, showRange);
				affectedIndices << index.siblingAtColumn(static_cast<int>(Column::ChannelShowRangeStart + channelIndex));
			}
		}
	}

	return affectedIndices;
}

QVariant Configuration::getChannelEnabled(const std::int32_t& channelIndex, const std::int32_t& role) const
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
				return QString("%1 is %2").arg(_channels[channelIndex]->getDisplayName(), enabledString);

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
				return QString("%1 dataset name: %2").arg(_channels[channelIndex]->getDisplayName(), datasetName);

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
				return QString("%1 data name: %2").arg(_channels[channelIndex]->getDisplayName(), dataName);

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
				return QString("%1 color: %2").arg(_channels[channelIndex]->getDisplayName(), colorString);

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
				return QString("%1 opacity: %2").arg(_channels[channelIndex]->getDisplayName(), opacityString);

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
				return QString("%1 profile type: %2").arg(_channels[channelIndex]->getDisplayName(), profileTypeString);

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

QVariant Configuration::getChannelBandType(const std::int32_t& channelIndex, const std::int32_t& role) const
{
	try
	{
		const auto bandType			= _channels[channelIndex]->getBandType();
		const auto bandTypeString	= Channel::getBandTypeName(bandType);

		switch (role)
		{
			case Qt::DisplayRole:
				return bandTypeString;

			case Qt::EditRole:
				return static_cast<int>(bandType);

			case Qt::ToolTipRole:
				return QString("%1 band type: %2").arg(_channels[channelIndex]->getDisplayName(), bandTypeString);

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

void Configuration::setChannelBandType(const std::int32_t& channelIndex, const Channel::BandType& bandType)
{
	try
	{
		_channels[channelIndex]->setBandType(bandType);
	}
	catch (std::exception exception)
	{
		qDebug() << exception.what();
	}
}

QVariant Configuration::getChannelShowRange(const std::int32_t& channelIndex, const std::int32_t& role) const
{
	try
	{
		const auto showRange		= _channels[channelIndex]->getShowRange();
		const auto showRangeString	= showRange ? "on" : "off";

		switch (role)
		{
			case Qt::DisplayRole:
				return showRangeString;

			case Qt::EditRole:
				return showRange;

			case Qt::ToolTipRole:
				return QString("%1 show range: %2").arg(_channels[channelIndex]->getDisplayName(), showRangeString);

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

void Configuration::setChannelShowRange(const std::int32_t& channelIndex, const bool& showRange)
{
	try
	{
		_channels[channelIndex]->setShowRange(showRange);
	}
	catch (std::exception exception)
	{
		qDebug() << exception.what();
	}
}

QVariant Configuration::getChannelLocked(const std::int32_t& channelIndex, const std::int32_t& role) const
{
	try
	{
		const auto locked           = _channels[channelIndex]->isLocked();
		const auto lockedString     = locked ? "on" : "off";

		switch (role)
		{
			case Qt::DisplayRole:
				return lockedString;

			case Qt::EditRole:
				return locked;

			case Qt::ToolTipRole:
				return QString("%1 locked: %2").arg(_channels[channelIndex]->getDisplayName(), lockedString);

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

void Configuration::setChannelLocked(const std::int32_t& channelIndex, const bool& locked)
{
	try
	{
		_channels[channelIndex]->setLocked(locked);
	}
	catch (std::exception exception)
	{
		qDebug() << exception.what();
	}
}

Configuration::Channels& Configuration::getChannels()
{
	return _channels;
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

QVariantMap Configuration::toVariantMap() const
{
	QVariantMap configuration, channels;

	QStringList datasets;

	for (auto channel : _channels) {
		if (!channel->isEnabled())
			continue;

		channels[channel->getInternalName()] = channel->getSpec();

		datasets << channel->getDatasetName();
	}

	configuration["title"]		= datasets.join(", ");
	configuration["channels"]	= channels;

	//qDebug() << configuration;

	return configuration;
}

QString Configuration::htmlTooltip(const QString& title, const QString& description) const
{
	return QString("<html><head/><body><p><span style='font-weight:600;'>%1<br/></span>%2</p></body></html>").arg(title, description);
}