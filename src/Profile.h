#pragma once

#include <QObject>
#include <QMap>
#include <QSet>

/**
 * Profile utility class
 *
 * @author T. Kroes
 */
class Profile {

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
     * @param profileType Profile type
     */
    Profile(const ProfileType& profileType);

public: // Operators

    Profile& operator=(const Profile& other)
    {
        _profileType    = other._profileType;
        _rangeTypes     = other._rangeTypes;
        _rangeType      = other._rangeType;

        return *this;
    }

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

private:

    /** Updates the internals */
    void update();
    
private:
    bool            _locked;            /** Whether settings are locked */
    ProfileTypes    _profileTypes;      /** Current profile type */
    ProfileType     _profileType;       /** Current profile type */
    RangeTypes      _rangeTypes;        /** Range types (depends on the current profile type) */
    RangeType       _rangeType;         /** Current range type */

    friend class Channel;
};

/** Get scoped enum in profile type set to work */
inline uint qHash(Profile::ProfileType key, uint seed) {
    return ::qHash(static_cast<uint>(key), seed);
}

/** Get scoped enum in range type set to work */
inline uint qHash(Profile::RangeType key, uint seed) {
    return ::qHash(static_cast<uint>(key), seed);
}