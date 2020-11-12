#pragma once

#include <QObject>
#include <QMap>

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

    QStringList getProfileTypeNames() const;

    ProfileType getProfileType() const;

    void setProfileType(const ProfileType& profileType);

    RangeType getRangeType() const;

    void setRangeType(const RangeType& rangeType);

    

    QStringList getRangeTypeNames() const;

    QVector<RangeType> getRangeTypes() const;

private:
    bool                    _enabled;           /** Whether the profile is enabled or not */
    ProfileType             _profileType;       /** Current profile type */
    RangeType               _rangeType;         /** Current range type */
    QVector<RangeType>      _rangeTypes;        /** Range types (depends on the current profile type) */

    friend class Channel;
};