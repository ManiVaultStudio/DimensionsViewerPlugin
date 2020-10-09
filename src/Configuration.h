#pragma once

#include <QStringList>
#include <QColor>

class Configuration
{
protected:

	/** Channel settings */
	struct Channel {
		Channel(const bool& enabled, const QString& datasetName, const QColor& color) :
			_enabled(enabled),
			_datasetName(datasetName),
			_color(color)
		{
		}

		bool		_enabled;		/** Whether the channel is enabled or not */
		QString		_datasetName;	/** Channel dataset name */
		QColor		_color;			/** Channel color */
	};

	using Channels = QVector<Channel>;

public: // Enumerations

	/** Data columns */
	enum class Column {
		Channel1Enabled,				/** Whether the first subset is enabled */
		Channel2Enabled,				/** Whether the second subset is enabled */
		Channel3Enabled,				/** Whether the second subset is enabled */
		Channel1Dataset,				/** The dataset name of the first channel */
		Channel2Dataset,				/** The dataset name of the second channel */
		Channel3Dataset,				/** The dataset name of the third channel */
		Channel1Color,					/** Color of the first channel */
		Channel2Color,					/** Color of the second channel */
		Channel3Color,					/** Color of the third channel */
		Subsets,						/** The subset(s) of the first dataset */

		Start = Channel1Enabled,		/** Column start */
		End = Subsets					/** Column End */
	};

public: // Construction

	/** Default constructor */
	Configuration(const QString& datasetName);

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

private:
	Channels		_channels;		/** Channels */
};