#pragma once

#include <QStringList>
#include <QColor>
#include <QModelIndex>

class Configuration
{
protected:

	/** Channel settings */
	struct Channel {
		Channel(const bool& enabled, const QString& datasetName, const QString& dataName, const QColor& color) :
			_enabled(enabled),
			_datasetName(datasetName),
			_dataName(dataName),
			_color(color)
		{
		}

		bool		_enabled;		/** Whether the channel is enabled or not */
		QString		_datasetName;	/** Channel dataset name */
		QString		_dataName;		/** Channel data name */
		QColor		_color;			/** Channel color */
	};

	using Channels = QVector<Channel>;

public: // Columns

	/** Data columns */
	enum class Column {
		Channel1Enabled,				/** Whether the first subset is enabled */
		Channel2Enabled,				/** Whether the second subset is enabled */
		Channel3Enabled,				/** Whether the second subset is enabled */
		Channel1DatasetName,			/** The dataset name of the first channel */
		Channel2DatasetName,			/** The dataset name of the second channel */
		Channel3DatasetName,			/** The dataset name of the third channel */
		Channel1DataName,				/** The data name of the first channel */
		Channel2DataName,				/** The data name of the second channel */
		Channel3DataName,				/** The data name of the third channel */
		Channel1Color,					/** Color of the first channel */
		Channel2Color,					/** Color of the second channel */
		Channel3Color,					/** Color of the third channel */
		Subsets,						/** The subset(s) of the first dataset */

		Start = Channel1Enabled,		/** Column start */
		End = Subsets					/** Column End */
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

			case Column::Subsets:
				return "Subsets";

			default:
				return QString();
		}

		return QString();
	}

public: // Construction

	/** Default constructor */
	Configuration(const QString& datasetName, const QString& dataName);

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

private:
	Channels		_channels;		/** Channels */
	QStringList		_subsets;		/** Subsets of the primary dataset (selected in the first channel) */
};