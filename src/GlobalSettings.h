#pragma once

#include "ModelItem.h"
#include "Profile.h"

#include <QObject>
#include <QModelIndexList>

/**
 * Global settings class
 *
 * @author T. Kroes
 */
class GlobalSettings : public ModelItem {

public: // Columns

    /** Data columns */
    enum class Column {
        Enabled,
        ProfileTypes,
        ProfileType,
        RangeType,
        RangeTypes,

        Start = Enabled,
        End = RangeTypes
    };

    /** Denotes which columns are affected by a change in model data */
    using AffectedColumns = QVector<Column>;

protected: // Construction

	GlobalSettings(ModelItem* parent);

public: // ModelIndex: MVC

    /**
     * Returns the number of rows in the model
     * @param parentIndex Parent index
     */
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

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
     * Sets the data value for the given model index and data role
     * @param index Model index
     * @param value Data value in variant form
     * @param role Data role
     * @return Model indices that are affected by the operation
     */
    QModelIndexList setData(const QModelIndex& index, const QVariant& value, const int& role) override;

    /**
     * Returns the model index belonging to the given model row and column
     * @param row Model row
     * @param column Model column
     * @param parent Parent model index
     * @return Model index for the given model row and column
     */
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;

    /**
     * Returns the parent model index
     * @param index Model index
     * @return Parent model index for the given model index
     */
    QModelIndex parent(const QModelIndex& index) const override;

public: // ModelIndex: Hierarchy

    /**
     * Returns a model item node by index
     * @param index Index of the child model item
     * @return Model item at index
     */
    ModelItem* getChild(const int& index) const override;

    /** Returns the number of children */
    int getChildCount() const override;

public: // Getters/setters

    /**
     * Gets enabled
     * @param role Data role
     * @return Global settings (on/off)
     */
     QVariant getEnabled(const std::int32_t& role) const;

    /**
     * Sets enabled
     * @param enabled Global settings (on/off)
     * @return Columns that are affected by the operation
     */
     AffectedColumns setEnabled(const bool& enabled);

     /**
     * Gets global profile types
     * @param role Data role
     * @return Global profile types in variant form
     */
     QVariant getProfileTypes(const std::int32_t& role) const;

    /**
     * Gets global profile type
     * @param role Data role
     * @return Global profile type in variant form
     */
     QVariant getProfileType(const std::int32_t& role) const;

    /**
     * Sets the global profile type
     * @param profileType Global profile type
     * @return Columns that are affected by the operation
     */
     AffectedColumns setProfileType(const Profile::ProfileType& globalProfileType);

    /**
     * Gets global range types
     * @param role Data role
     * @return Global range types in variant form
     */
     QVariant getRangeTypes(const std::int32_t& role) const;

    /**
     * Gets global range type
     * @param role Data role
     * @return Global range type in variant form
     */
     QVariant getRangeType(const std::int32_t& role) const;

    /**
     * Sets global range type
     * @param rangeType Global range type
     * @return Columns that are affected by the operation
     */
     AffectedColumns setRangeType(const Profile::RangeType& globalRangeType);

private:
    bool        _enabled;       /** Whether whether global settings are on or off */
    Profile     _profile;       /** Global profile settings */

    friend class Configuration;
};


//GlobalSettings,                 /** Whether whether global settings are on or off */
//GlobalProfileTypes,                                                 /** Global profile types */
//GlobalProfileType,                                                  /** Global profile type */
//GlobalRangeTypes,                                                   /** Global range types */
//GlobalRangeType,                                                    /** Global range type */