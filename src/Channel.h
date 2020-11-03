#pragma once

#include "Profile.h"

#include <QObject>
#include <QVector>
#include <QColor>
#include <QVariantMap>

class Points;
class DimensionsViewerPlugin;
class Configuration;

/**
 * Channel class
 *
 * Class for reading/writing channel visualization properties and for synchronization with the JS Vega web view
 * 
 * @author T. Kroes
 */
class Channel : public QObject {

	Q_OBJECT

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
	 */
	Channel(QObject* parent, const std::uint32_t& index, const QString& displayName, const bool& enabled, const QString& datasetName, const QString& dataName, const QColor& color, const float& opacity = 1.0f);

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

    /**
     * Sets name
     * @param displayName Display name
     */
    void setDisplayName(const QString& displayName);

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
    Profile::ProfileType getProfileType() const;

	/**
	 * Sets the profile type
	 * @param profileType Profile type
	 */
	void setProfileType(const Profile::ProfileType& profileType);

	/** Returns the range type */
    Profile::RangeType getRangeType() const;

	/**
	 * Sets the range type
	 * @param rangeType Range type
	 */
	void setRangeType(const Profile::RangeType& rangeType);

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
    Configuration*              _configuration;                 /** Parent configuration */
	const std::uint32_t		    _index;				            /** Channel index */
	const QString			    _internalName;		            /** Channel internal name (e.g. channel1, channel2) */
	QString			            _displayName;		            /** Channel display name (e.g. dataset, Subset1 and Subset 2) */
	bool					    _enabled;			            /** Whether the channel is enabled or not */
	QString					    _datasetName;		            /** Channel dataset name */
	QString					    _dataName;			            /** Channel data name */
	QColor					    _color;				            /** Channel color */
	float					    _opacity;			            /** Channel opacity */
    Profile				        _profile;		                /** TODO */
	QVariantMap				    _spec;				            /** Specification for use in JS visualization client (Vega) */
	Points*					    _points;			            /** Pointer to points dataset */

protected:
	static DimensionsViewerPlugin* dimensionsViewerPlugin;

	friend class DimensionsViewerPlugin;
	friend class Channels;
};