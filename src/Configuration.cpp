#include "Configuration.h"
#include "DimensionsViewerPlugin.h"

#include <QDebug>

Configuration::Configuration(const QString& datasetName) :
	_channels({ Channel(true, datasetName, Qt::cyan), Channel(false, "", Qt::red), Channel(false, "", Qt::yellow) })
{
}

QVariant Configuration::getChannelEnabled(const std::int32_t& channelIndex, const std::int32_t& role) const
{
	try
	{
		const auto enabled			= _channels[channelIndex]._enabled;
		const auto enabledString	= enabled ? "true" : "false";

		switch (role)
		{
			case Qt::DisplayRole:
				return enabledString;

			case Qt::EditRole:
				return enabled;

			case Qt::ToolTipRole:
				return QString("Channel %1 enabled: %2").arg(QString::number(channelIndex), enabledString);

			default:
				break;
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
		_channels[channelIndex]._enabled = enabled;
	}
	catch (std::exception exception)
	{
		qDebug() << exception.what();
	}
}

QVariant Configuration::getChannelDatasetName(const std::int32_t& channelIndex, const std::int32_t& role) const
{
	try
	{
		const auto datasetName = _channels[channelIndex]._datasetName;

		switch (role)
		{
			case Qt::DisplayRole:
				return datasetName;

			case Qt::EditRole:
				return datasetName;

			case Qt::ToolTipRole:
				return QString("Channel %1 dataset name: %2").arg(QString::number(channelIndex), datasetName);

			default:
				break;
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
		_channels[channelIndex]._datasetName = datasetName;
	}
	catch (std::exception exception)
	{
		qDebug() << exception.what();
	}
}

QVariant Configuration::getChannelColor(const std::int32_t& channelIndex, const std::int32_t& role) const
{
	try
	{
		const auto color		= _channels[channelIndex]._color;
		const auto colorString	= QString().arg(QString::number(color.red()), QString::number(color.green()), QString::number(color.blue()));

		switch (role)
		{
			case Qt::DisplayRole:
				return colorString;

			case Qt::EditRole:
				return color;

			case Qt::ToolTipRole:
				return QString("Channel %1 color: %2").arg(QString::number(channelIndex), colorString);

			default:
				break;
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
		_channels[channelIndex]._color = color;
	}
	catch (std::exception exception)
	{
		qDebug() << exception.what();
	}
}