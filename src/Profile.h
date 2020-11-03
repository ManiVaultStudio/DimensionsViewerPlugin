#pragma once

#include <QObject>
#include <QMap>

/**
 * Profile class
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

		End = Differential
	};
    
    /** Maps profile type name to profile type enum and */
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

    Profile(const ProfileType& profileType);

public: // Getters/setters

    ProfileType getProfileType() const;

    void setProfileType(const ProfileType& profileType);

    RangeType getRangeType() const;

    void setRangeType(const RangeType& rangeType);

    QStringList getProfileTypeNames() const;

    QStringList getRangeTypeNames() const;

    bool canDisplay() const;

private:
    ProfileType             _profileType;
    RangeType               _rangeType;
    QVector<RangeType>      _rangeTypes;
};