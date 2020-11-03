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

public: // Aliases

    /** Vector of channels */
	using Channels = QVector<Channel*>;

    /** Denotes which columns are affected by a change in model data */
    using AffectedColumns = QVector<std::int32_t>;

    /** The number of channels in the configuration */
	static const std::int32_t noChannels = 3;

public: // Columns

	/** Data columns */
	enum Column {
		Subsets,						                                    /** The subset(s) parameter of the first dataset */
        ChannelNameStart,				                                    /** Channel name first column */
        ChannelNameEnd = ChannelNameStart + noChannels,                     /** Channel name last column */
		ChannelEnabledStart,				                                /** Channel enabled first column */
		ChannelEnabledEnd = ChannelEnabledStart + noChannels,               /** Channel enabled last column */
		ChannelDatasetNameStart,			                                /** Channel dataset name first column */
		ChannelDatasetNameEnd = ChannelDatasetNameStart + noChannels,       /** Channel dataset name last column */
		ChannelDataNameStart,				                                /** Channel data name first column */
		ChannelDataNameEnd = ChannelDataNameStart + noChannels,             /** Channel data name last column */
		ChannelColorStart,					                                /** Channel color first column */
		ChannelColorEnd = ChannelColorStart + noChannels,                   /** Channel color last column */
		ChannelOpacityStart,				                                /** Channel opacity first column */
		ChannelOpacityEnd = ChannelOpacityStart + noChannels,               /** Channel opacity last column */
		ChannelProfileTypeStart,			                                /** Channel profile type first column */
		ChannelProfileTypeEnd = ChannelProfileTypeStart + noChannels,       /** Channel profile type last column */
        ChannelRangeTypeStart,				                                /** Channel range type first column */
        ChannelRangeTypeEnd = ChannelRangeTypeStart + noChannels,           /** Channel range type last column */
        ChannelSettingsStart,				                                /** Channel settings type first column */
        ChannelSettingsEnd = ChannelSettingsStart + noChannels,             /** Channel settings type last column */
        GlobalSettings,                                                     /** Whether whether global settings are on or off */
        GlobalProfileType,                                                  /** Global profile type */
        GlobalRangeType,                                                    /** Global range type */
		SelectionStamp,		                                                /** Auxiliary column for triggering synchronization */
		ShowDimensionNames,		                                            /** Whether dimensions names are displayed in the viewer */
        ShowDifferentialProfile,                                            /** Whether to show the differential profile in the viewer */
        Profile1DatasetNames,                                               /** First profile available dataset names (for differential profile) */
        Profile2DatasetNames,                                               /** Second profile available dataset names (for differential profile) */
        Profile1DatasetName,                                                /** First profile selected dataset name (for differential profile) */
        Profile2DatasetName,                                                /** Second profile selected dataset name (for differential profile) */

		Start = Subsets,                                                    /** Column start */
		End = Profile2DatasetName                                           /** Column end */
	};

	/** Get string representation of layer column enumeration */
	static QString getColumnName(const std::int32_t& column) {
		if (column == Column::Subsets)
			return "Subsets";

        if (column >= Column::ChannelNameStart && column < Column::ChannelNameEnd)
            return QString("Channel %1 Name").arg(QString::number(column - Column::ChannelNameStart + 1));

        if (column >= Column::ChannelEnabledStart && column < Column::ChannelEnabledEnd)
            return QString("Channel %1 Enabled").arg(QString::number(column - Column::ChannelEnabledStart + 1));

		if (column >= Column::ChannelColorStart && column < Column::ChannelColorEnd)
			return QString("Channel %1 Color").arg(QString::number(column - Column::ChannelColorStart + 1));

		if (column >= Column::ChannelOpacityStart && column < Column::ChannelOpacityEnd)
			return QString("Channel %1 Opacity").arg(QString::number(column - Column::ChannelOpacityStart + 1));

		if (column >= Column::ChannelProfileTypeStart && column < Column::ChannelProfileTypeEnd)
			return QString("Channel %1 Profile type").arg(QString::number(column - Column::ChannelProfileTypeStart + 1));

		if (column >= Column::ChannelRangeTypeStart && column < Column::ChannelRangeTypeEnd)
			return QString("Channel %1 Range type").arg(QString::number(column - Column::ChannelRangeTypeStart + 1));

		if (column >= Column::ChannelDataNameStart && column < Column::ChannelDataNameEnd)
			return QString("Channel %1 Data name").arg(QString::number(column - Column::ChannelDataNameStart + 1));

		if (column >= Column::ChannelDatasetNameStart && column < Column::ChannelDatasetNameEnd)
			return QString("Channel %1 Dataset name").arg(QString::number(column - Column::ChannelDatasetNameStart + 1));

        if (column >= Column::ChannelSettingsStart && column < Column::ChannelSettingsEnd)
            return QString("Channel %1 settings").arg(QString::number(column - Column::ChannelSettingsStart + 1));

        if (column == Column::GlobalSettings)
            return "Global settings";

        if (column == Column::GlobalProfileType)
            return "Global profile type";

        if (column == Column::GlobalRangeType)
            return "Global range type";

        if (column == Column::SelectionStamp)
            return "Selection stamp";

        if (column == Column::ShowDimensionNames)
            return "Show dimension names";

        if (column == Column::ShowDifferentialProfile)
            return "Show differential profile";

        if (column == Column::Profile1DatasetNames)
            return "Profile 1 dataset names";

        if (column == Column::Profile2DatasetName)
            return "Profile 2 dataset name";

        if (column == Column::Profile1DatasetName)
            return "Profile 1 dataset name";

        if (column == Column::Profile2DatasetNames)
            return "Profile 2 dataset names";

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
	 * Gets channel enabled
	 * @param channelIndex Index of the channel
	 * @param role Data role
	 * @return Channel enabled in variant form
	 */
	QVariant isChannelEnabled(const std::int32_t& channelIndex, const std::int32_t& role) const;

	/**
	 * Sets channel enabled
	 * @param channelIndex Index of the channel
	 * @param enabled Channel enabled for profile with \p profileIndex
	 * @return Columns that are affected by the operation
	 */
	Configuration::AffectedColumns setChannelEnabled(const std::int32_t& channelIndex, const bool& enabled);

    /**
     * Gets channel name
     * @param channelIndex Index of the channel
     * @param role Data role
     * @return Channel name
     */
    QVariant getChannelName(const std::int32_t& channelIndex, const std::int32_t& role) const;

    /**
     * Sets channel name
     * @param channelIndex Index of the channel
     * @param name Channel name
     * @return Columns that are affected by the operation
     */
    Configuration::AffectedColumns setChannelName(const std::int32_t& channelIndex, const QString& name);

	/**
	 * Returns subsets
	 * @param role Data role
	 * @return Subsets in variant form
	 */
	QVariant getSubsets(const std::int32_t& role) const;

	/**
	 * Sets subsets
	 * @param subsets Subsets
     * @return Columns that are affected by the operation
	 */
	Configuration::AffectedColumns setSubsets(const QStringList& subsets);

	/**
	 * Gets channel dataset name
	 * @param channelIndex Index of the channel
	 * @param role Data role
	 * @return Channel dataset name in variant form
	 */
	QVariant getChannelDatasetName(const std::int32_t& channelIndex, const std::int32_t& role) const;

	/**
	 * Sets channel dataset name
	 * @param channelIndex Index of the channel
	 * @param datasetName Channel dataset name
     * @return Columns that are affected by the operation
	 */
	Configuration::AffectedColumns setChannelDatasetName(const std::int32_t& channelIndex, const QString& datasetName);

	/**
	 * Gets a channel data name
	 * @param channelIndex Index of the channel
	 * @param role Data role
	 * @return Channel data name in variant form
	 */
	QVariant getChannelDataName(const std::int32_t& channelIndex, const std::int32_t& role) const;

	/**
	 * Gets channel color
	 * @param channelIndex Index of the channel
	 * @param role Data role
	 * @return Channel color in variant form
	 */
	QVariant getChannelColor(const std::int32_t& channelIndex, const std::int32_t& role) const;

	/**
	 * Sets channel color
	 * @param channelIndex Index of the channel
	 * @param color Channel color
     * @return Columns that are affected by the operation
	 */
	Configuration::AffectedColumns setChannelColor(const std::int32_t& channelIndex, const QColor& color);

	/**
	 * Gets channel opacity
	 * @param channelIndex Index of the channel
	 * @param role Data role
	 * @return Channel opacity in variant form
	 */
	QVariant getChannelOpacity(const std::int32_t& channelIndex, const std::int32_t& role) const;

	/**
	 * Sets the opacity of channel with \p channelIndex
	 * @param channelIndex Index of the channel
	 * @param opacity The opacity of channel with \p channelIndex
     * @return Columns that are affected by the operation
	 */
	Configuration::AffectedColumns setChannelOpacity(const std::int32_t& channelIndex, const float& opacity);

	/**
	 * Gets channel profile type
	 * @param channelIndex Index of the channel
	 * @param role Data role
	 * @return Channel profile type in variant form
	 */
	QVariant getChannelProfileType(const std::int32_t& channelIndex, const std::int32_t& role) const;

	/**
	 * Sets channel profile type
	 * @param channelIndex Index of the channel
	 * @param profileType Channel profile type
     * @return Columns that are affected by the operation
	 */
	Configuration::AffectedColumns setChannelProfileType(const std::int32_t& channelIndex, const Channel::ProfileType& profileType);

	/**
	 * Gets channel range type
	 * @param channelIndex Index of the channel
	 * @param role Data role
	 * @return Channel range type in variant form
	 */
	QVariant getChannelRangeType(const std::int32_t& channelIndex, const std::int32_t& role) const;

	/**
	 * Sets channel range type
	 * @param channelIndex Index of the channel
	 * @param rangeType Channel range type
     * @return Columns that are affected by the operation
	 */
	Configuration::AffectedColumns setChannelRangeType(const std::int32_t& channelIndex, const Channel::RangeType& rangeType);

	/**
	 * Gets global settings
	 * @param role Data role
	 * @return Global settings
	 */
	QVariant getGlobalSettings(const std::int32_t& role) const;

	/**
	 * Sets global settings
	 * @param globalSettings Global settings (on/off)
     * @return Columns that are affected by the operation
	 */
	Configuration::AffectedColumns setGlobalSettings(const bool& globalSettings);

    /**
     * Gets global profile type
     * @param role Data role
     * @return Global profile type in variant form
     */
    QVariant getGlobalProfileType(const std::int32_t& role) const;

    /**
     * Sets the global profile type
     * @param profileType Global profile type
     * @return Columns that are affected by the operation
     */
    Configuration::AffectedColumns setGlobalProfileType(const Channel::ProfileType& globalProfileType);

    /**
     * Gets global range type
     * @param role Data role
     * @return Global range type in variant form
     */
    QVariant getGlobalRangeType(const std::int32_t& role) const;

    /**
     * Sets global range type
     * @param rangeType Global range type
     * @return Columns that are affected by the operation
     */
    Configuration::AffectedColumns setGlobalRangeType(const Channel::RangeType& globalRangeType);

    /**
     * Gets show dimensions names
     * @param role Data role
     * @return Show dimensions names in variant form
     */
    QVariant getShowDimensionNames(const std::int32_t& role) const;

    /**
     * Sets show dimensions names
     * @param showDimensionNames Show dimensions names (on/off)
     * @return Columns that are affected by the operation
     */
    Configuration::AffectedColumns setShowDimensionNames(const bool& showDimensionNames);

    /** 
     * Gets show differential profile
     * @param role Data role
     * @return Show differential profile in variant form
     */
    QVariant getShowDifferentialProfile(const std::int32_t& role) const;

    /**
     * Sets show differential profile
     * @param showDifferentialProfile Show differential profile in the viewer (on/off)
     * @return Columns that are affected by the operation
     */
    Configuration::AffectedColumns setShowDifferentialProfile(const bool& showDifferentialProfile);

    /**
     * Gets profile dataset names (selectable)
     * @param profileIndex Index of the profile
     * @param role Data role
     * @return Profile dataset names in variant form
     */
    QVariant getProfileDatasetNames(const std::int32_t& profileIndex, const std::int32_t& role) const;

    /**
     * Sets selectable dataset names for profile with \p profileIndex
     * @param profileIndex Index of the profile
     * @param datasetNames Selectable dataset names
     * @return Columns that are affected by the operation
     */
    Configuration::AffectedColumns setProfileDatasetNames(const std::int32_t& profileIndex, const QStringList& datasetNames);

    /**
     * Gets profile dataset name
     * @param profileIndex Index of the profile
     * @param role Data role
     * @return Profile dataset name in variant form
     */
    QVariant getProfileDatasetName(const std::int32_t& profileIndex, const std::int32_t& role) const;

    /**
     * Sets profile dataset names
     * @param profileIndex Index of the profile
     * @param datasetName Profile dataset names
     * @return Columns that are affected by the operation
     */
    Configuration::AffectedColumns setProfileDatasetName(const std::int32_t& profileIndex, const QString& datasetName);

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

    /** Returns the index(es) of the enabled channels */
    QVector<std::uint32_t> getChannelsEnabled() const;

    /** Returns the number of enabled channels */
    std::int32_t getNoChannelsEnabled() const;

    /** Returns the number of displayable channels */
    std::int32_t getNoDisplayChannels() const;

    /** Returns whether a differential profile can be shown */
    bool canShowDifferentialProfile() const;

    /**
     * Get tooltip for \p column
     * @param column Column
     * @param description Description
     */
    QString getTooltip(const std::int32_t& column, const QString& description) const;

    /**
     * Updates the differential profile settings
     * @return Columns that are affected by the operation
     */
    Configuration::AffectedColumns updateDifferentialProfile();

private:
    Channels		        _channels;                      /** Channels */
    QStringList		        _subsets;                       /** Subsets of the primary dataset (selected in the first channel) */
    bool		            _showDifferentialProfile;       /** Whether to show the differential profile in the viewer */
    QStringList             _profileDatasetNames[2];        /** Profile 1-2 dataset names (for differential profile) */
    QString                 _profileDatasetName[2];         /** Profile 1-2 selected dataset name (for differential profile) */
    bool                    _showDimensionNames;            /** Whether to show dimension names in the viewer */
    bool                    _globalSettings;                /** Whether whether global settings are on or off */
    Channel::ProfileType	_globalProfileType;             /** Global profile type */
    Channel::RangeType		_globalRangeType;			    /** Global range type */
    QVariantMap             _spec;                          /** Specification for use in JS visualization client (Vega) */

protected:
    static DimensionsViewerPlugin* dimensionsViewerPlugin;
    static std::int32_t maxNoDimensions;

    friend class DimensionsViewerPlugin;
    friend class ConfigurationsModel;
};