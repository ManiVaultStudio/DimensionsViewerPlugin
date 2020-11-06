#pragma once

#include "ModelItem.h"
#include "Channels.h"
#include "GlobalSettings.h"

#include <QStringList>
#include <QColor>
#include <QModelIndex>

class DimensionsViewerPlugin;

/**
 * Configuration class
 *
 * @author T. Kroes
 */
class Configuration : public ModelItem
{
public: // Constants

    /** The number of channels in the configuration */
    static const std::int32_t noChannels = 3;

public: // Enumerations

    /** Data columns */
    enum class Column {
        Index,                      /** Index */
        Name,                       /** Name */
        DatasetName,                /** TODO */
        Subsets,                    /** The subset(s) parameter of the first dataset */
        SelectionStamp,             /** Auxiliary column for triggering synchronization */

        Start = Index,               /** Column start */
        End = SelectionStamp        /** Column end */
    };

    /** Maps column name to column enum and vice versa */
    static QMap<QString, Column> const columns;

	/** Get column name from column enum */
	static QString getColumnName(const Column& column) {
        return columns.key(column);
	}

    /** Get column enum from column name */
    static Column getColumnEnum(const QString& columnName) {
        return columns[columnName];
    }

    /** TODO */
    enum class Child {
        Channels,                       /** Channels model item */
        GlobalSettings,                 /** Global settings model item */
        DifferentialProfileSettings,    /** Differential profile settings model item */
        MiscellaneousSettings,          /** Miscellaneous settings model item */

        Start = Channels,               /** Column start */
        End = MiscellaneousSettings     /** Column end */
    };

public: // Construction

	/**
	 * Constructor
	 * @param parent Parent model item
	 * @param datasetName Name of the primary dataset
	 * @param dataName Name of the primary data
	 */
	Configuration(ModelItem* parent, const QString& datasetName, const QString& dataName);

public: // ModelIndex: MVC

    /**
     * Returns the item flags for the given model index
     * @param index Model index
     * @return Item flags for the index
     */
    Qt::ItemFlags getFlags(const QModelIndex& index) const override;

    /**
     * Returns the data for the given model index and data role
     * @param index Model index
     * @param role Data role
     * @return Data in variant form
     */
    QVariant getData(const QModelIndex& index, const int& role) const override;

    /**
     * Returns the data for the given column and data role
     * @param index Model index
     * @param role Data role
     * @return Data in variant form
     */
    QVariant getData(const int& column, const int& role) const override;

    /**
     * Sets the data value for the given model index and data role
     * @param index Model index
     * @param value Data value in variant form
     * @param role Data role
     * @return Model indices that are affected by the operation
     */
    QModelIndexList setData(const QModelIndex& index, const QVariant& value, const int& role) override;

public: // ModelIndex: Hierarchy

    /**
     * Returns a model item node by index
     * @param index Index of the child model item
     * @return Model item at index
     */
    ModelItem* getChild(const int& index) const override;

    /** Returns the number of children */
    int getChildCount() const override;

    /**
     * Returns the child index
     * @param child Pointer to child model item
     */
    int getChildIndex(ModelItem* child) const override;

public: //Getters/setters

    /**
     * Gets index
     * @param role Data role
     * @return Index in variant form
     */
    QVariant getIndex(const std::int32_t& role) const;

    /**
     * Returns name
     * @param role Data role
     * @return Name in variant form
     */
    QVariant getName(const std::int32_t& role) const;

    /**
     * Sets name
     * @param name Name
     * @return Columns that are affected by the operation
     */
    QModelIndexList setName(const QString& name);

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
    QModelIndexList setSubsets(const QStringList& subsets);

public: // Miscellaneous

	/** Get channels */
    Channels& getChannels() { return _channels; };

    /** Get global settings */
    GlobalSettings& getGlobalSettings() { return _globalSettings; };

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
     //AffectedColumns updateDifferentialProfile();

protected:
    std::int32_t            _index;                         /** TODO */
    QString                 _name;                          /** TODO */
    Channels		        _channels;                      /** TODO */
    GlobalSettings          _globalSettings;                /** TODO */
    QStringList		        _subsets;                       /** Subsets of the primary dataset (selected in the first channel) */
    bool		            _showDifferentialProfile;       /** Whether to show the differential profile in the viewer */
    QStringList             _profileDatasetNames[2];        /** Profile 1-2 dataset names (for differential profile) */
    QString                 _profileDatasetName[2];         /** Profile 1-2 selected dataset name (for differential profile) */
    bool                    _showDimensionNames;            /** Whether to show dimension names in the viewer */
    QVariantMap             _spec;                          /** Specification for use in JS visualization client (Vega) */

protected:
    static std::int32_t maxNoDimensions;
    static std::int32_t noConfigurations;

    friend class ConfigurationsModel;
    friend class Channels;
    friend class Channel;
};







//public: // Getters/setters

    

      /**
       * Gets show dimensions names
       * @param role Data role
       * @return Show dimensions names in variant form
       */
       //QVariant getShowDimensionNames(const std::int32_t& role) const;

      /**
       * Sets show dimensions names
       * @param showDimensionNames Show dimensions names (on/off)
       * @return Columns that are affected by the operation
       */
       //AffectedColumns setShowDimensionNames(const bool& showDimensionNames);

      /**
       * Gets show differential profile
       * @param role Data role
       * @return Show differential profile in variant form
       */
       //QVariant getShowDifferentialProfile(const std::int32_t& role) const;

      /**
       * Sets show differential profile
       * @param showDifferentialProfile Show differential profile in the viewer (on/off)
       * @return Columns that are affected by the operation
       */
       //AffectedColumns setShowDifferentialProfile(const bool& showDifferentialProfile);

      /**
       * Gets profile dataset names (selectable)
       * @param profileIndex Index of the profile
       * @param role Data role
       * @return Profile dataset names in variant form
       */
       //QVariant getProfileDatasetNames(const std::int32_t& profileIndex, const std::int32_t& role) const;

      /**
       * Sets selectable dataset names for profile with \p profileIndex
       * @param profileIndex Index of the profile
       * @param datasetNames Selectable dataset names
       * @return Columns that are affected by the operation
       */
       //AffectedColumns setProfileDatasetNames(const std::int32_t& profileIndex, const QStringList& datasetNames);

      /**
       * Gets profile dataset name
       * @param profileIndex Index of the profile
       * @param role Data role
       * @return Profile dataset name in variant form
       */
       //QVariant getProfileDatasetName(const std::int32_t& profileIndex, const std::int32_t& role) const;

      /**
       * Sets profile dataset names
       * @param profileIndex Index of the profile
       * @param datasetName Profile dataset names
       * @return Columns that are affected by the operation
       */
       //AffectedColumns setProfileDatasetName(const std::int32_t& profileIndex, const QString& datasetName);



//ShowDifferentialProfile,                                            /** Whether to show the differential profile in the viewer */
//Profile1DatasetNames,                                               /** First profile available dataset names (for differential profile) */
//Profile2DatasetNames,                                               /** Second profile available dataset names (for differential profile) */
//Profile1DatasetName,                                                /** First profile selected dataset name (for differential profile) */
//Profile2DatasetName,                                                /** Second profile selected dataset name (for differential profile) */