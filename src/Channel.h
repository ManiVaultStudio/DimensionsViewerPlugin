#pragma once

#include <QObject>
#include <QVector>
#include <QColor>
#include <QVariantMap>

class Points;

class DimensionsViewerPlugin;

/** Channel settings */
class Channel : public QObject {

	Q_OBJECT

public: // Enumerations

	/** Profile type (e.g. average and mean) */
	enum class ProfileType {
		None,
		Average,
		Mean,

		End = Mean
	};

	/** Get string representation of profile type enumeration */
	static QString getProfileTypeName(const ProfileType& profileType) {
		switch (profileType) {
			case ProfileType::None:
				return "Profile: None";

			case ProfileType::Average:
				return "Profile: Average";

			case ProfileType::Mean:
				return "Profile: Mean";
		}

		return QString();
	}

	/** Get profile type names in a string list */
	static QStringList getProfileTypeNames() {
		QStringList profileTypeNames;

		for (int i = 0; i <= static_cast<int>(ProfileType::End); ++i)
			profileTypeNames << getProfileTypeName(static_cast<ProfileType>(i));

		return profileTypeNames;
	}

	/** Band type (e.g. minimum/maximum and standard deviation) */
	enum class BandType {
		None,
		MinMax,
		StandardDeviation1,
		StandardDeviation2,

		End = StandardDeviation2
	};

	/** Get string representation of band type enumeration */
	static QString getBandTypeName(const BandType& bandType) {
		switch (bandType) {
			case BandType::None:
				return "Band: None";

			case BandType::MinMax:
				return "Band: Min/Max";

			case BandType::StandardDeviation1:
				return "Band: 1 SD";

			case BandType::StandardDeviation2:
				return "Band: 2 SD";
		}

		return QString();
	}

	/** Get band type names in a string list */
	static QStringList getBandTypeNames() {
		QStringList bandTypeNames;

		for (int i = 0; i <= static_cast<int>(BandType::End); ++i)
			bandTypeNames << getBandTypeName(static_cast<BandType>(i));

		return bandTypeNames;
	}

protected: // Construction

	/**
	 * Constructor
	 * @param name Name of the channel
	 * @param enabled Whether the channel is enabled
	 * @param datasetName The name of the channel dataset
	 * @param dataName The data name of the channel
	 * @param color The color of the channel
	 */
	Channel(QObject* parent, const QString& name, const bool& enabled, const QString& datasetName, const QString& dataName, const QColor& color);

public: // Getters/setters

	/** Returns the channel name */
	QString getName() const {
		return _name;
	};

	/** Returns whether the channel is enabled */
	bool isEnabled() const {
		return _enabled;
	};

	/**
	 * Sets whether the channel is enabled
	 * @param enabled Whether the channel is enabled
	 */
	void setEnabled(const bool& enabled);

	/** Returns the dataset name */
	QString getDatasetName() const {
		return _datasetName;
	};

	/**
	 * Sets the dataset name
	 * @param datasetName Name of the dataset
	 */
	void setDatasetName(const QString& datasetName);

	/** Returns the data name */
	QString getDataName() const {
		return _dataName;
	};

	/** Returns the color */
	QColor getColor() const {
		return _color;
	};

	/**
	 * Sets the color
	 * @param color Color
	 */
	void setColor(const QColor& color);

	/** Returns the profile type */
	ProfileType getProfileType() const {
		return _profileType;
	};

	/**
	 * Sets the profile type
	 * @param profileType Profile type
	 */
	void setProfileType(const ProfileType& profileType);

	/** Returns the band type */
	BandType getBandType() const {
		return _bandType;
	};

	/**
	 * Sets the band type
	 * @param bandType Band type
	 */
	void setBandType(const BandType& bandType);

signals:

	/**
	 * Signals that the visibility changed
	 * @param enabled Whether the channel is enabled
	 */
	void enabledChanged(const bool& enabled);

	/**
	 * Signals that the dataset name changed
	 * @param datasetName Name of the dataset
	 */
	void datasetNameChanged(const QString& datasetName);

	/**
	 * Signals that the dataset name changed
	 * @param datasetName Name of the dataset
	 */
	void colorChanged(const QColor& color);

	/**
	 * Signals that the profile type changed
	 * @param profileType Profile type
	 */
	void profileTypeChanged(const ProfileType& profileType);

	/**
	 * Signals that the band type changed
	 * @param bandType Band type
	 */
	void bandTypeChanged(const BandType& bandType);

private:
	QString			_name;				/** Channel name (e.g. dataset, Subset1 and Subset 2) */
	bool			_enabled;			/** Whether the channel is enabled or not */
	QString			_datasetName;		/** Channel dataset name */
	QString			_dataName;			/** Channel data name */
	QColor			_color;				/** Channel color */
	ProfileType		_profileType;		/** The type of profile to visualize */
	BandType		_bandType;			/** The type of band to visualize */

protected:
	static DimensionsViewerPlugin* dimensionsViewerPlugin;

	friend class DimensionsViewerPlugin;
	friend class Configuration;
};

/**
 */
//class Channel : public QObject
//{
//	Q_OBJECT
//
//public: // Construction/destruction
//
//	/**
//	 * Constructor
//	 * @param dimensionsViewerPlugin Pointer to the parent dimensions viewer plugin
//	 */
//	Channel(const QString& name, DimensionsViewerPlugin* dimensionsViewerPlugin);
//
//public:
//
//	/**
//	 * Computes the dimension statistics on \p points
//	 * @param points Pointer to points dataset
//	 */
//	void update(Points* points, const std::vector<std::uint32_t>& selectedIndices = std::vector<std::uint32_t>());
//
//	void setDataSetName(const QString& name);
//	QString getDataSetName()const;
//
//	QString getName() { return _name; }
//
//private:
//
//	/** Returns the indices of the selected points */
//	std::vector<std::uint32_t> selectedIndices() const;
//
//signals:
//
//	/**
//	 * 
//	 */
//	void changed(const QVariantList& dimensions);
//
//private:
//	QString						_name;
//	DimensionsViewerPlugin*		_dimensionsViewerPlugin;
//	QString						_dataSetName;
//	Points*						_points;
//	
//};