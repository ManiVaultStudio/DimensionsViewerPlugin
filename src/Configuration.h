#pragma once

#include "Channel.h"

#include <QStringList>
#include <QColor>
#include <QModelIndex>

/**
 * Configuration class
 *
 * This class represents a row of the configurations model
 *
 * @author T. Kroes
 */
class Configuration : public QObject
{
	Q_OBJECT

public:

	using Channels = QVector<Channel*>;

    /** The number of channels in the configuration */
	static const std::int32_t noChannels = 3;

public: // Columns

	/** Data columns */
	enum Column {
		Subsets,						                                    /** The subset(s) parameter of the first dataset */
		ChannelEnabledStart,				                                /** Channel enabled parameter of the first channel */
		ChannelEnabledEnd = ChannelEnabledStart + noChannels,               /** Channel enabled parameter of the last channel */
		ChannelDatasetNameStart,			                                /** Channel dataset name parameter of the first channel */
		ChannelDatasetNameEnd = ChannelDatasetNameStart + noChannels,       /** Channel dataset name parameter of the last channel */
		ChannelDataNameStart,				                                /** Channel data name parameter of the first channel */
		ChannelDataNameEnd = ChannelDataNameStart + noChannels,             /** Channel data name parameter of the last channel */
		ChannelColorStart,					                                /** Channel color parameter of the first channel */
		ChannelColorEnd = ChannelColorStart + noChannels,                   /** Channel color parameter of the last channel */
		ChannelOpacityStart,				                                /** Channel opacity parameter of the first channel */
		ChannelOpacityEnd = ChannelOpacityStart + noChannels,               /** Channel opacity parameter of the first channel */
		ChannelProfileTypeStart,			                                /** Channel profile type parameter of the first channel */
		ChannelProfileTypeEnd = ChannelProfileTypeStart + noChannels,       /** Channel profile type parameter of the second channel */
		ChannelBandTypeStart,				                                /** Channel band type parameter of the first channel */
		ChannelBandTypeEnd = ChannelBandTypeStart + noChannels,             /** Channel band type parameter of the second channel */
		ChannelShowRangeStart,				                                /** Channel show range parameter of the first channel */
		ChannelShowRangeEnd = ChannelShowRangeStart + noChannels,           /** Channel show range parameter of the last channel */
		ChannelLockedStart,                                                 /** Channel locked parameter of the first channel */
		ChannelLockedEnd = ChannelLockedStart + noChannels,		            /** Channel locked parameter of the last channel */
		SelectionStamp,		                                                /** Auxiliary column for triggering synchronization */
		ShowDimensionNames,		                                            /** Whether dimensions names are displayed in the viewer */
        ShowDifferentialProfile,                                            /** Whether to show the differential profile in the viewer */
        Profile1DatasetNames,                                               /** First profile available dataset names (for differential profile) */
        Profile1DatasetName,                                                /** First profile selected dataset name (for differential profile) */
        Profile2DatasetNames,                                               /** Second profile available dataset names (for differential profile) */
        Profile2DatasetName,                                                /** Second profile selected dataset name (for differential profile) */

		Start = Subsets,                                                    /** Column start */
		End = Profile2DatasetName                                           /** Column end */
	};

	/** Get string representation of layer column enumeration */
	static QString getColumnName(const std::int32_t& column) {
		if (column == Column::Subsets)
			return "Subsets";

		if (column >= Column::ChannelColorStart && column < Column::ChannelColorEnd)
			return QString("Channel %1: Color").arg(QString::number(column));

		if (column >= Column::ChannelOpacityStart && column < Column::ChannelOpacityEnd)
			return QString("Channel %1: Opacity").arg(QString::number(column));

		if (column >= Column::ChannelProfileTypeStart && column < Column::ChannelProfileTypeEnd)
			return QString("Channel %1: Profile type").arg(QString::number(column));

		if (column >= Column::ChannelBandTypeStart && column < Column::ChannelBandTypeEnd)
			return QString("Channel %1: Band type").arg(QString::number(column));

		if (column >= Column::ChannelDataNameStart && column < Column::ChannelDataNameEnd)
			return QString("Channel %1: Data name").arg(QString::number(column));

		if (column >= Column::ChannelDatasetNameStart && column < Column::ChannelDatasetNameEnd)
			return QString("Channel %1: Dataset name").arg(QString::number(column));

		if (column >= Column::ChannelEnabledStart && column < Column::ChannelEnabledEnd)
			return QString("Channel %1: Enabled").arg(QString::number(column));

		if (column >= Column::ChannelShowRangeStart && column < Column::ChannelShowRangeEnd)
			return QString("Channel %1: Show range").arg(QString::number(column));

		if (column >= Column::ChannelLockedStart && column < Column::ChannelLockedStart)
			return QString("Channel %1: Locked").arg(QString::number(column));

        if (column == Column::SelectionStamp)
            return "Selection stamp";

        if (column == Column::ShowDimensionNames)
            return "Show dimension names";

        if (column == Column::ShowDifferentialProfile)
            return "Show differential profile";

        if (column == Column::Profile1DatasetNames)
            return "Profile 1 dataset names";

        if (column == Column::Profile1DatasetName)
            return "Profile 1 dataset name";

        if (column == Column::Profile2DatasetNames)
            return "Profile 2 dataset names";

        if (column == Column::Profile2DatasetName)
            return "Profile 2 dataset name";

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

    /**
     * Gets whether dimensions names are displayed in the viewer
     * @param role Data role
     * @return Whether dimensions names are displayed in the viewer in variant form
     */
    QVariant getShowDimensionNames(const std::int32_t& role) const;

    /**
     * Sets whether dimensions names are displayed in the viewer
     * @param showDimensions Whether dimensions names are displayed in the viewer
     */
    void setShowDimensionNames(const bool& showDimensions);

    /** Gets whether to show a differential profile in the viewer
     * @param role Data role
     * @return Whether to show a differential profile in the viewer in variant form
     */
    QVariant getShowDifferentialProfile(const std::int32_t& role) const;

    /**
     * Sets whether to show a differential profile in the viewer
     * @param showDifferentialProfile Whether to show a differential profile in the viewer
     */
    void setShowDifferentialProfile(const bool& showDifferentialProfile);

    /** Gets selectable dataset names for profile with \p profileIndex
     * @param profileIndex Index of the profile
     * @param role Data role
     * @return Selectable dataset names for profile with \p profileIndex in variant form
     */
    QVariant getProfileDatasetNames(const std::int32_t& profileIndex, const std::int32_t& role) const;

    /**
     * Sets selectable dataset names for profile with \p profileIndex
     * @param profileIndex Index of the profile
     * @param datasetNames Selectable dataset names
     */
    void setProfileDatasetNames(const std::int32_t& profileIndex, const QStringList& datasetNames);

    /** Gets selected dataset name for profile with \p profileIndex
     * @param profileIndex Index of the profile
     * @param role Data role
     * @return Selected dataset name for profile with \p profileIndex in variant form
     */
    QVariant getProfileDatasetName(const std::int32_t& profileIndex, const std::int32_t& role) const;

    /**
     * Sets selectable dataset names for profile with \p profileIndex
     * @param profileIndex Index of the profile
     * @param datasetName Selectable dataset names
     */
    void setProfileDatasetName(const std::int32_t& profileIndex, const QString& datasetName);

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

public: // Spec

    /** Update configuration spec */
    void updateSpec();

	/** Get configuration spec */
	QVariantMap getSpec() const;

    /** Get modification time stamp */
    std::int32_t getModified() const;

private: // Internal

    /** Returns the number of displayable channels */
    std::int32_t getNoDisplayChannels() const;

    /** Returns whether a differential profile can be shown */
    bool canShowDifferentialProfile() const;

private:
    Channels		        _channels;                      /** Channels */
    QStringList		        _subsets;                       /** Subsets of the primary dataset (selected in the first channel) */
    bool		            _showDifferentialProfile;       /** Whether to show the differential profile in the viewer */
    QStringList             _profileDatasetNames[2];        /** Profile 1-2 dataset names (for differential profile) */
    QString                 _profileDatasetName[2];         /** Profile 1-2 selected dataset name (for differential profile) */
    bool                    _showDimensionNames;            /** Whether to show dimension names in the viewer */
    QVariantMap             _spec;                          /** Specification for use in JS visualization client (Vega) */

protected:
    static DimensionsViewerPlugin* dimensionsViewerPlugin;
    static std::int32_t maxNoDimensions;

    friend class DimensionsViewerPlugin;
    friend class ConfigurationsModel;
};