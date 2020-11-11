#pragma once

#include "ModelItem.h"

#include <QObject>
#include <QMap>

/**
 * Profile model item class
 *
 * @author T. Kroes
 */
class Profile : public ModelItem {

public: // Columns and rows

    /** Model item columns */
    enum class Column {
        Name,               /** Name of the model item */
        Enabled,            /** Enabled */
        ProfileTypes,       /** Available profile types */
        ProfileType,        /** Current profile type */
        RangeTypes,         /** Available range types */
        RangeType,          /** Current range type */

        Start   = Name,
        End     = RangeType,
        Count   = End + 1
    };

    /** Maps column name to column enum */
    static QMap<QString, Column> const columns;

    /** Get column name from column enum */
    static QString getColumnTypeName(const Column& column) {
        return columns.key(column);
    }

    /** Get column enum from column name */
    static Column getColumnTypeEnum(const QString& columnName) {
        return columns[columnName];
    }

    /** Model item rows */
    enum class Row {};

public: // Enumerations

	/** Dimension values profile type (e.g. mean and median) */
	enum class ProfileType {
		None,               /** No profile is displayed */
		Mean,               /** Display statistical mean */
		Median,             /** Display statistical median */
		Differential,       /** Display differential profile (difference between two profiles) */

        Start = None,
		End = Differential
	};
    
    /** Maps profile type name to profile type enum and vice versa */
    static QMap<QString, ProfileType> const profileTypes;

	/** Get string representation of profile type enumeration */
	static QString getProfileTypeName(const ProfileType& profileType) {
        return profileTypes.key(profileType);
	}

    /** Get enum representation from profile type name */
    static ProfileType getProfileTypeEnum(const QString& profileTypeName) {
        return profileTypes[profileTypeName];
    }

	/** Dimension values range type */
	enum class RangeType {
		None,                   /** Do not display value range */
		StandardDeviation1,     /** Plus and minus three standard deviations (mean profile type) */
		StandardDeviation2,     /** Plus and minus three standard deviations (mean profile type) */
		StandardDeviation3,     /** Plus and minus three standard deviations (mean profile type) */
		MinMax,                 /** Minimum and maximum range (mean profile type) */
		Percentile5,            /** 5% and 95% percentile (median profile type) */
		Percentile10,           /** 10% and 90% percentile (median profile type) */

        Start = None,
		End = Percentile10
	};

    /** Maps range type name to range type enum and */
    static QMap<QString, RangeType> const rangeTypes;

	/** Get string representation of range type enumeration */
	static QString getRangeTypeName(const RangeType& rangeType) {
        return rangeTypes.key(rangeType);
	}

    /** Get enum representation from range type name */
    static RangeType getRangeTypeEnum(const QString& rangeTypeName) {
        return rangeTypes[rangeTypeName];
    }

public: // Construction

    /**
     * Constructor
     * @param parent Parent model item
     * @param profileType Profile type
     */
    Profile(ModelItem* parent, const ProfileType& profileType);

public: // ModelIndex: Model

    /** Returns the number of columns in the item */
    int columnCount() const override;

    /**
     * Returns the item flags for the given model index
     * @param index Model index
     * @return Item flags for the index
     */
    Qt::ItemFlags getFlags(const QModelIndex& index) const override;

    /**
     * Get data role
     * @param column Column to fetch data from
     * @param role Data role
     * @return Data in variant form
     */
    QVariant getData(const std::int32_t& column, const std::int32_t& role) const override;

    /**
     * Sets data
     * @param index Model index
     * @param value Data value in variant form
     * @param role Data role
     * @return Model indices that are affected by the operation
     */
    QModelIndexList setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role = Qt::EditRole) override;

public: // Getters/setters

    ProfileType getProfileType() const;

    void setProfileType(const ProfileType& profileType);

    RangeType getRangeType() const;

    void setRangeType(const RangeType& rangeType);

    QStringList getProfileTypeNames() const;

    QStringList getRangeTypeNames() const;

    QVector<RangeType> getRangeTypes() const;

private:
    bool                    _enabled;           /** Whether the profile is enabled or not */
    ProfileType             _profileType;       /** Current profile type */
    RangeType               _rangeType;         /** Current range type */
    QVector<RangeType>      _rangeTypes;        /** Range types (depends on the current profile type) */

    friend class Channel;
};