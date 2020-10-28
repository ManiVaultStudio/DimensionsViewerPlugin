#pragma once

#include "Channel.h"

#include <QStringList>
#include <QColor>
#include <QModelIndex>

class Configuration : public QObject
{
	Q_OBJECT

public:

	using Channels = QVector<Channel*>;

	static const std::int32_t noChannels = 3;

public: // Columns

	/** Data columns */
	enum Column {
		ChannelEnabledStart,				                            /** Channel enabled for the first channel */
		ChannelEnabledEnd = ChannelEnabledStart + noChannels,           /** Channel enabled for the last channel */
		Subsets,						                                /** The subset(s) of the first dataset */
		Channel1DatasetName,			                                /** The dataset name of the first channel */
		Channel2DatasetName,			                                /** The dataset name of the second channel */
		Channel3DatasetName,			                                /** The dataset name of the third channel */
		Channel1DataName,				                                /** The data name of the first channel */
		Channel2DataName,				                                /** The data name of the second channel */
		Channel3DataName,				                                /** The data name of the third channel */
		Channel1Color,					                                /** Color of the first channel */
		Channel2Color,					                                /** Color of the second channel */
		Channel3Color,					                                /** Color of the third channel */
		Channel1Opacity,				                                /** Opacity of the first channel */
		Channel2Opacity,				                                /** Opacity of the second channel */
		Channel3Opacity,				                                /** Opacity of the third channel */
		Channel1ProfileType,			                                /** The profile type of the first channel */
		Channel2ProfileType,			                                /** The profile type of the second channel */
		Channel3ProfileType,			                                /** The profile type of the third channel */
		Channel1BandType,				                                /** The band type of the first channel */
		Channel2BandType,				                                /** The band type of the second channel */
		Channel3BandType,				                                /** The band type of the third channel */
		ChannelShowRangeStart,				                            /** Channel show range for the first channel */
		ChannelShowRangeEnd = ChannelShowRangeStart + noChannels,       /** Channel show range for the last channel */
		ChannelLockedStart,                                             /** Channel locked for the first channel */
		ChannelLockedEnd = ChannelLockedStart + noChannels,		        /** Channel locked for the last channel */
		
		Start = ChannelEnabledStart,                                    /** Column start */
		End = ChannelLockedEnd                                          /** Column end */
	};

	/** Get string representation of layer column enumeration */
	static QString getColumnName(const std::int32_t& column) {
		switch (column) {
			case Column::Subsets:
				return "Subsets";

			case Column::Channel1DatasetName:
				return "Channel 1: Dataset name";

			case Column::Channel2DatasetName:
				return "Channel 2: Dataset name";

			case Column::Channel3DatasetName:
				return "Channel 3: Dataset name";

			case Column::Channel1DataName:
				return "Channel 1: Data name";

			case Column::Channel2DataName:
				return "Channel 2: Data name";

			case Column::Channel3DataName:
				return "Channel 3: Data name";

			case Column::Channel1Color:
				return "Channel 1: Color";

			case Column::Channel2Color:
				return "Channel 2: Color";

			case Column::Channel3Color:
				return "Channel 3: Color";

			case Column::Channel1Opacity:
				return "Channel 1: Opacity";

			case Column::Channel2Opacity:
				return "Channel 2: Opacity";

			case Column::Channel3Opacity:
				return "Channel 3: Opacity";

			case Column::Channel1ProfileType:
				return "Channel 1: Profile type";

			case Column::Channel2ProfileType:
				return "Channel 2: Profile type";

			case Column::Channel3ProfileType:
				return "Channel 3: Profile type";

			case Column::Channel1BandType:
				return "Channel 1: Band type";

			case Column::Channel2BandType:
				return "Channel 2: Band type";

			case Column::Channel3BandType:
				return "Channel 3: Band type";

			default:
				return QString();
		}

		if (column >= Column::ChannelEnabledStart && column < Column::ChannelEnabledEnd)
			return QString("Channel %1: Enabled").arg(QString::number(column));

		if (column >= Column::ChannelShowRangeStart && column < Column::ChannelShowRangeEnd)
			return QString("Channel %1: Show range").arg(QString::number(column));

		if (column >= Column::ChannelLockedStart && column < Column::ChannelLockedStart)
			return QString("Channel %1: Locked").arg(QString::number(column));

		return QString();
	}

public: // Construction

	/**
	 * Constructor
	 * @param parent Parent object
	 * @param datasetName Name of the primary dataset
	 * @param dataName Name of the primary data
	 */
	Configuration(QObject* parent, const QString& datasetName, const QString& dataName);

public: // MVC

	/** Returns the number of columns */
	int getColumnCount() const { return static_cast<int>(Column::End) + 1; }

	/**
	 * Returns the item flags for the given model index
	 * @param index Model index
	 * @return Item flags for the index
	 */
	virtual Qt::ItemFlags getFlags(const QModelIndex& index) const;

	/**
	 * Returns the data for the given model index and data role
	 * @param index Model index
	 * @param role Data role
	 * @return Data in variant form
	 */
	virtual QVariant getData(const QModelIndex& index, const int& role) const;

	/**
	 * Sets the data value for the given model index and data role
	 * @param index Model index
	 * @param value Data value in variant form
	 * @param role Data role
	 * @return Model indices that are affected by the operation
	 */
	QModelIndexList setData(const QModelIndex& index, const QVariant& value, const int& role);

public: // Getters/setters

	/**
	 * Returns whether channel with \p channelIndex is enabled
	 * @param channelIndex Index of the channel
	 * @param role Data role
	 * @return Whether the channel is enabled in variant form
	 */
	QVariant getChannelEnabled(const std::int32_t& channelIndex, const std::int32_t& role) const;

	/**
	 * Sets whether channel with \p channelIndex is enabled
	 * @param channelIndex Index of the channel
	 * @param enabled Whether channel with \p channelIndex is enabled
	 */
	void setChannelEnabled(const std::int32_t& channelIndex, const bool& enabled);

	/**
	 * Returns the subset
	 * @param role Data role
	 * @return Subsets in variant form
	 */
	QVariant getSubsets(const std::int32_t& role) const;

	/**
	 * Sets the subsets
	 * @param subsets The subsets
	 */
	void setSubsets(const QStringList& subsets);

	/**
	 * Returns the dataset name of channel with \p channelIndex
	 * @param channelIndex Index of the channel
	 * @param role Data role
	 * @return Channel dataset name in variant form
	 */
	QVariant getChannelDatasetName(const std::int32_t& channelIndex, const std::int32_t& role) const;

	/**
	 * Sets the dataset name of channel with \p channelIndex
	 * @param channelIndex Index of the channel
	 * @param datasetName The dataset name of channel with \p channelIndex
	 */
	void setChannelDatasetName(const std::int32_t& channelIndex, const QString& datasetName);

	/**
	 * Returns the data name of channel with \p channelIndex
	 * @param channelIndex Index of the channel
	 * @param role Data role
	 * @return Channel data name in variant form
	 */
	QVariant getChannelDataName(const std::int32_t& channelIndex, const std::int32_t& role) const;

	/**
	 * Returns the color of channel with \p channelIndex
	 * @param channelIndex Index of the channel
	 * @param role Data role
	 * @return Channel color in variant form
	 */
	QVariant getChannelColor(const std::int32_t& channelIndex, const std::int32_t& role) const;

	/**
	 * Sets the color of channel with \p channelIndex
	 * @param channelIndex Index of the channel
	 * @param color The color of channel with \p channelIndex
	 */
	void setChannelColor(const std::int32_t& channelIndex, const QColor& color);

	/**
	 * Returns the opacity of channel with \p channelIndex
	 * @param channelIndex Index of the channel
	 * @param role Data role
	 * @return Channel opacity in variant form
	 */
	QVariant getChannelOpacity(const std::int32_t& channelIndex, const std::int32_t& role) const;

	/**
	 * Sets the opacity of channel with \p channelIndex
	 * @param channelIndex Index of the channel
	 * @param opacity The opacity of channel with \p channelIndex
	 */
	void setChannelOpacity(const std::int32_t& channelIndex, const float& opacity);

	/**
	 * Returns the profile type of channel with \p channelIndex
	 * @param channelIndex Index of the channel
	 * @param role Data role
	 * @return Profile type in variant form
	 */
	QVariant getChannelProfileType(const std::int32_t& channelIndex, const std::int32_t& role) const;

	/**
	 * Sets the profile type of channel with \p channelIndex
	 * @param channelIndex Index of the channel
	 * @param profileType The profile type of channel with \p channelIndex
	 */
	void setChannelProfileType(const std::int32_t& channelIndex, const Channel::ProfileType& profileType);

	/**
	 * Returns the band type of channel with \p channelIndex
	 * @param channelIndex Index of the channel
	 * @param role Data role
	 * @return Band type in variant form
	 */
	QVariant getChannelBandType(const std::int32_t& channelIndex, const std::int32_t& role) const;

	/**
	 * Sets the band type of channel with \p channelIndex
	 * @param channelIndex Index of the channel
	 * @param bandType The band type of channel with \p channelIndex
	 */
	void setChannelBandType(const std::int32_t& channelIndex, const Channel::BandType& bandType);

	/**
	 * Returns the show range parameter of channel with \p channelIndex
	 * @param channelIndex Index of the channel
	 * @param role Data role
	 * @return Show range in variant form
	 */
	QVariant getChannelShowRange(const std::int32_t& channelIndex, const std::int32_t& role) const;

	/**
	 * Sets the show range parameter of channel with \p channelIndex
	 * @param channelIndex Index of the channel
	 * @param showRange Show range parameter of channel with \p channelIndex
	 */
	void setChannelShowRange(const std::int32_t& channelIndex, const bool& showRange);

	/**
	 * Returns the locked parameter of channel with \p channelIndex
	 * @param channelIndex Index of the channel
	 * @param role Data role
	 * @return Locked in variant form
	 */
	QVariant getChannelLocked(const std::int32_t& channelIndex, const std::int32_t& role) const;

	/**
	 * Sets the locked parameter of channel with \p channelIndex
	 * @param channelIndex Index of the channel
	 * @param locked Locked parameter of channel with \p channelIndex
	 */
	void setChannelLocked(const std::int32_t& channelIndex, const bool& locked);

public: // Miscellaneous

	/** Get configuration channels */
	Channels& getChannels();

	/**
	 * Returns a channel by \p datasetName
	 * @param datasetName Name of the dataset to search for
	 */
	Channel* getChannelByDatasetName(const QString& datasetName);

	/**
	 * Returns whether dataset with name \p datasetName exists in the configuration
	 * @param datasetName Name of the dataset to search for
	 */
	bool hasDataset(const QString& datasetName) const;

public slots:

	/** Convert the state of the configuration to a variant map  */
	QVariantMap toVariantMap() const;

private:

	/**
	 * Returns an HTML tooltip
	 * @param title Title of the tooltip
	 * @param description Description of the tooltip
	 * @return HTML tooltip
	 */
	QString htmlTooltip(const QString& title, const QString& description) const;

signals:

	/**
	 * Signals that the configuration has changed
	 * @param configuration Configuration
	 */
	void changed(const QVariantMap& configuration);

private:
	Channels		_channels;				/** Channels */
	QStringList		_subsets;				/** Subsets of the primary dataset (selected in the first channel) */
	QVariantMap		_settings;				/** Settings for JS */
};