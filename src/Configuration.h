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

public: // Columns

	/** Data columns */
	enum class Column {
		Channel1Enabled,				/** Whether the first subset is enabled */
		Channel2Enabled,				/** Whether the second subset is enabled */
		Channel3Enabled,				/** Whether the second subset is enabled */
		Subsets,						/** The subset(s) of the first dataset */
		Channel1DatasetName,			/** The dataset name of the first channel */
		Channel2DatasetName,			/** The dataset name of the second channel */
		Channel3DatasetName,			/** The dataset name of the third channel */
		Channel1DataName,				/** The data name of the first channel */
		Channel2DataName,				/** The data name of the second channel */
		Channel3DataName,				/** The data name of the third channel */
		Channel1Color,					/** Color of the first channel */
		Channel2Color,					/** Color of the second channel */
		Channel3Color,					/** Color of the third channel */
		Channel1ProfileType,			/** The profile type of the first channel */
		Channel2ProfileType,			/** The profile type of the second channel */
		Channel3ProfileType,			/** The profile type of the third channel */
		Channel1BandType,				/** The band type of the first channel */
		Channel2BandType,				/** The band type of the second channel */
		Channel3BandType,				/** The band type of the third channel */
		Channel1ShowRange,				/** Whether to show the dimension range for the first channel*/
		Channel2ShowRange,				/** Whether to show the dimension range for the first channel*/
		Channel3ShowRange,				/** Whether to show the dimension range for the first channel*/
		GlobalSettings,					/** Whether settings of the first channel are global settings */
		
		Start = Channel1Enabled,		/** Column start */
		End = GlobalSettings			/** Column End */
	};

	/** Get string representation of layer column enumeration */
	static QString getColumnName(const Column& column) {
		switch (column) {
			case Column::Channel1Enabled:
				return "Channel 1: Enabled";

			case Column::Channel2Enabled:
				return "Channel 2: Enabled";

			case Column::Channel3Enabled:
				return "Channel 3: Enabled";

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

			case Column::Channel1ShowRange:
				return "Channel 1: Show range";

			case Column::Channel2ShowRange:
				return "Channel 2: Show range";

			case Column::Channel3ShowRange:
				return "Channel 3: Show range";

			case Column::GlobalSettings:
				return "Global settings";

			default:
				return QString();
		}

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
	 * Returns whether the settings of the first channel are global or not
	 * @param role Data role
	 * @return Whether settings of the first channel are global or not in variant form
	 */
	QVariant getGlobalSettings(const std::int32_t& role) const;

	/**
	 * Sets whether the settings of the first channel are global or not
	 * @param globalSettings Whether the settings of the first channel are global or not
	 */
	void setGlobalSettings(const bool& globalSettings);

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
	 * Signals that the dimensions have changed
	 * @param dimensions Channel dimensions
	 */
	void dimensionsChanged(const QVariantList& dimensions);

	/**
	 * Signals that the settings have changed
	 * @param settings Channel settings
	 */
	void settingsChanged(const QVariantMap& settings);

private:
	Channels		_channels;				/** Channels */
	QStringList		_subsets;				/** Subsets of the primary dataset (selected in the first channel) */
	bool			_globalSettings;		/** Whether the range settings of the first channel determine the subsequent channel range settings */
	QVariantMap		_settings;				/** Settings for JS */
};