#pragma once

#include "Item.h"

#include <QObject>
#include <QVector>
#include <QColor>
#include <QVariantMap>

class Points;
class DimensionsViewerPlugin;
class Configuration;
class Channels;

/**
 * Channel tree item class
 *
 * @author T. Kroes
 */
class Channel : public tree::Item
{

public: // Enumerations

    /** Child enumeration (each child enum item maps to a child tree item) */
    enum class Child {
        Enabled,
        DatasetNames,
        DatasetName,
        Linked,
        NoPoints,
        NoDimensions,
        Profile,
        Differential,
        Styling,

        _Start  = Enabled,
        _End    = Styling,
        _Count  = _End + 1
    };

    /** Children set alias */
    using Items = QSet<Child>;

    /** Maps child name to child enum */
    static QMap<QString, Child> const children;

protected: // Construction

	/**
	 * Constructor
	 * @param parent Parent tree item
	 * @param index Channel index
	 * @param displayName Channel name in the user interface
	 * @param enabled Whether the channel is enabled
	 * @param datasetName The name of the channel dataset
	 * @param dataName The data name of the channel
	 * @param color The color of the channel
	 * @param opacity Render opacity
	 */
	Channel(tree::Item* parent, const std::uint32_t& index, const QString& name, const bool& enabled, const bool& linked, const QString& datasetName);

public: // TreeItem: model API

    /** Initialization after the model index has been set */
    void initialize() override;

public: // TreeItem: visitor API

    /** Accept visitor */
    void accept(tree::Visitor* visitor) const override;

public: // Plugin

    /**
     * Set dimensions viewer plugin
     * @param dimensionsViewerPlugin Dimensions viewer plugin
     */
    static void setDimensionsViewerPlugin(DimensionsViewerPlugin* dimensionsViewerPlugin);

    /** Returns the number of dimensions */
    std::int32_t getNoDimensions() const;

    /** Returns the number of points */
    std::int32_t getNoPoints() const;

    /** Resolved points data based on points dataset name */
    void resolvePoints();

protected: // Miscellaneous
	
    /** Get parent channels tree item */
    const Channels* getChannels() const;

protected:
    Points*     _points;        /** Pointer to points dataset */

    /** Pointer to dimensions viewer plugin (used to retrieve point data) */
    static DimensionsViewerPlugin*  dimensionsViewerPlugin;

protected:
	friend class ConfigurationsModel;
	friend class Channels;
	friend class Differential;
	friend class Profile;
};

/** Get scoped enum in rows set to work */
inline uint qHash(Channel::Child key, uint seed) {
    return ::qHash(static_cast<uint>(key), seed);
}