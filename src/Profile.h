#pragma once

#include "Item.h"

#include <QObject>
#include <QMap>
#include <QSet>

class Channel;

/**
 * Profile tree item class
 *
 * @author T. Kroes
 */
class Profile : public tree::Item {

public: // Enumerations

    /** Child enumeration (each child enum item maps to a child tree item) */
    enum class Child {
        ProfileTypes,           /** Available profile types */
        ProfileType,            /** Current profile type */
        RangeTypes,             /** Available range types */
        RangeType,              /** Current range type */

        _Start  = ProfileType,
        _End    = RangeType,
        _Count  = _End + 1
    };

    /** Children set alias */
    using Children = QSet<Child>;

    /** Maps child name to child enum */
    static QMap<QString, Child> const children;

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
    Profile(Item* parent = nullptr, const ProfileType& profileType = ProfileType::Mean);

public: // TreeItem: visitor API

    /** Accept visitor */
    void accept(tree::Visitor* visitor) const override;

private: // Miscellaneous

    /** Updates the internals */
    QStringList getRangeTypes(const ProfileType& profileType);
    
    friend class Channels;
    friend class Channel;
};

/** Get scoped enum in columns set to work */
inline uint qHash(Profile::Child key, uint seed) {
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