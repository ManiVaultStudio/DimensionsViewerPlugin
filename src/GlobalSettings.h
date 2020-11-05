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
    enum Column {
        Enabled,
        ProfileTypes,
        ProfileType,
        RangeTypes,
        RangeType,

        Start = Enabled,
        End = RangeType
    };

public: // Get/set data roles

    static QMap<Column, std::function<QVariant(GlobalSettings* channel)>> const getEditRoles;
    static QMap<Column, std::function<QVariant(GlobalSettings* channel)>> const getDisplayRoles;
    static QMap<Column, std::function<QModelIndexList(GlobalSettings* channel, const QVariant& value, const QModelIndex& index)>> const setEditRoles;

protected: // Construction

	GlobalSettings(ModelItem* parent);

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
     QModelIndexList setEnabled(const bool& enabled);

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
     QModelIndexList setProfileType(const Profile::ProfileType& globalProfileType);

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
     QModelIndexList setRangeType(const Profile::RangeType& globalRangeType);

private:
    bool        _enabled;       /** Whether whether global settings are on or off */
    Profile     _profile;       /** Global profile settings */

    friend class Configuration;
};