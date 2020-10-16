#include "Configuration.h"
#include "DimensionsViewerPlugin.h"

#include <QDebug>

Configuration::Configuration(QObject* parent, const QString& datasetName, const QString& dataName) :
	QObject(parent),
	_channels({ 
		new Channel(parent, "Dataset", true, datasetName, dataName, Qt::black),
		new Channel(parent, "Subset 1", false, "", dataName, QColor(249, 149, 0)), 
		new Channel(parent, "Subset 2", false, "", dataName, QColor(0, 112, 249))
	}),
	_subsets(),
	_globalSettings(true)
{
}

Qt::ItemFlags Configuration::getFlags(const QModelIndex& index) const
{
	Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsEditable;

	const auto channel2Enabled = !_globalSettings && _channels[1]->isEnabled() && _subsets.size() >= 1;
	const auto channel3Enabled = !_globalSettings && _channels[2]->isEnabled() && _subsets.size() >= 2;

	switch (static_cast<Column>(index.column())) {
		case Column::Channel1Enabled: {
			flags |= Qt::ItemIsEnabled;
			
			break;
		}

		case Column::Channel2Enabled: {
			if (_subsets.size() >= 1)
				flags |= Qt::ItemIsEnabled;

			break;
		}

		case Column::Channel3Enabled: {
			if (_subsets.size() >= 2)
				flags |= Qt::ItemIsEnabled;

			break;
		}

		case Column::Channel1DatasetName: {
			flags |= Qt::ItemIsEnabled;

			break;
		}

		case Column::Channel2DatasetName: {
			if (_channels[1]->isEnabled() && _subsets.size() >= 1)
				flags |= Qt::ItemIsEnabled;

			break;
		}
		
		case Column::Channel3DatasetName: {
			if (_channels[2]->isEnabled() && _subsets.size() >= 2)
				flags |= Qt::ItemIsEnabled;

			break;
		}

		case Column::Channel1Color: {
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

		case Column::Channel1ProfileType: {
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

		case Column::GlobalSettings: {
			if (!_subsets.isEmpty())
				flags |= Qt::ItemIsEnabled;

			break;
		}
		
		default:
			break;
	}

	return flags;
}

QVariant Configuration::getData(const QModelIndex& index, const int& role) const
{
	switch (static_cast<Column>(index.column())) {
		case Column::Channel1Enabled:
			return getChannelEnabled(0, role);

		case Column::Channel2Enabled:
			return getChannelEnabled(1, role);

		case Column::Channel3Enabled:
			return getChannelEnabled(2, role);
		
		case Column::Subsets:
			return getSubsets(role);

		case Column::Channel1DatasetName:
			return getChannelDatasetName(0, role);

		case Column::Channel2DatasetName:
			return getChannelDatasetName(1, role);

		case Column::Channel3DatasetName:
			return getChannelDatasetName(2, role);

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

		case Column::GlobalSettings:
			return getGlobalSettings(role);

		default:
			break;
	}

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
				case Column::Channel1Enabled: {
					setChannelEnabled(0, value.toBool());
					affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel1DatasetName));
					break;
				}

				case Column::Channel2Enabled: {
					setChannelEnabled(1, value.toBool());

					affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel2DatasetName));
					affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel2Color));
					affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel2ProfileType));
					affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel2BandType));
					break;
				}

				case Column::Channel3Enabled: {
					setChannelEnabled(2, value.toBool());
					
					affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel3DatasetName));
					affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel3Color));
					affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel3ProfileType));
					affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel3BandType));
					break;
				}

				case Column::Subsets: {
					setSubsets(value.toStringList());

					switch (_subsets.size())
					{
						case 1: {
							setChannelDatasetName(1, _subsets[0]);
							setChannelEnabled(1, true);

							affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel2Enabled));
							affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel2DatasetName));
							affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel2Color));
							affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel2ProfileType));
							affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel2BandType));
							break;
						}

						case 2: {
							setChannelDatasetName(1, _subsets[0]);
							setChannelDatasetName(2, _subsets[1]);

							setChannelEnabled(1, true);
							setChannelEnabled(2, true);

							affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel2Enabled));
							affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel2DatasetName));
							affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel2Color));
							affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel2ProfileType));
							affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel2BandType));
							affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel3Enabled));
							affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel3DatasetName));
							affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel3Color));
							affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel3ProfileType));
							affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel3BandType));
							break;
						}

						default:
							break;
					}

					affectedIndices << index.siblingAtColumn(static_cast<int>(Column::GlobalSettings));

					break;
				}

				case Column::Channel1DatasetName: {
					setChannelDatasetName(0, value.toString());
					break;
				}

				case Column::Channel2DatasetName: {
					setChannelDatasetName(1, value.toString());
					break;
				}

				case Column::Channel3DatasetName: {
					setChannelDatasetName(2, value.toString());
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

				case Column::Channel1ProfileType: {
					const auto profileType = static_cast<Channel::ProfileType>(value.toInt());

					setChannelProfileType(0, profileType);

					if (_globalSettings) {
						setChannelProfileType(1, profileType);
						setChannelProfileType(2, profileType);

						affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel2ProfileType));
						affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel3ProfileType));
					}

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

					if (_globalSettings) {
						setChannelBandType(1, bandType);
						setChannelBandType(2, bandType);

						affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel2BandType));
						affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel3BandType));
					}

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

				case Column::GlobalSettings: {
					setGlobalSettings(value.toBool());
					setChannelProfileType(1, _channels.first()->getProfileType());
					setChannelProfileType(2, _channels.first()->getProfileType());
					setChannelBandType(1, _channels.first()->getBandType());
					setChannelBandType(2, _channels.first()->getBandType());

					affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel2ProfileType));
					affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel2BandType));
					affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel3ProfileType));
					affectedIndices << index.siblingAtColumn(static_cast<int>(Column::Channel3BandType));
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
				return QString("%1 is %2").arg(_channels[channelIndex]->getName(), enabledString);

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
				return QString("%1 dataset name: %2").arg(_channels[channelIndex]->getName(), datasetName);

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
				return QString("%1 data name: %2").arg(_channels[channelIndex]->getName(), dataName);

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
				return QString("%1 color: %2").arg(_channels[channelIndex]->getName(), colorString);

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
				return QString("%1 profile type: %2").arg(_channels[channelIndex]->getName(), profileTypeString);

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
				return QString("%1 band type: %2").arg(_channels[channelIndex]->getName(), bandTypeString);

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

QVariant Configuration::getGlobalSettings(const std::int32_t& role) const
{
	const auto globalSettingsString = _globalSettings ? "on" : "off";

	switch (role)
	{
		case Qt::DisplayRole:
			return globalSettingsString;

		case Qt::EditRole:
			return _globalSettings;

		case Qt::ToolTipRole:
			return QString("Global settings are %1").arg(globalSettingsString);

		default:
			return QVariant();
	}

	return QVariant();
}

void Configuration::setGlobalSettings(const bool& globalSettings)
{
	_globalSettings = globalSettings;
}

Configuration::Channels& Configuration::getChannels()
{
	return _channels;
}

QString Configuration::htmlTooltip(const QString& title, const QString& description) const
{
	return QString("<html><head/><body><p><span style='font-weight:600;'>%1<br/></span>%2</p></body></html>").arg(title, description);
}
