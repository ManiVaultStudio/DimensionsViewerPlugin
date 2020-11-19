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
class ChannelItem : public tree::Item {

    Q_OBJECT

public:

    /** Tree item rows */
    enum class Row {
        Profile,
        Differential,
        Styling,
        NoPoints,
        NoDimensions,

        _Start  = Profile,
        _End    = Styling,
        _Count  = _End + 1
    };

    /** Rows set alias */
    using Rows = QSet<Row>;

    /** Maps row name to row enum */
    static QMap<QString, Row> const rows;

    /** Get row name from row enum */
    static QString getRowTypeName(const Row& row) {
        return rows.key(row);
    }

    /** Get row enum from row name */
    static Row getRowTypeEnum(const QString& rowName) {
        return rows[rowName];
    }

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
	ChannelItem(tree::Item* parent, const std::uint32_t& index, const QString& name, const bool& enabled, const bool& linked, const QString& datasetName);

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
    const std::uint32_t     _index;             /** Index */
    bool                    _linked;            /** Whether settings are linked to the settings of the first channel */
    QStringList             _datasetNames;      /** Dataset names */
    QString                 _datasetName;       /** Dataset name */
    Points*                 _points;            /** Pointer to points dataset */

    /** Pointer to dimensions viewer plugin (used to retrieve point data) */
    static DimensionsViewerPlugin*  dimensionsViewerPlugin;

protected:
	friend class ConfigurationsModel;
	friend class Channels;
	friend class Differential;
	friend class Profile;
};

/** Get scoped enum in columns set to work */
inline uint qHash(ChannelItem::Column key, uint seed) {
    return ::qHash(static_cast<uint>(key), seed);
}

/** Get scoped enum in rows set to work */
inline uint qHash(ChannelItem::Row key, uint seed) {
    return ::qHash(static_cast<uint>(key), seed);
}