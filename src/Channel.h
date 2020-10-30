#pragma once

#include <QObject>
#include <QVector>
#include <QColor>
#include <QVariantMap>

class Points;
class DimensionsViewerPlugin;

/**
 * Channel class
 *
 * Class for reading/writing channel visualization properties and for synchronization with the JS Vega web view
 * 
 * @author T. Kroes
 */
class Channel : public QObject {

	Q_OBJECT

public: // Enumerations

	/** Profile type (e.g. mean and median) */
	enum class ProfileType {
		None,               /** No profile is displayed */
		Mean,               /** Display statistical mean */
		Median,             /** Display statistical median */

		End = Median
	};

	/** Get string representation of profile type enumeration */
	static QString getProfileTypeName(const ProfileType& profileType) {
		switch (profileType) {
			case ProfileType::None:
				return "None";

			case ProfileType::Mean:
				return "Mean";

			case ProfileType::Median:
				return "Median";
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

	/** Band type (e.g. standard deviation) */
	enum class BandType {
		None,                   /** Do not display standard deviation */
		StandardDeviation1,     /** Display one standard deviation */
		StandardDeviation2,     /** Display two standard deviations */

		End = StandardDeviation2
	};

	/** Get string representation of band type enumeration */
	static QString getBandTypeName(const BandType& bandType) {
		switch (bandType) {
			case BandType::None:
				return "None";

			case BandType::StandardDeviation1:
				return "1 SD";

			case BandType::StandardDeviation2:
				return "2 SD";
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
	 * @param parent Parent object
	 * @param index Channel index
	 * @param displayName Channel name in the user interface
	 * @param enabled Whether the channel is enabled
	 * @param datasetName The name of the channel dataset
	 * @param dataName The data name of the channel
	 * @param color The color of the channel
	 * @param opacity Render opacity
	 * @param lock Whether settings are locked
	 */
	Channel(QObject* parent, const std::uint32_t& index, const QString& displayName, const bool& enabled, const QString& datasetName, const QString& dataName, const QColor& color, const float& opacity = 1.0f, const bool& lock = false);

public: // Getters/setters

	/** Returns the channel index */
	std::uint32_t getIndex() const {
		return _index;
	};

	/** Returns the channel internal name */
	QString getInternalName() const {
		return _internalName;
	};

	/** Returns the channel display name */
	QString getDisplayName() const {
		return _displayName;
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

	/** Returns the render opacity */
	float getOpacity() const {
		return _opacity;
	};

	/**
	 * Sets the render opacity
	 * @param opacity Render opacity
	 */
	void setOpacity(const float& opacity);

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

	/** Returns whether to show the dimension range */
	bool getShowRange() const {
		return _showRange;
	};

	/**
	 * Sets whether to show the dimension range
	 * @param showRange Whether to show the dimension range
	 */
	void setShowRange(const bool& showRange);

	/** Returns whether settings are locked */
	bool isLocked() const {
		return _locked;
	};

	/**
	 * Sets whether settings are locked
	 * @param locked Whether settings are locked
	 */
	void setLocked(const bool& locked);

    /** Returns whether the channel can be displayed in the viewer */
    bool canDisplay() const;

	/** Returns the visualization specification */
	QVariantMap getSpec() {
		return _spec;
	};

public: // Points wrapper functions

    /** Returns the number of dimensions */
    std::int32_t getNoDimensions() const;

    /** Returns the number of points */
    std::int32_t getNoPoints() const;

private: // Miscellaneous
	
	/** Returns if the referenced dataset is a subset */
	bool isSubset() const;

	/** Updates the visualization specification */
	void updateSpec();

signals:

    /** Signals that the channel spec has changed */
    void specChanged(Channel* channel);

private:
	const std::uint32_t		_index;				/** Channel index */
	const QString			_internalName;		/** Channel internal name (e.g. channel1, channel2) */
	const QString			_displayName;		/** Channel display name (e.g. dataset, Subset1 and Subset 2) */
	bool					_enabled;			/** Whether the channel is enabled or not */
	QString					_datasetName;		/** Channel dataset name */
	QString					_dataName;			/** Channel data name */
	QColor					_color;				/** Channel color */
	float					_opacity;			/** Channel opacity */
	ProfileType				_profileType;		/** The type of profile to visualize */
	BandType				_bandType;			/** The type of band to visualize */
	bool					_showRange;			/** Show the dimensions ranges */
	bool					_locked;			/** Whether settings are locked (settings are linked to another channel) */
	QVariantMap				_spec;				/** Specification for use in JS visualization client (Vega) */
	Points*					_points;			/** Pointer to points dataset */

protected:
	static DimensionsViewerPlugin* dimensionsViewerPlugin;

	friend class DimensionsViewerPlugin;
	friend class Configuration;
};