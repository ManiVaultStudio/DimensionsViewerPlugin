#pragma once

#include "TreeItem.h"
#include "Profile.h"
#include "Differential.h"
#include "Styling.h"

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
class Channel : public TreeItem {

    Q_OBJECT

public: // Columns and rows

    /** Tree item columns */
    enum class Column {

        /** Derived tree item columns */
        Type,                           /** Type of tree item */
        Name,                           /** Name of tree item */
        Enabled,                        /** Whether the tree item is enabled or not */
        Modified,                       /** Last modified integer stamp */
        UUID,                           /** Universal unique identifier */

        /** Channel tree item columns */
        Index,                          /** Channel index */
        DatasetNames,                   /** Candidate dataset name(s) */
        DatasetName,                    /** Candidate dataset name */
        Differential,                   /** Differential */
        Profile,                        /** Profile */
        Styling,                        /** Styling properties */
        Linked,                         /** Whether the channel is linked to another channel */
        NoDimensions,                   /** Number of points in the referenced points dataset */
        NoPoints,                       /** Number of dimensions in the referenced points dataset */
        IsAggregate,                    /** Whether the channel is the combination of one or more other channels */

        _Start  = Index,
        _End    = NoPoints,
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

    /** Tree item rows */
    enum class Row {
        Profile,
        Differential,
        Styling,

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
	Channel(TreeItem* parent, const std::uint32_t& index, const QString& name, const bool& enabled, const bool& linked, const QString& datasetName, const Profile::ProfileType& profileType, const QColor& color, const float& opacity = 1.0f);

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

    /** Get number of columns */
    std::uint32_t getColumnCount() const override {
        return to_ul(Column::_Count);
    };

public: // TreeItem: hierarchy API

    /**
     * Returns a tree item node by index
     * @param index Index of the child tree item
     * @return Tree item at index
     */
    TreeItem* getChild(const int& index) const override;

    /** Returns the number of children */
    int getChildCount() const override;

    /**
        * Returns the child index
        * @param child Pointer to child tree item
        */
    int getChildIndex(TreeItem* child) const override;

public: // TreeItem: visitor API

    /** Accept visitor */
    void accept(Visitor* visitor) const override;

public: // Points data functions

    /** Returns the number of dimensions */
    std::int32_t getNoDimensions() const;

    /** Returns the number of points */
    std::int32_t getNoPoints() const;

    /** Resolved points data based on points dataset name */
    void resolvePoints();

protected: // Miscellaneous
	
    /** Get parent channels tree item */
    const Channels* getChannels() const;

public: // Getters

    /** Get profile settings */
    Profile* getProfile() {
        return _profile;
    }

    /** Get differential settings */
    Differential* getDifferential() {
        return _differential;
    }

    /** Get styling settings */
    Styling* getStyling() {
        return _styling;
    }

private:
    const std::uint32_t     _index;             /** Index */
    bool                    _linked;            /** Whether settings are linked to the settings of the first channel */
    QStringList             _datasetNames;      /** Dataset names */
    QString                 _datasetName;       /** Dataset name */
    Profile*                _profile;           /** Profile settings */
    Differential*           _differential;      /** Differential settings */
    Styling*                _styling;           /** Style settings */
    Points*                 _points;            /** Pointer to points dataset */

protected:
	friend class ConfigurationsModel;
	friend class Channels;
	friend class Differential;
	friend class Profile;
};

/** Get scoped enum in columns set to work */
inline uint qHash(Channel::Column key, uint seed) {
    return ::qHash(static_cast<uint>(key), seed);
}

/** Get scoped enum in rows set to work */
inline uint qHash(Channel::Row key, uint seed) {
    return ::qHash(static_cast<uint>(key), seed);
}