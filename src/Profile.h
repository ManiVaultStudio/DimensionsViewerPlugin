#pragma once

#include "TreeItem.h"

#include <QObject>
#include <QMap>
#include <QSet>

class Channel;

/**
 * Profile tree item class
 *
 * @author T. Kroes
 */
class Profile : public TreeItem {

public: // Columns and rows

    /** Tree item columns */
    enum class Column {

        /** Derived tree item columns */
        Type,                           /** Type of tree item */
        Name,                           /** Name of tree item */
        Enabled,                        /** Whether the tree item is enabled or not */
        Modified,                       /** Last modified integer stamp */
        UUID,                           /** Universal unique identifier */

        /** Styling tree item columns */
        ProfileTypes,                   /** Available profile types */
        ProfileType,                    /** Current profile type */
        RangeTypes,                     /** Available range types */
        RangeType,                      /** Current range type */

        _Start  = ProfileTypes,
        _End    = RangeType,
        _Count  = _End + 1
    };

    /** Columns set alias */
    using Columns = QSet<Column>;

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

public: // Enumerations

	/** Dimension values profile type (e.g. mean and median) */
	enum class ProfileType {
		None,               /** No profile is displayed */
		Mean,               /** Display statistical mean */
		Median,             /** Display statistical median */
		Differential,       /** Display differential profile (difference between two profiles) */

        _Start  = None,
		_End    = Differential,
		_Count  = _End + 1
	};
    
    /** Profile types set alias */
    using ProfileTypes = QSet<ProfileType>;

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

        _Start  = None,
		_End    = Percentile10,
		_Count  = _End + 1
	};

    /** Range types set alias */
    using RangeTypes = QSet<RangeType>;

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
     * @param parent Parent tree item
     * @param profileType Profile type
     */
    Profile(TreeItem* parent = nullptr, const ProfileType& profileType = ProfileType::Mean);

public: // TreeItem: model API

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
     * Get data role
     * @param column Column to fetch data from
     * @param role Data role
     * @return Data in variant form
     */
    QVariant getData(const Column& column, const std::int32_t& role) const;

    /**
     * Sets data
     * @param index Model index
     * @param value Data value in variant form
     * @param role Data role
     * @return Model indices that are affected by the operation
     */
    QModelIndexList setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role = Qt::EditRole) override;

    /**
     * Get indices that are affected by a data change of \p index
     * @param index Model index which changed
     * @return Affected model indices
     */
    QModelIndexList getAffectedIndices(const QModelIndex& index) const override;

    /** Get number of columns */
    std::uint32_t getColumnCount() const override {
        return to_ul(Column::_Count);
    };

public: // TreeItem: visitor API

    /** Accept visitor */
    void accept(Visitor* visitor) const override;

public: // Getters/setters

    /** Gets the profile types */
    ProfileTypes getProfileTypes() const;

    /** Gets profile type names */
    QStringList getProfileTypeNames() const;

    /** Gets current profile type */
    ProfileType getProfileType() const;

    /**
     * Sets current profile type (if not locked)
     * @param profileType Profile type
     */
    void setProfileType(const ProfileType& profileType);

    /** Gets range types */
    RangeTypes getRangeTypes() const;

    /** Gets range type names */
    QStringList getRangeTypeNames() const;

    /** Gets current range type */
    RangeType getRangeType() const;

    /**
     * Sets current range type (if not locked)
     * @param rangeType Range type
     */
    void setRangeType(const RangeType& rangeType);

private: // Miscellaneous

    /** Updates the internals */
    void update();
    
    /** get channel */
    const Channel* getChannel() const;

private:
    bool            _locked;            /** Whether settings are locked */
    ProfileTypes    _profileTypes;      /** Current profile type */
    ProfileType     _profileType;       /** Available profile type(s) */
    RangeTypes      _rangeTypes;        /** Available range types (depends on the current profile type) */
    RangeType       _rangeType;         /** Current range type */

    friend class Channel;
};

/** Get scoped enum in columns set to work */
inline uint qHash(Profile::Column key, uint seed) {
    return ::qHash(static_cast<uint>(key), seed);
}

/** Get scoped enum in profile type set to work */
inline uint qHash(Profile::ProfileType key, uint seed) {
    return ::qHash(static_cast<uint>(key), seed);
}

/** Get scoped enum in range type set to work */
inline uint qHash(Profile::RangeType key, uint seed) {
    return ::qHash(static_cast<uint>(key), seed);
}