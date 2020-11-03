#pragma once

#include "Profile.h"

#include <QObject>
#include <QModelIndexList>

/**
 * Global settings class
 *
 * @author T. Kroes
 */
class GlobalSettings : public QObject {

public: // Columns

    /** Data columns */
    enum Column {
        Enabled,
        ProfileTypes,
        ProfileType,
        RangeType,
        RangeTypes,

        Start = Enabled,
        End = RangeTypes
    };

protected: // Construction

	GlobalSettings(QObject* parent);

public: // MVC

    /** Returns the number of columns */
    int getColumnCount() const { return static_cast<int>(Column::End) + 1; }

    /**
     * Returns the item flags for the given model index
     * @param index Model index
     * @return Item flags for the index
     */
    Qt::ItemFlags getFlags(const QModelIndex& index) const;

    /**
     * Returns the data for the given model index and data role
     * @param index Model index
     * @param role Data role
     * @return Data in variant form
     */
    QVariant getData(const QModelIndex& index, const int& role) const;

    /**
     * Sets the data value for the given model index and data role
     * @param index Model index
     * @param value Data value in variant form
     * @param role Data role
     * @return Columns that are affected by the operation
     */
    AffectedColumns setData(const QModelIndex& index, const QVariant& value, const int& role);

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